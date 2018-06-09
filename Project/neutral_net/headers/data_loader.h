#ifndef PROJECT_DATA_LOADER_H
#define PROJECT_DATA_LOADER_H

#include <vector>
#include <string>

#include "../../data_models/digit.cpp"

using namespace std;

class DataLoader {
public:
    vector<Digit *> load(string images, string labels, unsigned long size);

private:
    int reverseInt(int i);

    vector<vector<int>> loadRawImages(string path, unsigned long images);

    vector<int> loadRawLabels(string path, unsigned long labels);

    vector<Digit *> mapToDigit(vector<vector<int>> images, vector<int> labels);
};

#endif //PROJECT_DATA_LOADER_H
