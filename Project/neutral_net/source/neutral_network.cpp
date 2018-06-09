#include "../headers/neutral_network.h"
#include "../../data_models/consts.h"

#include <random>
#include <iostream>
#include <iterator>

using namespace std;

NeutralNetwork::NeutralNetwork(vector<int> layers) {
    weights = vector<MatrixXd *>(layers.size() - 1);
    biases = vector<MatrixXd *>(layers.size() - 1);
    initNetwork(layers);
}

MatrixXd NeutralNetwork::forwardPass(Digit *digit) {

    MatrixXd activation = *digit->pixels;
    auto size = weights.size();

    for (int i = 0; i < size - 1; i++) {
        MatrixXd *weight = weights[i];
        MatrixXd *bias = biases[i];
        MatrixXd z = (activation.transpose() * (*weight)).transpose() + *bias;

        // cout << activation.rows() << " -> " << weight->cols() << endl;

        activation = reLu(z);
        cash[Consts::RELU + to_string(i)] = activation;
        cash[Consts::ZS + to_string(i)] = z;
    }

    return (activation.transpose() * (*weights[size - 1])).transpose() + *biases[size - 1];
}

void NeutralNetwork::backwardPass(MatrixXd scores, Digit *digit) {
    auto softmaxPrime = softmaxLoss(std::move(scores), *digit->label);
    auto size = weights.size();
    for (long i = size - 2; i >= 0; i--) {
        MatrixXd weight = cash[Consts::RELU + to_string(i)];
        cash[Consts::WEIGHT + to_string(i)] = weight * softmaxPrime.transpose();
    }
}

void NeutralNetwork::initNetwork(vector<int> layers) {

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0, 5e-2);

    for (int i = 0; i < layers.size() - 1; i++) {
        int inputNeurons = layers[i];
        int outputNeurons = layers[i + 1];

        auto *bias = new MatrixXd(MatrixXd::Zero(outputNeurons, 1));
        auto *weight = new MatrixXd(MatrixXd::Zero(inputNeurons, outputNeurons).unaryExpr(
                [&distribution, &generator](double x) { return distribution(generator); }));

        weights[i] = weight;
        biases[i] = bias;
    }
}

MatrixXd NeutralNetwork::reLu(MatrixXd matrix) {
    return (matrix.array() < 0).select(0, matrix);
}

MatrixXd NeutralNetwork::softmaxLoss(MatrixXd x, MatrixXd y) {
    MatrixXd normalized = softmax(x);

    //double loss = (normalized.array() * y.array()).log().sum();

    return MatrixXd(normalized - y);
}

MatrixXd NeutralNetwork::softmax(MatrixXd matrix) {

    auto z_exp = MatrixXd(matrix.unaryExpr([](double x) { return exp(x); }));
    double sum = z_exp.sum();

    return z_exp.unaryExpr([&sum](auto x) { return x / sum; });
}

vector<MatrixXd *> NeutralNetwork::getWeights() {
    return weights;
}

vector<MatrixXd *> NeutralNetwork::getBiases() {
    return biases;
}

map<string, MatrixXd> NeutralNetwork::getCash() {
    return cash;
}
