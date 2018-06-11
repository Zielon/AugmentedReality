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
        this->label = new MatrixXd(MatrixXd::Zero(10, 1));
        this->pixels = new MatrixXd(Map<Matrix<int, 784, 1>>(pixels.data()).cast<double>());
        this->truth = label;
        (*this->label)(label, 0) = 1;
    };
};

#endif //PROJECT_DIGIT_H
