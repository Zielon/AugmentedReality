#ifndef PROJECT_NEUTRAL_NETWORK_H
#define PROJECT_NEUTRAL_NETWORK_H

#include "../../data_models/digit.cpp"

#include <vector>
#include <map>
#include <tuple>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

class NeutralNetwork {
public:
    NeutralNetwork(vector<int> layers);

    void forwardPass(Digit *digit);

    void backwardPass(Digit *digit);

    MatrixXd sigmoid(MatrixXd &matrix);

    MatrixXd sigmoidPrime(MatrixXd &matrix);

    MatrixXd reLu(MatrixXd matrix);

    MatrixXd reLuPrime(MatrixXd matrix);

    MatrixXd softmax(MatrixXd matrix);

    MatrixXd softmaxPrime(MatrixXd x, MatrixXd y);

    void updateGradient(int miniBatchSize, map<string, MatrixXd> &cash);

    int predict(Digit *digit);

    vector<MatrixXd> getWeights();

    vector<MatrixXd> getBiases();

    map<string, MatrixXd> getCash();

private:
    vector<MatrixXd> weights;
    vector<MatrixXd> biases;
    map<string, MatrixXd> cash;

    string join(string name, int i);

    void initNetwork(vector<int> layers);
};

#endif //PROJECT_NEUTRAL_NETWORK_H
