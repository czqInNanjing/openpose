#include <openpose_camera/myDatum.hpp>
#include <openpose_camera/headers.hpp>



void WOutPuter::initializationOnThread() {
}

void WOutPuter::workConsumer(const std::shared_ptr<std::vector<WMyDatum>>& datumsPtr)
{
    try
    {
        // User's displaying/saving/other processing here
            // datum.cvOutputData: rendered frame with pose or heatmaps
            // datum.poseKeypoints: Array<float> with the estimated pose
        if (datumsPtr != nullptr && !datumsPtr->empty())
        {   

            op::log("Deal with data ");
            cv::imshow("User worker GUI", datumsPtr->at(0).cvOutputData);
            // It displays the image and sleeps at least 1 ms (it usually sleeps ~5-10 msec to display the image)
            cv::waitKey(1);
            op::log("Array floats of thecm datum is ");
        }
    }
    catch (const std::exception& e)
    {
        op::log("Some kind of unexpected error happened.");
        this->stop();
        op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
    }
}