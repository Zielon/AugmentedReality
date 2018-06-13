#ifndef PROJECT_SANDWITCH_H
#define PROJECT_SANDWITCH_H

#include <tuple>
#include <Eigen/Dense>
#include "neutral_network.h"

using namespace std;
using namespace Eigen;

class Sandwicher {
public:
    Sandwicher(NeutralNetwork &network);

    tuple<double, double, double> backwardSandwich(int layer, MatrixXd delta);

private:
    NeutralNetwork network;
};

#endif //PROJECT_SANDWITCH_H
