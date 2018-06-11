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
    auto size = (int) weights.size();

    MatrixXd activation = *digit->pixels;
    cash[join(Consts::ACTIVATION, -1)] = activation;

    for (int i = 0; i < size; i++) {
        MatrixXd weight = *weights[i];
        MatrixXd bias = *biases[i];
        MatrixXd z = (activation.transpose() * weight).transpose() + bias;

        // cout << activation.rows() << " -> " << weight.cols() << endl;

        activation = sigmoid(z);

        cash[join(Consts::ACTIVATION, i)] = activation;
        cash[join(Consts::ZS, i)] = z;
    }

    return cash[join(Consts::ACTIVATION, size - 1)];
}

void NeutralNetwork::backwardPass(Digit *digit) {

    auto layersNum = (int) weights.size() - 1;

    auto activation = cash[join(Consts::ACTIVATION, layersNum)];
    auto z = cash[join(Consts::ZS, layersNum)];

    MatrixXd delta = softmaxPrime(activation, *digit->label).cwiseProduct(sigmoidPrime(z));

    cash[join(Consts::GRADIENT_BIAS, layersNum)] = delta;
    cash[join(Consts::GRADIENT_WEIGHT, layersNum)] = cash[join(Consts::ZS, layersNum - 1)] * delta.transpose();

    for (int i = layersNum - 1; i >= 0; i--) {
        MatrixXd weight = *weights[i + 1];

        auto prime = sigmoidPrime(cash[join(Consts::ZS, i)]);

        delta = (weight * delta).cwiseProduct(prime);

        cash[join(Consts::GRADIENT_BIAS, i)] = delta;

        cash[join(Consts::GRADIENT_WEIGHT, i)] = delta * cash[join(Consts::ACTIVATION, i - 1)].transpose();
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

MatrixXd NeutralNetwork::sigmoid(MatrixXd z) {
    return z.unaryExpr([](double x) {
        return 1.0 / (1.0 + exp(-x));
    });
}

MatrixXd NeutralNetwork::sigmoidPrime(MatrixXd z) {
    return sigmoid(z).cwiseProduct(sigmoid(z).unaryExpr([](double x) {
        return 1.0 - x;
    }));
}

MatrixXd NeutralNetwork::reLu(MatrixXd matrix) {
    return (matrix.array() < 0).select(0, matrix);
}

MatrixXd NeutralNetwork::reLuPrime(MatrixXd matrix) {
    return matrix.unaryExpr([](double x) { return x >= 0.0 ? 1.0 : 0.0; });
}

MatrixXd NeutralNetwork::softmax(MatrixXd matrix) {

    auto z_exp = MatrixXd(matrix.unaryExpr([](double x) { return exp(x); }));
    double sum = z_exp.sum();

    return z_exp.unaryExpr([&sum](auto x) { return x / sum; });
}

MatrixXd NeutralNetwork::softmaxPrime(MatrixXd x, MatrixXd y) {
    MatrixXd z = x.cwiseProduct(y);

    double loss = -z.sum();

    //cout << loss << endl;

    return x - y;
}

void NeutralNetwork::updateGradient(int miniBatchSize) {

    auto layers = (int) weights.size() - 1;

    for (int i = 0; i < layers; i++) {
        auto d_bias = cash[join(Consts::GRADIENT_BIAS, i)];
        auto d_weight = cash[join(Consts::GRADIENT_WEIGHT, i)].transpose();

        MatrixXd weight = *weights[i];
        MatrixXd bias = *biases[i];

        weight = weight - (0.5 / miniBatchSize * d_weight);
        bias = bias - (0.5 / miniBatchSize * d_bias);

//        weights[i] = &weight;
//        biases[i] = &bias;
    }
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

string NeutralNetwork::join(string name, int i) {
    return name + to_string(i);
}
