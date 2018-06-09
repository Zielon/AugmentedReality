#include <random>
#include "../headers/solver.h"

Solver::Solver(vector<Digit *> digits, NeutralNetwork &network) : network(network) {
    this->data = digits;
}

void Solver::train() {
    for (auto digit : getMiniBatch(1000)) {
        auto pass = network.forwardPass(digit);
        network.backwardPass(pass, digit);
    }

    auto weights = network.getWeights();
    auto biases = network.getBiases();
}

int Solver::predict(Digit digit) {
    return 0;
}

vector<Digit *> Solver::getMiniBatch(int size) {

    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(0, static_cast<int>(this->data.size() - 1));

    vector<Digit *> output;

    for (int n = 0; n < size; ++n)
        output.push_back(data[n]);

    return output;
}