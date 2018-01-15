#include <openpose_camera/headers.hpp>
#include <iomanip>
#include <algorithm>

using namespace std;

WPostProcessing::WPostProcessing(string _modelPath, int _maxItems, int _computeWithin) : modelPath(std::move(_modelPath)),
                                                                     maxItems(_maxItems), computeWithin(_computeWithin) {}

WPostProcessing::~WPostProcessing() {
    Py_Finalize();
}

void WPostProcessing::initializationOnThread() {
    Py_Initialize();
    // TODO make it to gui set this path
    string path = "./assets/python";
    string chdir_cmd = string("sys.path.append(\"") + path + "\")";

    PyRun_SimpleString("import sys");
    PyRun_SimpleString(chdir_cmd.c_str());

    pytorchModule = PyImport_Import(PyString_FromString("ModelCPPCaller"));
    if (!pytorchModule) {
        op::error("[ERROR] Python get module failed.", __LINE__, __FUNCTION__, __FILE__);
    }
    PyObject *initFunc = PyObject_GetAttr(pytorchModule, PyString_FromString("initModel"));

    PyObject *args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, PyString_FromString(modelPath.c_str()));

    model = PyObject_CallObject(initFunc, args);
    if (!model) {
        op::error("[ERROR] Python get model failed.", __LINE__, __FUNCTION__, __FILE__);
    }
    forwardFunc = PyObject_GetAttr(pytorchModule, PyString_FromString("forwardModel"));
}

namespace op {
    void addInfo(cv::Mat &cvOutputData, string pros, int count) {
        try {
            // Security checks
            if (cvOutputData.empty())
                error("Wrong input element (empty cvOutputData).", __LINE__, __FUNCTION__, __FILE__);
            // Size
            const auto borderMargin = intRound(fastMax(cvOutputData.cols, cvOutputData.rows) * 0.025);
            // Used colors
            const cv::Scalar white{255, 255, 255};

            putTextOnCvMat(cvOutputData, pros,
                           {borderMargin, (int) (5 + count * 2) * borderMargin}, white, false);

        }
        catch (const std::exception &e) {
            error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }
    }
}

vector<string> WPostProcessing::computeProbability() {
    while (recentData.size() > computeWithin) {
        recentData.erase(recentData.begin());
    }
    map<string, double> newProbability;
    for(auto& oneMap : recentData) {
        for( auto& onePair : oneMap) {
            newProbability[onePair.first] += onePair.second;
        }
    }
    vector<pair<string, double>> sortPair(newProbability.begin(), newProbability.end());
    std::sort(sortPair.begin(), sortPair.end(), [](const std::pair<string, double> &left, const std::pair<string, double> &right) {
        return left.second > right.second;
    });
    vector<string> result;
    if(sortPair.size() >= maxItems) {
        for (int i = 0; i < maxItems; ++i) {
            stringstream ss;
            ss << fixed << setprecision(2) << sortPair[i].first << " " << sortPair[i].second / recentData.size();
            result.push_back(ss.str());
        }
    }
    return result;
}

void WPostProcessing::work(std::shared_ptr<std::vector<WMyDatum>> &datumsPtr) {

    // datum.cvOutputData: rendered frame with pose or heatmaps
    // datum.poseKeypoints: Array<float> with the estimated pose

    try {
        if (datumsPtr != nullptr && !datumsPtr->empty()) {
            for (auto &datum : *datumsPtr) {
                string args;
                int rows = datum.cvInputData.rows;
                int cols = datum.cvInputData.cols;
                std::vector<int> dimensionSize = datum.poseKeypoints.getSize();
                if (!dimensionSize.empty()) {
                    for (int i = 0; i < 1; ++i) {
                        for (int j = 0; j < dimensionSize[1]; ++j) {
                            std::vector<int> v1 = {i, j, 0};
                            std::vector<int> v2 = {i, j, 1};
                            // Normalize the data
                            args += to_string(datum.poseKeypoints.at(v1) / cols) + "," +
                                    to_string(datum.poseKeypoints.at(v2) / rows) + " ";

                        }
                    }

                    PyObject *pargs = PyTuple_New(2);
                    PyTuple_SetItem(pargs, 0, model);
                    PyTuple_SetItem(pargs, 1, PyString_FromString(args.c_str()));
                    PyObject *result = PyObject_CallObject(forwardFunc, pargs);

                    PyObject *actionNameTuple = PyList_AsTuple(PyTuple_GetItem(result, 0));
                    PyObject *actionProbilityTuple = PyList_AsTuple(PyTuple_GetItem(result, 1));
                    map<string, double> thisData;

                    int size = PyTuple_Size(actionNameTuple);

                    for (int k = 0; k < size; k++) {
                        thisData[PyString_AsString(PyTuple_GetItem(actionNameTuple, k))] = PyFloat_AsDouble(PyTuple_GetItem(actionProbilityTuple, k));
                    }
                    recentData.push_back(thisData);
                    vector<string> strResult = computeProbability();

                    for (int l = 0; l < strResult.size(); ++l) {
                        op::addInfo(datum.cvOutputData, strResult[l], l);
                        cout << strResult[l] << " ";
                    }
                    cout << endl;


                } else {

                }

            }


        }
    }
    catch (const std::exception &e) {
        op::log("Some kind of unexpected error happened.");
        this->stop();
        op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
    }
}

