#include "../headers/neural_network.h"
#include "../../data_models/consts.h"

#include <random>
#include <iostream>
#include <iterator>

using namespace std;

NeuralNetwork::NeuralNetwork(vector<int> layers) {
    weights = vector<MatrixXd>(layers.size() - 1);
    biases = vector<MatrixXd>(layers.size() - 1);
    initNetwork(layers);
}

void NeuralNetwork::forwardPass(Digit *digit) {
    auto size = (int) weights.size();

    MatrixXd activation = *digit->pixels;
    cash[join(Consts::ACTIVATION, -1)] = activation;

    for (int i = 0; i < size; i++) {
        MatrixXd weight = weights[i];
        MatrixXd bias = biases[i];
        MatrixXd z = (weight.transpose() * activation) + bias;

        //cout << activation.rows() << " -> " << weight.cols() << endl;

        activation = sigmoid(z);

        cash[join(Consts::Z, i)] = z;
        cash[join(Consts::ACTIVATION, i)] = activation;
    }
}

void NeuralNetwork::backwardPass(Digit *digit) {

    auto lastLayer = (int) weights.size() - 1;

    auto activation = cash[join(Consts::ACTIVATION, lastLayer)];
    auto z = cash[join(Consts::Z, lastLayer)];
    auto y = *digit->label;

    MatrixXd delta = softmaxPrime(activation, y).cwiseProduct(sigmoidPrime(z));

    cash[join(Consts::GRADIENT_BIAS, lastLayer)] = delta;
    cash[join(Consts::GRADIENT_WEIGHT, lastLayer)] = delta * cash[join(Consts::ACTIVATION, lastLayer - 1)].transpose();

    for (int i = lastLayer - 1; i >= 0; i--) {
        MatrixXd weight = weights[i + 1];

        activation = cash[join(Consts::ACTIVATION, i - 1)];
        z = cash[join(Consts::Z, i)];

        delta = (weight * delta).cwiseProduct(sigmoidPrime(z));

        cash[join(Consts::GRADIENT_BIAS, i)] = delta;
        cash[join(Consts::GRADIENT_WEIGHT, i)] = delta * activation.transpose();
    }
}

void NeuralNetwork::updateGradient(double miniBatchSize) {

    auto layers = (int) weights.size();

    for (int i = 0; i < layers; i++) {
        auto d_bias = 0.5 * cash[join(Consts::GRADIENT_BIAS, i)];
        auto d_weight = 0.5 * cash[join(Consts::GRADIENT_WEIGHT, i)].transpose();

        //cout << weights[i].rows() << " -> " << weights[i].cols() << endl;

        biases[i] -= d_bias;
        weights[i] -= d_weight;
    }

    cash.clear();
}

void NeuralNetwork::initNetwork(vector<int> layers) {

    std::default_random_engine generator;
    std::normal_distribution<double> weights_distribution(0, 1);
    std::normal_distribution<double> bias_distribution(0, 1);

    for (int i = 0; i < layers.size() - 1; i++) {
        int inputNeurons = layers[i];
        int outputNeurons = layers[i + 1];

        auto bias = MatrixXd::Zero(outputNeurons, 1)
                .cast<double>()
                .unaryExpr([&bias_distribution, &generator](double x) {
                    return bias_distribution(generator);
                });

        auto weight = MatrixXd::Zero(inputNeurons, outputNeurons)
                .cast<double>()
                .unaryExpr([&weights_distribution, &generator](double x) {
                    return weights_distribution(generator);
                });

        weights[i] = weight;
        biases[i] = bias;
    }
}

MatrixXd NeuralNetwork::sigmoid(MatrixXd z) {
    return z.unaryExpr([](double x) { return 1.0 / (1.0 + std::exp(-x)); });
}

MatrixXd NeuralNetwork::sigmoidPrime(MatrixXd z) {
    return sigmoid(z).cwiseProduct(sigmoid(z).unaryExpr([](double x) { return 1.0 - x; }));
}

MatrixXd NeuralNetwork::reLu(MatrixXd matrix) {
    return (matrix.array() < 0).select(0, matrix);
}

MatrixXd NeuralNetwork::reLuPrime(MatrixXd matrix) {
    return matrix.unaryExpr([](double x) { return x >= 0.0 ? 1.0 : 0.0; });
}

MatrixXd NeuralNetwork::softmax(MatrixXd matrix) {

    auto z_exp = MatrixXd(matrix.unaryExpr([](double x) { return exp(x); }));
    double sum = z_exp.sum();

    return z_exp.unaryExpr([&sum](auto x) { return x / sum; });
}

MatrixXd NeuralNetwork::softmaxPrime(MatrixXd x, MatrixXd y) {
    return x - y;
}

map<string, MatrixXd> &NeuralNetwork::getCash() {
    return cash;
}

string NeuralNetwork::join(string name, int i) {
    return name + to_string(i);
}

int NeuralNetwork::predict(Digit *digit) {
    auto size = (int) weights.size();

    MatrixXd activation = *digit->pixels;

    for (int i = 0; i < size; i++) {
        MatrixXd z = weights[i].transpose() * activation + biases[i];
        activation = sigmoid(z);
    }

    int maxIndex = -1;
    double maxElement = -1;
    for (int i = 0; i < 10; i++) {
        if (activation(i, 0) >= maxElement) {
            maxElement = activation(i, 0);
            maxIndex = i;
        }
    }

    return maxIndex;
}
