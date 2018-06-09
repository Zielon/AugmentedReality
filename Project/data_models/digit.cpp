#ifndef PROJECT_DIGIT_H
#define PROJECT_DIGIT_H

#include <utility>
#include <vector>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

struct Digit {
    MatrixXd *pixels;
    MatrixXd *label;

    Digit(std::vector<int> pixels, int label) {
        this->label = new MatrixXd(10, 1);
        for (int i = 0; i < 10; i++) (*this->label)(i, 0) = 0;

        (*this->label)(label, 0) = 1;

        this->pixels = new MatrixXd(784, 1);
        for (int i = 0; i < 784; i++) (*this->pixels)(i, 0) = pixels[i];
    };

    Digit() {};
};

#endif //PROJECT_DIGIT_H
