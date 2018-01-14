#include <openpose_camera/headers.hpp>
#include <sstream>
#include <chrono>
#include <cstdio> // std::snprintf
#include <limits> // std::numeric_limits
#include <openpose/utilities/fastMath.hpp>
#include <openpose/utilities/openCv.hpp>
#include <openpose/gui/guiInfoAdder.hpp>
#include <iomanip>

using namespace std;

WPostProcessing::WPostProcessing(string _modelPath, int _maxItems) : modelPath(_modelPath), maxItems(_maxItems) {}

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
    if (!pytorchModule)
    {
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
                    stringstream ss, ss1, ss2;
                    PyObject *pargs = PyTuple_New(2);
                    PyTuple_SetItem(pargs, 0, model);
                    PyTuple_SetItem(pargs, 1, PyString_FromString(args.c_str()));
                    PyObject *result = PyObject_CallObject(forwardFunc, pargs);
//                    cout << rows << cols << args << endl;
                    ss << fixed << setprecision(2) << PyString_AsString(PyTuple_GetItem(result, 0)) << " "
                       << PyFloat_AsDouble(PyTuple_GetItem(result, 1));
                    ss1 << fixed << setprecision(2) << PyString_AsString(PyTuple_GetItem(result, 2)) << " "
                        << PyFloat_AsDouble(PyTuple_GetItem(result, 3));
                    ss2 << fixed << setprecision(2) << PyString_AsString(PyTuple_GetItem(result, 4)) << " "
                        << PyFloat_AsDouble(PyTuple_GetItem(result, 5));
                    op::addInfo(datum.cvOutputData, ss.str(), 0);
                    op::addInfo(datum.cvOutputData, ss1.str(), 1);
                    op::addInfo(datum.cvOutputData, ss2.str(), 2);
                    cout << ss.str() << " " << ss1.str() << " " << ss2.str() << endl;

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

