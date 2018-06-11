#include <random>
#include <utility>
#include <iostream>
#include "../headers/solver.h"

Solver::Solver(vector<Digit *> digits, NeutralNetwork &network) : network(network) {
    this->data = std::move(digits);
}

void Solver::train() {

    int numberEpoch = 1000;
    int batchSize = 100;

    for (int i = 0; i < numberEpoch; i++) {
        for (auto digit : getMiniBatch(batchSize)) {
            auto pass = network.forwardPass(digit);
            network.backwardPass(pass, digit);
            network.updateGradient();
        }
    }

    predict(getMiniBatch(1000)[5]);
    predict(getMiniBatch(1000)[123]);
    predict(getMiniBatch(1000)[666]);
    predict(getMiniBatch(1000)[444]);
    predict(getMiniBatch(1000)[888]);
}

int Solver::predict(Digit *digit) {
    auto score = network.forwardPass(digit);
    auto value = score(digit->truth, 0);

    cout << "SCORE for [ " << digit->truth << " ] is -> " << value << endl;

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
