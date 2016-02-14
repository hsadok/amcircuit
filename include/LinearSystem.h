//
// Created by Hugo Sadok on 2/11/16.
//

#ifndef AMCIRCUIT_LINEARSYSTEM_H
#define AMCIRCUIT_LINEARSYSTEM_H
namespace amcircuit {

void lu_decomposition(double **matrix, const int size, double **L, double **U);

void solveLU(double **L, double **U, double *X, double *B, double* C,
             const int size);

}  // namespace amcircuit

#endif //AMCIRCUIT_LINEARSYSTEM_H
