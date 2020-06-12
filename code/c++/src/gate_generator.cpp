#include "gate_generator.h"
#include <vector>
#include <Eigen/Dense>
#include <iostream>

std::vector<Eigen::Matrix2cd> getSeeds() {
    std::vector<Eigen::Matrix2cd> seeds;
    Eigen::Matrix2cd h;
    h << 1.0, 1.0, 1.0, -1.0;
    seeds.emplace_back(std::complex<double>(0, 1 / sqrt(2)) * h);

    Eigen::Matrix2cd t;
    t << exp(std::complex<double>(0, -M_PI / 8)), 0, 0, exp(std::complex<double>(0, M_PI / 8));
    seeds.push_back(t);
    return seeds;
}

std::vector<std::vector<Eigen::Matrix2cd> *> *generateCompositeGates(int n, std::vector<Eigen::Matrix2cd> *seeds) {
    if (n == 0) {
        return new std::vector<std::vector<Eigen::Matrix2cd> *>{seeds};
    }

    auto vec = generateCompositeGates(n - 1, seeds);

    std::cout << "generating " << n << "-composite gates" << std::endl;
    auto gen = new std::vector<Eigen::Matrix2cd>;
    for (const auto &x: *vec->back()) {
        for (const auto &y: *seeds) {
            gen->emplace_back(x * y);
        }
    }

    vec->push_back(gen);
    std::cout << gen->size() << " gates generated" << std::endl;
    return vec;
}

#define EPSILON 0.001

//unitaryPolarRepr polarRepresentation(const Eigen::Matrix2cd &u) {
//    Eigen::Vector3d n((u(0, 1) + u(1, 0)).imag() / 2.0,
//                      (u(0, 1) - u(1, 0)).real() / 2.0,
//                      (u(1, 1) - u(0, 0)).imag() / 2.0);
//
//    double cosTheta = (u(0, 0) + u(1, 1)).real() / 2.0;
//    double sinTheta = n.norm();
//
//    double theta = atan2(sinTheta, cosTheta);
//
//    return (unitaryPolarRepr){
//        theta,
//        n.normalize()
//    };
//}

std::tuple<double, double, double> vectorize(const Eigen::Matrix2cd &u) {
//    assert(fabs((u(0, 0) + u(1, 1)).imag()) < EPSILON); // special
//
//    assert(fabs((u(0, 1) + u(1, 0)).real()) < EPSILON);
//    assert(fabs((u(0, 1) - u(1, 0)).imag()) < EPSILON);
//    assert(fabs((u(1, 1) - u(0, 0)).real()) < EPSILON);
//

    Eigen::Vector3d n((u(0, 1) + u(1, 0)).imag() / 2.0,
                      (u(0, 1) - u(1, 0)).real() / 2.0,
                      (u(1, 1) - u(0, 0)).imag() / 2.0);

    double cosTheta = (u(0, 0) + u(1, 1)).real() / 2.0;
    double sinTheta = n.norm();

    double theta = atan2(sinTheta, cosTheta);
    n.normalize();


    return {
            n(0) * theta,
            n(1) * theta,
            n(2) * theta
    };
}