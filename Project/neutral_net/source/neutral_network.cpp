#include "../headers/neutral_network.h"

#include <random>
#include <iostream>
#include <iterator>

using namespace std;

NeutralNetwork::NeutralNetwork(vector<int> layers) {
    weights = vector<MatrixXd *>(layers.size() - 1);
    biases = vector<MatrixXd *>(layers.size() - 1);
    initNetwork(layers);
}

void NeutralNetwork::forwardPass(Digit *digit) {

    MatrixXd activation = *digit->pixels;

    for (int i = 0; i < weights.size() - 1; i++) {
        MatrixXd *weight = weights[i];
        MatrixXd *bias = biases[i];
        MatrixXd z = (activation.transpose() * (*weight)).transpose() + *bias;

        //cout << activation.rows() << " -> " << weight->cols() << endl;

        activation = reLu(z);
        cash[RELU + to_string(i)] = activation;
        cash[ZS + to_string(i)] = z;
    }
}

void NeutralNetwork::backwardPass() {

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
    x = softmax(x);

    return Eigen::MatrixXd();
}

MatrixXd NeutralNetwork::softmax(MatrixXd matrix) {

    auto z_exp = MatrixXd(matrix.unaryExpr([](double x) { return exp(x); }));
    auto sum = z_exp.rowwise().sum();

    for (int i = 0; i < z_exp.rows(); i++) {
        double s = sum(i, 0);
        for (int j = 0; j < z_exp.cols(); j++) {
            double e = z_exp(i, j);
            double a = e / s;
            z_exp(i, j) = a;
        }
    }

    return z_exp;
}
