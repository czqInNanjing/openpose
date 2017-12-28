#ifndef OPENPOSE_POST_PROCESSING_HPP
#define OPENPOSE_POST_PROCESSING_HPP

#include <openpose/headers.hpp>
#include <Spinnaker.h>
#include <openpose_camera/myDatum.hpp>



class WPostProcessing : public op::Worker<std::shared_ptr<std::vector<WMyDatum>>>
{
public:
    WPostProcessing();

    void initializationOnThread();

    void work(std::shared_ptr<std::vector<WMyDatum>>& datumsPtr);
private:

};

#endif //OPENPOSE_POST_PROCESSING_HPP