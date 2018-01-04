//
// Created by fyf on 18-1-3.
//

#ifndef OPENPOSE_WPOSEEXTRATOR_HPP
#define OPENPOSE_WPOSEEXTRATOR_HPP

#include <openpose/headers.hpp>
#include <Spinnaker.h>
#include <openpose_camera/myDatum.hpp>
#include "openpose_camera/Dataset.hpp"




struct OneActionAxis {
    vector<vector<float>> x;
    vector<vector<float>> y;
    OneActionAxis() = default;
};



class WPoseExtrator : public op::Worker<std::shared_ptr<std::vector<WMyDatum>>>
{
public:
    WPoseExtrator(std::vector<ActionStruct>& _dataset): dataset(_dataset){};

    ~WPoseExtrator(){};

    void initializationOnThread(){};

    void work(std::shared_ptr<std::vector<WMyDatum>>& datumsPtr);

private:
    const std::vector<ActionStruct>& dataset;
    int currentFrame = 0;
    int currentPart = 0;
    OneActionAxis oneActionAxis;
};


#endif //OPENPOSE_WPOSEEXTRATOR_HPP
