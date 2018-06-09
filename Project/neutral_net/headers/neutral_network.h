#ifndef PROJECT_NEUTRAL_NETWORK_H
#define PROJECT_NEUTRAL_NETWORK_H

#include "../../data_models/digit.cpp"

#include <vector>
#include <map>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

class NeutralNetwork {
public:
    NeutralNetwork(vector<int> layers);

    void forwardPass(Digit *digit);

    void backwardPass();

private:
    vector<MatrixXd *> weights;
    vector<MatrixXd *> biases;
    map<string, MatrixXd> cash;

    const string RELU = "RELU";
    const string ZS = "ZS";

    MatrixXd reLu(MatrixXd matrix);

    MatrixXd softmax(MatrixXd matrix);

    MatrixXd softmaxLoss(MatrixXd x, MatrixXd y);

    void initNetwork(vector<int> layers);
};

#endif //PROJECT_NEUTRAL_NETWORK_H
