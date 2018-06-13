#include "../headers/sandwicher.h"

tuple<double, double, double> Sandwicher::backwardSandwich(int layer, MatrixXd dout) {
    return {0, 0, 0};
}

Sandwicher::Sandwicher(NeuralNetwork &network) : network(network) { }
