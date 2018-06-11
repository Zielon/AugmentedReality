#include <string>

using namespace std;

class Consts {
public:
    static const string RELU;
    static const string ZS;
    static const string GRADIENT_WEIGHT;
    static const string GRADIENT_BIAS;
};

const string Consts::RELU = "RELU_";
const string Consts::ZS = "ZS_";
const string Consts::GRADIENT_WEIGHT = "WEIGHT_";
const string Consts::GRADIENT_BIAS = "BIAS_";