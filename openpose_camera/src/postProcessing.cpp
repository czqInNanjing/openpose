#include <openpose_camera/headers.hpp>

WPostProcessing::WPostProcessing() {}


void WPostProcessing::initializationOnThread()
{
}

void WPostProcessing::work(std::shared_ptr<std::vector<WMyDatum>>& datumsPtr)
{
    
        // datum.cvOutputData: rendered frame with pose or heatmaps
        // datum.poseKeypoints: Array<float> with the estimated pose
    try
    {
        // TODO: detect wave hands by checking the poseKeyPoints between picture 
    }
    catch (const std::exception& e)
    {
        op::log("Some kind of unexpected error happened.");
        this->stop();
        op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
    }
}