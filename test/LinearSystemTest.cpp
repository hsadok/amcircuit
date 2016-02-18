//
// Created by Hugo Sadok on 2/14/16.
//


#include "catch.hpp"

#include "LinearSystem.h"
#include "helpers.h"

using namespace amcircuit;

// Getting rid of unused-value warning from GCC and clang
// It's a useful warning but doesn't make sense for test
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"

SCENARIO("A linear system should be decomposed and resolved",
         "[linear_system]") {
  GIVEN("A linear system") {
    unsigned system_size = 9;
    amc_float** A;
    amc_float** L;
    amc_float* b;
    amc_float* c;
    amc_float* x;
    b = (amc_float*) malloc(sizeof(*b) * system_size);
    c = (amc_float*) malloc(sizeof(*c) * system_size);
    x = (amc_float*) malloc(sizeof(*x) * system_size);

    A = (amc_float**) malloc_array(sizeof(**A), 2, system_size, system_size+1);
    L = (amc_float**) malloc_array(sizeof(**L), 2, system_size, system_size);

    A[0][0]=-4; A[0][1]= 1; A[0][2]= 0; A[0][3]= 1; A[0][4]= 0; A[0][5]= 0; A[0][6]= 0; A[0][7]= 0; A[0][8]= 0;
    A[1][0]= 1; A[1][1]=-4; A[1][2]= 1; A[1][3]= 0; A[1][4]= 1; A[1][5]= 0; A[1][6]= 0; A[1][7]= 0; A[1][8]= 0;
    A[2][0]= 0; A[2][1]= 1; A[2][2]=-4; A[2][3]= 0; A[2][4]= 0; A[2][5]= 1; A[2][6]= 0; A[2][7]= 0; A[2][8]= 0;
    A[3][0]= 1; A[3][1]= 0; A[3][2]= 0; A[3][3]=-4; A[3][4]= 1; A[3][5]= 0; A[3][6]= 1; A[3][7]= 0; A[3][8]= 0;
    A[4][0]= 0; A[4][1]= 1; A[4][2]= 0; A[4][3]= 1; A[4][4]=-4; A[4][5]= 1; A[4][6]= 0; A[4][7]= 1; A[4][8]= 0;
    A[5][0]= 0; A[5][1]= 0; A[5][2]= 1; A[5][3]= 0; A[5][4]= 1; A[5][5]=-4; A[5][6]= 0; A[5][7]= 0; A[5][8]= 1;
    A[6][0]= 0; A[6][1]= 0; A[6][2]= 0; A[6][3]= 1; A[6][4]= 0; A[6][5]= 0; A[6][6]=-4; A[6][7]= 1; A[6][8]= 0;
    A[7][0]= 0; A[7][1]= 0; A[7][2]= 0; A[7][3]= 0; A[7][4]= 1; A[7][5]= 0; A[7][6]= 1; A[7][7]=-4; A[7][8]= 1;
    A[8][0]= 0; A[8][1]= 0; A[8][2]= 0; A[8][3]= 0; A[8][4]= 0; A[8][5]= 1; A[8][6]= 0; A[8][7]= 1; A[8][8]=-4;

    b[0]=0;
    b[1]=0;
    b[2]=0;
    b[3]=0;
    b[4]=0;
    b[5]=0;
    b[6]=-1;
    b[7]=-1;
    b[8]=-1;

    WHEN("solving by LU decomposition") {
      lu_decomposition(L, A, system_size);
      solve_lu(L, A, x, b, c, system_size);

      CHECK( x[0] == Approx( 0.07142 ) );
      CHECK( x[1] == Approx( 0.09821 ) );
      CHECK( x[2] == Approx( 0.07142 ) );
      CHECK( x[3] == Approx( 0.18750 ) );
      CHECK( x[4] == Approx( 0.25000 ) );
      CHECK( x[5] == Approx( 0.18750 ) );
      CHECK( x[6] == Approx( 0.42857 ) );
      CHECK( x[7] == Approx( 0.52678 ) );
      CHECK( x[8] == Approx( 0.42857 ) );
    }

    free_array(A, 2, system_size);
    free_array(L, 2, system_size);
    free(b);
    free(c);
    free(x);
  }
  GIVEN("a matrix which is singular without ignoring the first row and column"){
    unsigned system_size = 4;
    amc_float** A;
    amc_float** L;
    amc_float* b;
    amc_float* c;
    amc_float* x;
    b = (amc_float*) malloc(sizeof(*b) * system_size);
    c = (amc_float*) malloc(sizeof(*c) * system_size);
    x = (amc_float*) malloc(sizeof(*x) * system_size);

    A = (amc_float**) malloc_array(sizeof(**A), 2, system_size, system_size);
    L = (amc_float**) malloc_array(sizeof(**L), 2, system_size, system_size);

    A[0][0]=0.001; A[0][1]= 0; A[0][2]= -0.001; A[0][3]= -1;
    A[1][0]= 0; A[1][1]=0.001; A[1][2]= -0.001; A[1][3]= 1;
    A[2][0]= -0.001; A[2][1]= -0.001; A[2][2]=0.002; A[2][3]= 0;
    A[3][0]= 1; A[3][1]= -1; A[3][2]= 0; A[3][3]=0;

    b[0]=0;
    b[1]=0;
    b[2]=0;
    b[3]=-10;

    WHEN("solving by LU decomposition") {
      lu_decomposition(L, A, system_size, 1);
      solve_lu(L, A, x, b, c, system_size, 1);
      CHECK( x[1] == Approx( 10.00000 ) );
      CHECK( x[2] == Approx( 5.00000 ) );
      CHECK( x[3] == Approx( -0.00500 ) );
    }

    WHEN("solving by Moreirao") {
      solve_system(A, b, system_size);
      CHECK( b[1] == Approx( 10.00000 ) );
      CHECK( b[2] == Approx( 5.00000 ) );
      CHECK( b[3] == Approx( -0.00500 ) );
    }

    free_array(A, 2, system_size);
    free_array(L, 2, system_size);
    free(b);
    free(c);
    free(x);
  }
}
#pragma GCC diagnostic pop
