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
    int truth;

    Digit(std::vector<int> pixels, int label) {
        this->label = new MatrixXd(MatrixXd::Zero(10, 1).cast<double>());
        this->truth = label;
        this->pixels = new MatrixXd(MatrixXd::Zero(784, 1).cast<double>());

        for (int i = 0; i < 784; i++) (*this->pixels)(i, 0) = (double) pixels[i] / 256.0;
        (*this->label)(label, 0) = 1;
    };
};

#endif //PROJECT_DIGIT_H
