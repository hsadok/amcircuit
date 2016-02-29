//
// Created by Hugo Sadok on 2/9/16.
//

#ifndef AMCIRCUIT_HELPERS_H
#define AMCIRCUIT_HELPERS_H

#include <string>
#include <sstream>

#include "AMCircuit.h"

namespace amcircuit {

std::string str_upper(std::string);

std::string get_executable_path();

#define to_str( x ) static_cast< std::ostringstream & >( \
  ( std::ostringstream() << std::dec << x ) ).str()

// C style function to allocate arbitrary dimension arrays
// It allocates arrays in a way that can be passed to functions easily
// Example: Allocate a 3 dimension array (6x5x4) of doubles
// double*** A = malloc_array(sizeof(double), 3, 6, 5, 4)
void* malloc_array(unsigned sizeof_param, unsigned num_dimension, ...);

// C style function to free arrays allocated with malloc_array
// You need to specify all the dimensions but the last in order to free the
// array properly (If you specify the last it will be ignored anyway).
// Example: Allocate and free the 3 dimension array (6x5x4)
// double*** A = malloc_array(sizeof(double), 3, 6, 5, 4)
// free_array(A, 3, 6, 5);
void free_array(void* ptr, unsigned num_dimension, ...);

amc_float** allocate_matrix(const int size1, const int size2);

amc_float** allocate_matrix(const int size);

amc_float* allocate_vector(const int size);

void zero_vector(amc_float* const matrix, const int size);

void zero_matrix(amc_float** const matrix, const int size);

void print_matrix(amc_float **matrix, unsigned const size1, unsigned size2 = 0);
void print_vector(amc_float *vector, unsigned const size);


}  // namespace amcircuit

#endif //AMCIRCUIT_HELPERS_H
