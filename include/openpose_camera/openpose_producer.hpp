#ifndef OPENPOSE_PRODUCER_HPP
#define OPENPOSE_PRODUCER_HPP

#include <openpose/headers.hpp>
#include <Spinnaker.h>
#include <openpose_camera/myDatum.hpp>


class WProducer : public op::WorkerProducer<std::shared_ptr<std::vector<WMyDatum>>>
{
public:
    WProducer(int _runTime, int _millSecondsBetweenImage);

    ~WProducer();

    void initializationOnThread();

    std::shared_ptr<std::vector<WMyDatum>> workProducer();

private:
    const std::chrono::time_point<std::chrono::high_resolution_clock> timerBegin = std::chrono::high_resolution_clock::now();
    const int runTime;
    const int millSecondsBetweenImage;
    bool initialized;
    Spinnaker::CameraList mCameraList;
    Spinnaker::SystemPtr mSystemPtr;
};

#endif // OPENPOSE_PRODUCER_HPP
