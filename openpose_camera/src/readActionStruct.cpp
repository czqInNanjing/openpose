#include "openpose_camera/Dataset.hpp"

Dataset readActionsFromFile(string filepath) {
    fstream infile(filepath);
    string line;
    map<string, vector<ActionStruct>> datas;
    map<string, string> videos2actions;


    while (std::getline(infile, line)) {
        if (line.find("frames") == line.npos) continue;
        string actionName = line.substr(line.find('_') + 1, line.rfind('_') - line.find('_') - 1);
        // TODO some video name does not end with \t and cause some video lost
        string videoName = line.substr(0, line.find('\t'));
        vector<ActionStruct> actions;
        line = line.substr(line.find("frames") + 6);
        while (true) {
            int start = stoi(line.substr(0, line.find('-')));
            int end = stoi(line.substr(line.find('-') + 1, line.find(' ') - line.find('-') - 1));
            actions.emplace_back(start, end, actionName);
            bool beforeToEnd = line.find(',') == line.npos;
            line = line.substr(line.find(',') + 1);
            if (line.find(',') == line.npos && beforeToEnd)
                break;
        }
        datas[videoName].insert(datas[videoName].end(), actions.begin(), actions.end());
        videos2actions[videoName] = actionName;
    }

    return Dataset(datas, videos2actions);
}

