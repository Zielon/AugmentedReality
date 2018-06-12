#include "../headers/solver.h"
#include <random>
#include <utility>
#include <iostream>

Solver::Solver(vector<Digit *> digits, NeutralNetwork &network) : network(network) {
    this->data = std::move(digits);
}

void Solver::train() {

    int numberEpoch = 3;
    int batchSize = 50;

    map<string, MatrixXd> miniBatchCash;

    for (int i = 0; i < numberEpoch; i++) {
        for (auto digit : getMiniBatch(batchSize)) {
            network.forwardPass(digit);
            network.backwardPass(digit);

            auto cash = network.getCash();
            mergeDeltas(miniBatchCash, cash);
        }

        network.updateGradient(batchSize, miniBatchCash);

        cout << "EPOCH " << i + 1 << endl;
    }

    int correct = 0;
    int number = 10000;
    for (int i = 0; i < number; i++) {
        correct += predict(data[i]);
    }

    cout << "CORRECT = " << correct  << " / " << number << endl;
}

int Solver::predict(Digit *digit) {
    auto score = network.forwardPass(digit);

    int maxIndex = -1;
    double maxElement = 0;
    for(int i = 0; i < 10; i++){
        if(score(i, 0) > maxElement){
            maxElement = score(i, 0);
            maxIndex = i;
        }
    }

    return maxIndex == digit->truth;
}

vector<Digit *> Solver::getMiniBatch(int size) {

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distribution(0, static_cast<int>(this->data.size() - 1));

    vector<Digit *> output;

    for (int n = 0; n < size; ++n)
        output.push_back(data[distribution(gen)]);

    return output;
}

void Solver::mergeDeltas(map<string, MatrixXd> &miniBatchCash, map<string, MatrixXd> &cash) {

    for (auto &it : cash) {
        MatrixXd matrix = it.second;
        auto finder = miniBatchCash.find(it.first);
        if(finder == miniBatchCash.end()) {
            miniBatchCash[it.first] = matrix;
        }else{
            miniBatchCash[it.first] += matrix;
        };
    }
}