#include "gate_generator.hpp"


Eigen::Matrix2cd randomUnitary(normGen *double()) {
    a = normGen(), b = normGen(), c = normGen(), d = normGen();

    Eigen::Matrix2cd t;
    t << std::complex(a, b) << std::complex(-c, d) << std::complex(c, d) << std::complex(a, -b);
    return t
}

#define DEPTH 19
#define SAMPLES 10

int main() {
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0, 1);

    auto seeds = getSeeds();
    const std::vector<std::vector<Eigen::Matrix2cd> *> &gates = *generateCompositeGates(DEPTH, &seeds);

    double vals[DEPTH] = 0;

    for (int i = 0; i < SAMPLES; i ++) {

    }

}