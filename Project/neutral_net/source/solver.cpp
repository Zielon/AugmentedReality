#include <random>
#include <utility>
#include "../headers/solver.h"

Solver::Solver(vector<Digit *> digits, NeutralNetwork &network) : network(network) {
    this->data = std::move(digits);
}

void Solver::train() {

    int numberEpoch = 1;
    int batchSize = 1;

    for (int i = 0; i < numberEpoch; i++) {
        for (auto digit : getMiniBatch(batchSize)) {
            network.backwardPass(network.forwardPass(digit), digit);
            gradientUpdate(network.getCash());
        }
    }
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

void Solver::gradientUpdate(map<string, MatrixXd> cash) {

}
