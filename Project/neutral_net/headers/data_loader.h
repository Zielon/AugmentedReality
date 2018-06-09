#ifndef PROJECT_DATA_LOADER_H
#define PROJECT_DATA_LOADER_H

#include <vector>
#include <string>

#include "../../data_models/digit.cpp"

class DataLoader {
public:
    std::vector<Digit> load(std::string images, std::string labels, unsigned long size);

private:
    int reverseInt(int i);

    std::vector<std::vector<int>> loadRawImages(std::string path, unsigned long images);

    std::vector<int> loadRawLabels(std::string path, unsigned long labels);

    std::vector<Digit> mapToDigit(std::vector<std::vector<int>> images, std::vector<int> labels);
};

#endif //PROJECT_DATA_LOADER_H
