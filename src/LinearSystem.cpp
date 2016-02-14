//
// Created by Hugo Sadok on 2/11/16.
//

#include "AMCircuit.h"
#include "LinearSystem.h"

namespace amcircuit {
// Use U for the original matrix as well as the output U from the decomposition
void lu_decomposition(amc_float **L, amc_float **U, const int size,
                      const int first_index) {
  int i, j, k;

  for (i = first_index; i < size; ++i){
    L[i][i] = 1;
    for(k = i+1; k < size; ++k){
      L[k][i] = U[k][i]/U[i][i];
      for (j = first_index; j < size; ++j){
        U[k][j] = U[k][j] - L[k][i]*U[i][j];
      }
    }
    for(k = first_index; k < i; ++k){
      L[k][i] = 0;
    }
  }
}

// C is not an input, it must be a vector allocated with the right size
void solve_lu(amc_float **L, amc_float **U, amc_float *X, amc_float *B,
             amc_float* C, const int size, const int first_index) {
  int i, j;

  //Lc = b
  for (i = first_index; i < size; ++i){
    double aux = 0;
    for(j = first_index; j < i; ++j){
      aux += L[i][j]*C[j];
    }
    C[i] = (B[i]-aux)/L[i][i];
  }

  //Ux = c
  for (i = size - 1; i >= first_index; --i){
    double aux = 0;
    for (j = i+1; j < size; ++j){
      aux += U[i][j]*X[j];
    }
    X[i] = (C[i]-aux)/U[i][i];
  }
}

}  // namespace amcircuit
