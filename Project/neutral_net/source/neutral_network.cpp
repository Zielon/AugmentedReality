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
        MatrixXd z = weight.transpose() * activation + bias;

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
    cash[join(Consts::GRADIENT_WEIGHT, layersNum)] = delta * cash[join(Consts::ACTIVATION, layersNum - 1)].transpose();

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
    std::normal_distribution<double> weights_distribution(0, 1);
    std::normal_distribution<double> bias_distribution(0, 1);

    for (int i = 0; i < layers.size() - 1; i++) {
        int inputNeurons = layers[i];
        int outputNeurons = layers[i + 1];

        auto *bias = new MatrixXd(MatrixXd::Zero(outputNeurons, 1).cast<double>().unaryExpr(
                [&bias_distribution, &generator](double x) { return bias_distribution(generator); }));

        auto *weight = new MatrixXd(MatrixXd::Zero(inputNeurons, outputNeurons).cast<double>().unaryExpr(
                [&weights_distribution, &generator](double x) { return weights_distribution(generator); }));

        weights[i] = weight;
        biases[i] = bias;
    }
}

MatrixXd NeutralNetwork::sigmoid(MatrixXd &z) {
    return z.unaryExpr([](double x) { return 1.0 / (1.0 + exp(-x)); });
}

MatrixXd NeutralNetwork::sigmoidPrime(MatrixXd &z) {
    auto sig = sigmoid(z);
    return sig.cwiseProduct(sig.unaryExpr([](double x) { return 1.0 - x; }));
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
    return x - y;
}

void NeutralNetwork::updateGradient(int miniBatchSize, map<string, MatrixXd> miniBatchCash) {

    auto layers = (int) weights.size();

    for (int i = 0; i < layers; i++) {
        auto d_bias = 0.5 / miniBatchSize * miniBatchCash[join(Consts::GRADIENT_BIAS, i)];
        auto d_weight = 0.5 / miniBatchSize * miniBatchCash[join(Consts::GRADIENT_WEIGHT, i)].transpose();

        MatrixXd weight = *weights[i];
        MatrixXd bias = *biases[i];

        //cout << weight.rows() << " -> " << weight.cols() << endl;

        auto b = new MatrixXd(bias - d_bias);
        auto w = new MatrixXd(weight - d_weight);

        delete (biases[i]);
        delete (weights[i]);

        biases[i] = b;
        weights[i] = w;
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

int NeutralNetwork::predict(Digit *digit) {
    auto size = (int) weights.size();

    MatrixXd activation = *digit->pixels;

    for (int i = 0; i < size; i++) {
        MatrixXd weight = *weights[i];
        MatrixXd bias = *biases[i];
        MatrixXd z = weight.transpose() * activation + bias;
        activation = sigmoid(z);
    }

    int maxIndex = -1;
    double maxElement = 0;
    for(int i = 0; i < 10; i++){
        if(activation(i, 0) >= maxElement){
            maxElement = activation(i, 0);
            maxIndex = i;
        }
    }

    return maxIndex;
}
