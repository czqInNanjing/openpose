#include <openpose_camera/headers.hpp>
#include <sstream>

using namespace std;

WPostProcessing::WPostProcessing() {}

WPostProcessing::~WPostProcessing() {
    Py_Finalize();
}

void WPostProcessing::initializationOnThread() {
    Py_Initialize();

    string path = "/home/fyf/Desktop/Practice/ActionClassifier";
    string chdir_cmd = string("sys.path.append(\"") + path + "\")";
    const char *cstr_cmd = chdir_cmd.c_str();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString(cstr_cmd);

    pytorchModule = PyImport_Import(PyString_FromString("ModelCPPCaller"));
    if (!pytorchModule) // 加载模块失败
    {
        cout << "[ERROR] Python get module failed." << endl;
        return;
    }
    cout << "[INFO] Python get module succeed." << endl;
    PyObject *initFunc = PyObject_GetAttr(pytorchModule, PyString_FromString("initModel"));

    PyObject *args = PyTuple_New(0);

    model = PyObject_CallObject(initFunc, args);
    if (model) {
        cout << "Get model successfully";
    }
    forwardFunc = PyObject_GetAttr(pytorchModule, PyString_FromString("forwardModel"));
}


void WPostProcessing::work(std::shared_ptr<std::vector<WMyDatum>> &datumsPtr) {

    // datum.cvOutputData: rendered frame with pose or heatmaps
    // datum.poseKeypoints: Array<float> with the estimated pose

    try {
        if (datumsPtr != nullptr && !datumsPtr->empty()) {
            for (auto &datum : *datumsPtr) {
                string args;
                std::vector<int> dimensionSize = datum.poseKeypoints.getSize();
                if (!dimensionSize.empty()) {
                    for (int i = 0; i < 1; ++i) {
                        for (int j = 0; j < dimensionSize[1]; ++j) {
                            std::vector<int> v1 = {i, j, 0};
                            std::vector<int> v2 = {i, j, 1};
                            args += to_string(datum.poseKeypoints.at(v1)) + "," +
                                    to_string(datum.poseKeypoints.at(v1)) + " ";

                        }
                    }
                    PyObject *pargs = PyTuple_New(2);
                    PyTuple_SetItem(pargs, 0, model);
                    PyTuple_SetItem(pargs, 1, PyString_FromString(args.c_str()));
                    PyObject *result = PyObject_CallObject(forwardFunc, pargs);
                    cout << PyString_AsString(result) << endl;
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