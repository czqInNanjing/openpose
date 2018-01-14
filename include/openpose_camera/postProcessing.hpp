#ifndef OPENPOSE_POST_PROCESSING_HPP
#define OPENPOSE_POST_PROCESSING_HPP

#include <openpose/headers.hpp>
#include <Spinnaker.h>
#include <openpose_camera/myDatum.hpp>
#include <Python.h>
#include <string>


class WPostProcessing : public op::Worker<std::shared_ptr<std::vector<WMyDatum>>>
{
public:
    WPostProcessing(std::string _modelPath, int _maxItems);
    ~WPostProcessing();
    void initializationOnThread();

    void work(std::shared_ptr<std::vector<WMyDatum>>& datumsPtr);
private:
    PyObject *pytorchModule,*model, *forwardFunc;
    const std::string modelPath;
    const int maxItems;
};

#endif //OPENPOSE_POST_PROCESSING_HPP