#include "../headers/solver.h"
#include <random>
#include <utility>
#include <iostream>

Solver::Solver(vector<Digit *> &digits, NeutralNetwork &network) : network(network), data(digits) {
}

void Solver::train() {

    int numberEpoch = 100;
    int batchSize = 100;

    map<string, MatrixXd> miniBatchCash;

    for (int i = 0; i < numberEpoch; i++) {
        for (auto digit : getMiniBatch(batchSize)) {

            network.forwardPass(digit);
            network.backwardPass(digit);
            network.updateGradient(batchSize);

            auto cash = network.getCash();
            mergeDeltas(miniBatchCash, cash);
        }

        //cout << "EPOCH " << i + 1 << endl;
    }

    int correct = 0;
    int number = 60000;
    for (int i = 50000; i < number; i++) {
        correct += network.predict(data[i]) == data[i]->truth;
    }

    cout << "CORRECT = " << correct << " / 10000" << endl;
}

vector<Digit *> Solver::getMiniBatch(int size) {

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distribution(0, static_cast<int>(this->data.size() - 10001));

    vector<Digit *> output;

    for (int n = 0; n < size; ++n) {
        int rand = distribution(gen);
        output.push_back(data[rand]);
    }
    return output;
}

void Solver::mergeDeltas(map<string, MatrixXd> &miniBatchCash, map<string, MatrixXd> &cash) {

    for (auto &it : cash) {
        MatrixXd matrix = it.second;
        auto finder = miniBatchCash.find(it.first);
        if (finder == miniBatchCash.end()) {
            miniBatchCash[it.first] = MatrixXd(matrix);
        } else {
            miniBatchCash[it.first] = miniBatchCash[it.first] + matrix;
        };
    }
}