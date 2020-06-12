//
// Created by grizzly on 4/21/19.
//

#ifndef UNITARYREPR_GATE_GENERATOR_H
#define UNITARYREPR_GATE_GENERATOR_H

#include <vector>
#include <complex>
#include <Eigen/Dense>

std::vector<Eigen::Matrix2cd> getSeeds();

std::vector<std::vector<Eigen::Matrix2cd>*> *generateCompositeGates(int n, std::vector<Eigen::Matrix2cd> *seeds) ;

typedef struct {
    double theta;
    Eigen::Vector3d n;
} unitaryPolarRepr;

//unitaryPolarRepr polarRepresentation(const Eigen::Matrix2cd &u);

std::tuple<double, double, double> vectorize(const Eigen::Matrix2cd &u);

#endif //UNITARYREPR_GATE_GENERATOR_H
