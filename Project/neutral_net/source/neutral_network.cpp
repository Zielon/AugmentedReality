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
    cash[join(Consts::RELU, -1)] = activation;

    for (int i = 0; i < size; i++) {
        MatrixXd weight = *weights[i];
        MatrixXd bias = *biases[i];
        MatrixXd z = (activation.transpose() * weight).transpose() + bias;

        // cout << activation.rows() << " -> " << weight.cols() << endl;

        activation = reLu(z);

        cash[join(Consts::RELU, i)] = activation;
        cash[join(Consts::ZS, i)] = z;
    }

    // Return the last summation without adding the activation function
    return cash[join(Consts::ZS, size - 1)];
}

void NeutralNetwork::backwardPass(MatrixXd scores, Digit *digit) {
    // the cost derivate
    MatrixXd delta = softmaxPrime(std::move(scores), *digit->label);
    auto layersNum = (int)weights.size() - 1;

    MatrixXd activation = cash[join(Consts::ZS, layersNum - 2)];

    cash[join(Consts::GRADIENT_WEIGHT, layersNum)] = activation * delta.transpose();
    cash[join(Consts::GRADIENT_BIAS, layersNum)] = delta;

    for (int i = layersNum - 1; i >= 0; i--) {
        MatrixXd weight = *weights[i + 1];

        auto prime = reLuPrime(cash[join(Consts::ZS, i)]);

        delta = (weight * delta).cwiseProduct(prime);

        cash[join(Consts::GRADIENT_BIAS, i)] = delta;
        cash[join(Consts::GRADIENT_WEIGHT, i)] = delta * cash[join(Consts::RELU, i - 1)].transpose();
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

MatrixXd NeutralNetwork::softmaxPrime(MatrixXd x, MatrixXd y) {
    MatrixXd normalized = softmax(std::move(x));
    auto z = normalized.array() * y.array();
    z.log();
    double loss = -z.sum();

    // cout << loss << endl;

    // The Softmax derivate
    return MatrixXd(normalized - y);
}

MatrixXd NeutralNetwork::softmax(MatrixXd matrix) {

    auto z_exp = MatrixXd(matrix.unaryExpr([](double x) { return exp(x); }));
    double sum = z_exp.sum();

    return z_exp.unaryExpr([&sum](auto x) { return x / sum; });
}

MatrixXd NeutralNetwork::reLuPrime(MatrixXd matrix) {
    return (matrix.array() <= 0).select(0, matrix);
}

void NeutralNetwork::updateGradient() {

    auto layers = (int)weights.size() - 1;

    for (int i = 0; i < layers; i++) {
        auto d_bias = cash[join(Consts::GRADIENT_BIAS, i)];
        auto d_weight = cash[join(Consts::GRADIENT_WEIGHT, i)].transpose();

        MatrixXd weight = *weights[i];
        MatrixXd bias = *biases[i];

        weights[i] = new MatrixXd(weight - 1e-3 * d_weight);
        biases[i] = new MatrixXd(bias - 1e-3 * d_bias);
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
