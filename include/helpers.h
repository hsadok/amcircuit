//
// Created by Hugo Sadok on 2/9/16.
//

#ifndef AMCIRCUIT_HELPERS_H
#define AMCIRCUIT_HELPERS_H

#include <string>
#include <sstream>

namespace amcircuit {

std::string str_upper(std::string);

std::string get_executable_path();

#define to_str( x ) static_cast< std::ostringstream & >( \
  ( std::ostringstream() << std::dec << x ) ).str()

}  // namespace amcircuit

// C style function to allocate arbitrary dimension arrays
// It allocates arrays in a way that can be passed to functions easily
// Example: Allocate a 3 dimension array (6x5x4) of doubles
// double*** A = malloc_array(sizeof(double), 3, 6, 5, 4)
void* malloc_array(unsigned sizeof_param, unsigned num_dimension, ...);

// C style function to free arrays allocated with malloc_array
// You need to specify all the dimensions but the last in order to free the
// array properly (If you specify the last it will be ignored anyway).
// Example: Free the 3 dimension array (6x5x4)
// double*** A = malloc_array(sizeof(double), 3, 6, 5, 4)
// free_array(A, 3, 6, 5);
void free_array(void* ptr, unsigned num_dimension, ...);

#endif //AMCIRCUIT_HELPERS_H
