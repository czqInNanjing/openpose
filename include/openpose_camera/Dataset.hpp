#ifndef OPENPOSE_DATASET_HPP
#define OPENPOSE_DATASET_HPP

#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <sstream>
#include <string>
#include <map>

using namespace std;

// goal :  video ->  (action and their axis)

struct ActionStruct
{
    int startFrame;
    int endFrame;
    string type;
    ActionStruct(int startFrame, int endFrame, string type) : startFrame(startFrame), endFrame(endFrame),
                                                              type(std::move(type)) {}
};

struct Dataset
{
    map<string, vector<ActionStruct>> datas; // "video name" -> a list of running example
    map<string, string> videos2actions;

    Dataset(map<string, vector<ActionStruct>> datas, map<string, string> videos2actions) : datas(std::move(datas)),
                                                                                           videos2actions(
                                                                                                   std::move(videos2actions)) {}
};

#endif // OPENPOSE_DATASET_HPP