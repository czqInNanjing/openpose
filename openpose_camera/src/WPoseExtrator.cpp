//
// Created by fyf on 18-1-3.
//

#include "openpose_camera/WPoseExtrator.hpp"


void WPoseExtrator::work(std::shared_ptr<std::vector<WMyDatum>> &datumsPtr) {

    try {
        if (datumsPtr != nullptr && !datumsPtr->empty()) {
            for (auto &datum : *datumsPtr) {


                std::vector<int> dimensionSize = datum.poseKeypoints.getSize();
                std::cout << " Person number " << dimensionSize[0] << " Body parts Num " << dimensionSize[1]
                          << std::endl;
                for (int i = 0; i < dimensionSize[0]; ++i) {
                    for (int j = 0; j < dimensionSize[1]; ++j) {
                        std::vector<int> v1 = {i, j, 0};
                        std::vector<int> v2 = {i, j, 1};
                        std::cout << " Part" << j << " with x " << datum.poseKeypoints.at(v1) << "with y "
                                  << datum.poseKeypoints.at(v2) << std::endl;
                    }
                }


            }
        }
    }
    catch (const std::exception &e) {
        op::log("Some kind of unexpected error happened.");
        this->stop();
        op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
    }
}


