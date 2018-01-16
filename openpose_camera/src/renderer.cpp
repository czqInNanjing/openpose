#include <openpose_camera/myDatum.hpp>
#include <openpose_camera/headers.hpp>


void WOutPuter::initializationOnThread() {

    cv::destroyAllWindows();
}

void WOutPuter::workConsumer(const std::shared_ptr<std::vector<WMyDatum>> &datumsPtr) {
    try {
        // User's displaying/saving/other processing here
        // datum.cvOutputData: rendered frame with pose or heatmaps
        // datum.poseKeypoints: Array<float> with the estimated pose
        if (datumsPtr != nullptr && !datumsPtr->empty()) {
            for (auto &datum : *datumsPtr) {
                if (saveToVideo) {
                    toBeSaved.push_back(datum);
                }
//                frameDisplayer.displayFrame(datum.cvOutputData, 1);
            }
        }
    }
    catch (const std::exception &e) {
        op::log("Some kind of unexpected error happened.");
        this->stop();
        op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
    }
}

void WOutPuter::tryStop() {
    try {
        cv::destroyAllWindows();
        if (saveToVideo) {
            if (!toBeSaved.empty()) {
                using namespace std::chrono;
                const auto timePoint = system_clock::to_time_t(system_clock::now());
                const std::string name = outputPath + ctime(&timePoint) + ".avi";

                cv::Size size(
                        toBeSaved[0].cvOutputData.cols,
                        toBeSaved[0].cvOutputData.rows
                );

                cv::VideoWriter outputVideo;
                if (outputVideo.open(name, CV_FOURCC('M','J','P','G'), fps, size, true)) {
                    for (auto outputFrame : toBeSaved) {
                        outputVideo << outputFrame.cvOutputData;
                    }
                } else {
                    op::log("Video was not opened !!! ");
                }
            }
        }
    }
    catch (const std::exception &e) {
        op::log("Error happened when saving the video");
        this->stop();
        op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
    }
    op::WorkerConsumer<std::shared_ptr<std::vector<WMyDatum>>>::tryStop();
}