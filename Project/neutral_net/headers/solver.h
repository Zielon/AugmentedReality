#ifndef PROJECT_SOLVER_H
#define PROJECT_SOLVER_H

#include "../../data_models/digit.cpp"
#include "neutral_network.h"

class Solver {
public:
    Solver(vector<Digit *> digits, NeutralNetwork &network);

    void train();

    int predict(Digit *digit);

private:
    vector<Digit *> data;

    vector<Digit *> getMiniBatch(int size);

    void gradientUpdate(map<string, MatrixXd> cash);

    NeutralNetwork &network;
};

#endif //PROJECT_SOLVER_H
