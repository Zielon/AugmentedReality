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

        //cout << activation.rows() << " -> " << weight->cols() << endl;

        activation = reLu(z);
        cash[Consts::RELU + to_string(i)] = activation;
        cash[Consts::ZS + to_string(i)] = z;
    }

    return (activation.transpose() * (*weights[size - 1])).transpose() + *biases[size - 1];
}

void NeutralNetwork::backwardPass(MatrixXd scores, Digit *digit) {
    auto softmaxPrime = softmaxLoss(scores, *digit->label);
    auto size = weights.size();
    for (int i = size - 2; i >= 0; i--) {
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

        auto *weight = new MatrixXd(inputNeurons, outputNeurons);
        auto *bias = new MatrixXd(outputNeurons, 1);

        for (int r = 0; r < inputNeurons; r++) {
            for (int c = 0; c < outputNeurons; c++) {
                weight->operator()(r, c) = distribution(generator);
                bias->operator()(c, 0) = 0;
            }
        }

        weights[i] = weight;
        biases[i] = bias;

        //cout << "Matrix m:\n" << *biases[i] << std::endl;
    }

    cout << "Network ready" << std::endl;
}

MatrixXd NeutralNetwork::reLu(MatrixXd matrix) {
    return (matrix.array() < 0).select(0, matrix);
}

MatrixXd NeutralNetwork::softmaxLoss(MatrixXd x, MatrixXd y) {
    MatrixXd normalized = softmax(x);

    double loss = (normalized.array() * y.array()).log().sum();

    return MatrixXd(normalized - y);
}

MatrixXd NeutralNetwork::softmax(MatrixXd matrix) {

    auto z_exp = MatrixXd(matrix.unaryExpr([](double x) { return exp(x); }));
    double sum = z_exp.sum();

    for (int i = 0; i < z_exp.rows(); i++) {
        double e = z_exp(i, 0);
        double a = e / sum;
        z_exp(i, 0) = a;
    }

    return z_exp;
}

vector<MatrixXd *> NeutralNetwork::getWeights() {
    return weights;
}

vector<MatrixXd *> NeutralNetwork::getBiases() {
    return biases;
}
