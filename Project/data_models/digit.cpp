#ifndef PROJECT_DIGIT_H
#define PROJECT_DIGIT_H

#include <utility>
#include <vector>

struct Digit {
    std::vector<int> pixels;
    int label;
    Digit(std::vector<int> pixels, int label) : pixels(std::move(pixels)), label(label) {};
    Digit(){};
};

#endif //PROJECT_DIGIT_H
