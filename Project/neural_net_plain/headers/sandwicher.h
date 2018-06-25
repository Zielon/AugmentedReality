#ifndef PROJECT_SANDWITCH_H
#define PROJECT_SANDWITCH_H

#include <tuple>
#include <Eigen/Dense>
#include "neural_network.h"

using namespace std;
using namespace Eigen;

class Sandwicher {
public:
    Sandwicher(NeuralNetwork &network);

    tuple<double, double, double> backwardSandwich(int layer, MatrixXd delta);

private:
    NeuralNetwork network;
};

#endif //PROJECT_SANDWITCH_H
