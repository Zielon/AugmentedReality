#ifndef PROJECT_SOLVER_H
#define PROJECT_SOLVER_H

#include "../../data_models/digit.cpp"
#include "neural_network.h"
#include <Eigen/Dense>
#include <iterator>
#include <map>
#include <string>

using namespace std;
using namespace Eigen;

class Solver {
public:
    Solver(vector<Digit *> &digits, NeuralNetwork &network);

    void train();

private:
    vector<Digit *> data;

    vector<Digit *> getMiniBatch(int size);

    void mergeDeltas(map<string, MatrixXd> &miniBatchCash, map<string, MatrixXd> &cash);

    NeuralNetwork &network;
};

#endif //PROJECT_SOLVER_H
