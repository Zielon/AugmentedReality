#include <random>
#include <utility>
#include <iostream>
#include "../headers/solver.h"

Solver::Solver(vector<Digit *> digits, NeutralNetwork &network) : network(network) {
    this->data = std::move(digits);
}

void Solver::train() {

    int numberEpoch = 100;
    int batchSize = 50;

    for (int i = 0; i < numberEpoch; i++) {
        for (auto digit : getMiniBatch(batchSize)) {
            network.forwardPass(digit);
            network.backwardPass(digit);
            network.updateGradient(batchSize);
        }
    }

    for(int i = 0; i < 25; i++)
        predict(getMiniBatch(100)[i]);
}

int Solver::predict(Digit *digit) {
    auto score = network.forwardPass(digit);
    double value = score(digit->truth, 0);

    string correct = score.maxCoeff() == value ? "TRUE " : "FALSE";

    cout << "SCORE for [ " << digit->truth << " ] is -> " << value
         << " | correct: " <<  correct <<  " | " << score.maxCoeff() << endl;

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
