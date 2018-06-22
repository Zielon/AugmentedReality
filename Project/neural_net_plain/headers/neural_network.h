#ifndef PROJECT_NEUTRAL_NETWORK_H
#define PROJECT_NEUTRAL_NETWORK_H

#include "../../data_models/digit.cpp"

#include <vector>
#include <map>
#include <tuple>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

class NeuralNetwork {
public:
    NeuralNetwork(vector<int> layers);

    void forwardPass(Digit *digit);

    void backwardPass(Digit *digit);

    void updateGradient(double miniBatchSize);

    int predict(Digit *digit);

    map<string, MatrixXd> &getCash();

private:
    vector<MatrixXd> weights;
    vector<MatrixXd> biases;
    map<string, MatrixXd> cash;

    string join(string name, int i);

    MatrixXd sigmoid(MatrixXd matrix);

    MatrixXd sigmoidPrime(MatrixXd matrix);

    MatrixXd reLu(MatrixXd matrix);

    MatrixXd reLuPrime(MatrixXd matrix);

    MatrixXd softmax(MatrixXd matrix);

    MatrixXd softmaxPrime(MatrixXd x, MatrixXd y);

    void initNetwork(vector<int> layers);
};

#endif //PROJECT_NEUTRAL_NETWORK_H
