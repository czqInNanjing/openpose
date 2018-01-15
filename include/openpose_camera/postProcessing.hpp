#ifndef OPENPOSE_POST_PROCESSING_HPP
#define OPENPOSE_POST_PROCESSING_HPP

#include <openpose/headers.hpp>
#include <Spinnaker.h>
#include <openpose_camera/myDatum.hpp>
#include <Python.h>
#include <string>
#include <vector>
#include <map>

using std::vector;
using std::map;
using std::string;
class WPostProcessing : public op::Worker<std::shared_ptr<std::vector<WMyDatum>>>
{
public:
    WPostProcessing(std::string _modelPath, int _maxItems, int _computeWithin);
    ~WPostProcessing();
    void initializationOnThread();
    vector<string> computeProbability();
    void work(std::shared_ptr<std::vector<WMyDatum>>& datumsPtr);
private:
    PyObject *pytorchModule,*model, *forwardFunc;
    const std::string modelPath;
    const int maxItems;
    const int computeWithin;
    vector<map<string, double>> recentData;
};

#endif //OPENPOSE_POST_PROCESSING_HPP