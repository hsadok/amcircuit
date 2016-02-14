//
// Created by Hugo Sadok on 2/11/16.
//

#ifndef AMCIRCUIT_LINEARSYSTEM_H
#define AMCIRCUIT_LINEARSYSTEM_H

#include "AMCircuit.h"

namespace amcircuit {

// Use U for the original matrix as well as the output U from the decomposition
void lu_decomposition(amc_float **L, amc_float **U, const int size);

// C is not an input, it must be a vector allocated with the right size
void solve_lu(amc_float **L, amc_float **U, amc_float *x, amc_float *b,
             amc_float* c, const int size);

}  // namespace amcircuit

#endif //AMCIRCUIT_LINEARSYSTEM_H
