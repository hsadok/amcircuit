//
// Created by Hugo Sadok on 2/9/16.
//

#include <algorithm>
#include <string>

#include <stdlib.h>
#include <stdarg.h>

#include "helpers.h"
#include "AMCircuitException.h"

namespace amcircuit {

std::string str_upper(std::string str) {
  for (unsigned long i = 0; i < str.size(); ++i){
    str[i] = (char) toupper(str[i]);
  }
  return str;
}

#ifdef __APPLE__
#include <mach-o/dyld.h>
  std::string get_executable_path() {
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) != 0)
      throw BufferTooSmall(to_str("need size: " << size << std::endl));
    std::string string_path = std::string(path);
    return string_path.substr(0, string_path.find_last_of("\\/"));
  }
#elif __linux__
#include <unistd.h>
#include <limits.h>
  std::string get_executable_path() {
    char buff[PATH_MAX];
    ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff)-1);
    if (len == -1) {
      throw BufferTooSmall(to_str("need size: " << PATH_MAX << std::endl));
    }
    buff[len] = '\0';
    std::string string_path = std::string(buff);
    return string_path.substr(0, string_path.find_last_of("\\/"));
  }
#else  // Assume Windows, gets rid of Windows mess
#include <windows.h>
  std::string get_executable_path() {
    wchar_t buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
    string::size_type pos = string( buffer ).find_last_of( "\\/" );
    return std::string( buffer ).substr( 0, pos);
  }
#endif

void* createArrayUsingDimensionVector(unsigned sizeof_param,
                                      unsigned num_dimension,
                                      unsigned *dimensionVector);

void free_array_using_dimension_vector(void** ptr, unsigned num_dimension,
                                       unsigned *dimension_vector);

void* malloc_array(unsigned sizeof_param, unsigned num_dimension,  ...) {
  va_list ap;
  unsigned i;
  unsigned* dimension_vector;
  void* ret_ptr;

  if (!(dimension_vector = (unsigned*) malloc(num_dimension *
                                                  sizeof(*dimension_vector)))) {
    return NULL;
  }

  va_start(ap, num_dimension);

  for (i = 0; i < num_dimension; i++){
    dimension_vector[i] = va_arg(ap, unsigned);
  }
  va_end(ap);

  ret_ptr =  createArrayUsingDimensionVector(sizeof_param, num_dimension,
                                             dimension_vector);
  free(dimension_vector);
  return ret_ptr;
}

void* createArrayUsingDimensionVector(unsigned sizeof_param,
                                      unsigned num_dimension,
                                      unsigned *dimension_vector) {
  unsigned i;
  void **matrix;

  if (num_dimension <= 1){
    return (calloc(dimension_vector[0], sizeof_param));
  }

  matrix = (void**) calloc(dimension_vector[0], sizeof(void*));
  if (matrix == NULL){
    return NULL;
  }
  for(i = 0; i < dimension_vector[0]; i++){
    if ((matrix[i] = createArrayUsingDimensionVector(
        sizeof_param, num_dimension - 1, dimension_vector + 1)) == NULL) {
      return NULL;
    }
  }
  return (void*) matrix;
}

void free_array(void* ptr, unsigned num_dimension, ...) {
  va_list ap;
  unsigned i;
  unsigned* dimension_vector;

  if (!(dimension_vector = (unsigned*) malloc((num_dimension - 1) *
                                                  sizeof(*dimension_vector)))) {
    return;
  }

  va_start(ap, num_dimension);

  for (i = 0; i < num_dimension-1; i++){
    dimension_vector[i] = va_arg(ap, unsigned);
  }
  va_end(ap);

  free_array_using_dimension_vector((void**) ptr, num_dimension,
                                    dimension_vector);
  free(dimension_vector);
}

void free_array_using_dimension_vector(void** ptr, unsigned num_dimension,
                                       unsigned *dimension_vector) {
  unsigned i;

  if (num_dimension <= 1) {
    if (num_dimension == 1) {
      free(ptr);
    }
    return;
  }

  for(i = 0; i < dimension_vector[0]; i++){
    free_array_using_dimension_vector((void**) ptr[i], num_dimension-1,
                                      dimension_vector+1);
  }
  free(ptr);
}

void print_matrix(amc_float **matrix, unsigned const size1, unsigned size2)
{
  unsigned i, j;

  if (size2 == 0) { size2 = size1; }

  for (i = 0; i < size1; ++i){
    printf("| ");
    for (j = 0; j < size2; ++j){
      printf("%18.15f ", matrix[i][j]);
    }
    printf("|\r\n");
  }
  printf("\r\n");
}

void print_vector(amc_float *vector, unsigned const size)
{
  unsigned i;

  for (i = 0; i < size; ++i){
    printf("| %18.15f |\r\n", vector[i]);
  }
  printf("\r\n");
}

} //  namespace amcircuit
