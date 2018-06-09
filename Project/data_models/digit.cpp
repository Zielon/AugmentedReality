#ifndef PROJECT_DIGIT_H
#define PROJECT_DIGIT_H

#include <utility>
#include <vector>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

struct Digit {
    MatrixXd *pixels;
    vector<int> label;

    Digit(std::vector<int> pixels, int label) {
        this->label = vector<int>(10);
        this->label[label - 1] = 1;
        this->pixels = new MatrixXd(784, 1);
        for (int i = 0; i < 784; i++) (*this->pixels)(i, 0) = pixels[i];
    };

    Digit() {};
};

#endif //PROJECT_DIGIT_H
