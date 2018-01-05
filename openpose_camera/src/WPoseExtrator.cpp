//
// Created by Qiang on 18-1-3.
//

#include "openpose_camera/WPoseExtrator.hpp"
#include <fstream>

void writeToFile(string actionName, OneActionAxis axis) {
    fstream fout("data/action/" + actionName + ".txt", fstream::app);
    fout << "Num of frames: " <<  axis.x.size() << std::endl;
    for (int i = 0; i < axis.x.size(); ++i) {
        for (int j = 0; j < axis.x[i].size(); ++j) {
            fout << axis.x[i][j] << "," << axis.y[i][j] << " ";
        }
        fout << std::endl;
    }
    fout.flush();
    fout.close();
}



void WPoseExtrator::work(std::shared_ptr<std::vector<WMyDatum>> &datumsPtr) {

    try {
        if (datumsPtr != nullptr && !datumsPtr->empty()) {
            for (auto &datum : *datumsPtr) {
                currentFrame++;
                if(currentPart >= dataset.size()) return;

                if(dataset[currentPart].startFrame == currentFrame) {
                    std::cout << "Start one part " << currentPart << std::endl;
                    oneActionAxis = OneActionAxis();
                }

                std::vector<int> dimensionSize = datum.poseKeypoints.getSize();
                if(!dimensionSize.empty()) {
                    for (int i = 0; i < dimensionSize[0]; ++i) {              // nums of people detected, 0 or 1 in KTH set
                        vector<float> x;
                        vector<float> y;
                        for (int j = 0; j < dimensionSize[1]; ++j) {          // 18 for COCO
                            std::vector<int> v1 = {i, j, 0};
                            std::vector<int> v2 = {i, j, 1};
                            x.emplace_back(datum.poseKeypoints.at(v1));
                            y.emplace_back(datum.poseKeypoints.at(v2));
                        }
                        oneActionAxis.x.push_back(x);
                        oneActionAxis.y.push_back(y);
                    }
                }


                if(dataset[currentPart].endFrame == currentFrame) {
                    writeToFile(dataset[currentPart].type, oneActionAxis);
                    std::cout << "Finish one part " << currentPart << std::endl;
                    currentPart++;
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


