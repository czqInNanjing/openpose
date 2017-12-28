#ifndef OPENPOSE_OUTPUTER_HPP
#define OPENPOSE_OUTPUTER_HPP

#include <thread>
#include <vector>
#include <openpose/headers.hpp>
#include <openpose_camera/myDatum.hpp>

// This worker will do 3-D rendering
class WOutPuter : public op::WorkerConsumer<std::shared_ptr<std::vector<WMyDatum>>>
{
public:
    WOutPuter(){}

    void initializationOnThread();

    void workConsumer(const std::shared_ptr<std::vector<WMyDatum>>& datumsPtr);

private:
};

#endif // OPENPOSE_OUTPUTER_HPP
