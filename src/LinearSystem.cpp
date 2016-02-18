//
// Created by Hugo Sadok on 2/11/16.
//
#include <cmath>


#include "AMCircuit.h"
#include "LinearSystem.h"
#include "AMCircuitException.h"

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
    C[i] = (B[i]-aux);
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

// Solve linear system using Gauss-Jordan with pivotal compensation
// adapted from Moreirao (ACMQ http://www.coe.ufrj.br/~acmq/)
// A is the entire system, it includes A and b the output x will be on b
void solve_system(amc_float** A, amc_float* b_x, const int size) {
  int i, j, l, a;
  double t, p;

  for (i=1; i < size; i++) {
    t=0.0;
    a=i;
    for (l=i; l < size; l++) {
      if (std::abs(A[l][i])>std::abs(t)) {
        a=l;
        t=A[l][i];
      }
    }
    if (i!=a) {
      for (l = 1; l < size; l++) {
        p=A[i][l];
        A[i][l]=A[a][l];
        A[a][l]=p;
      }
      p=b_x[i];
      b_x[i]=b_x[a];
      b_x[a]=p;
    }
    if (std::abs(t) < 1e-9) {
      throw SingularSystem("System is singular, no solution.");
    }

    b_x[i] /= t;
    p=b_x[i];
    if (p!=0) {
      for (l=1; l < size; l++) {
        if (l!=i)
          b_x[l] -= A[l][i]*p;
      }
    }

    for (j = size-1; j >= 1; j--) {  /* Basta j>i em vez de j>0 */
      A[i][j] /= t;
      p=A[i][j];
      if (p!=0) {
        for (l = 1; l < size; l++) {
          if (l != i)
            A[l][j] -= A[l][i] * p;
        }
      }
    }
  }
  b_x[0] = 0; // ground voltage should be 0
}

}  // namespace amcircuit
