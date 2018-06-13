#include <string>

using namespace std;

class Consts {
public:
    static const string ACTIVATION;
    static const string Z;
    static const string GRADIENT_WEIGHT;
    static const string GRADIENT_BIAS;
};

const string Consts::ACTIVATION = "ACTIVATION_";
const string Consts::Z = "Z_";
const string Consts::GRADIENT_WEIGHT = "WEIGHT_";
const string Consts::GRADIENT_BIAS = "BIAS_";