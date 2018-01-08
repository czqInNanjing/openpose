#ifndef OPENPOSE_POST_PROCESSING_HPP
#define OPENPOSE_POST_PROCESSING_HPP

#include <openpose/headers.hpp>
#include <Spinnaker.h>
#include <openpose_camera/myDatum.hpp>
#include <Python.h>


class WPostProcessing : public op::Worker<std::shared_ptr<std::vector<WMyDatum>>>
{
public:
    WPostProcessing();
    ~WPostProcessing();
    void initializationOnThread();

    void work(std::shared_ptr<std::vector<WMyDatum>>& datumsPtr);
private:
    PyObject *pytorchModule,*model, *forwardFunc;
};

#endif //OPENPOSE_POST_PROCESSING_HPP