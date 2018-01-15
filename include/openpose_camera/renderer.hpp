#ifndef OPENPOSE_OUTPUTER_HPP
#define OPENPOSE_OUTPUTER_HPP

#include <thread>
#include <vector>
#include <openpose/headers.hpp>
#include <openpose_camera/myDatum.hpp>


class WOutPuter : public op::WorkerConsumer<std::shared_ptr<std::vector<WMyDatum>>>
{
public:
    WOutPuter(double _fps=5.0, bool _saveToVideo= false, std::string _outputPath="output/"): fps(_fps), saveToVideo(_saveToVideo), outputPath(_outputPath){}

    void initializationOnThread();

    void workConsumer(const std::shared_ptr<std::vector<WMyDatum>>& datumsPtr);

    void tryStop();

private:
    // input source fps
    const double fps;
    const bool saveToVideo;
    std::vector<WMyDatum> toBeSaved;
    const std::string outputPath;
    op::FrameDisplayer frameDisplayer;
};

#endif // OPENPOSE_OUTPUTER_HPP
