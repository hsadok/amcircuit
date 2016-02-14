//
// Created by Hugo Sadok on 2/13/16.
//

#include <vector>
#include <cstring>

#include "CircuitSolver.h"
#include "Elements.h"
#include "Statement.h"
#include "Netlist.h"
#include "AMCircuitException.h"
#include "LinearSystem.h"
#include "helpers.h"

namespace amcircuit {


CircuitSolver::CircuitSolver(Netlist* netlist)
    : netlist(*netlist), config(find_first_tran_statement()) {
  prepare_circuit();
  solve_circuit();
}

CircuitSolver::~CircuitSolver() {
  free_array(matrix.A, 2, matrix.size);
  free_array(matrix.L, 2, matrix.size);
  free_array(solutions, 2, num_solution_samples);
  free(matrix.b);
  free(matrix.x);
  free(matrix.c);
}

inline amc_float* allocate_vector(const int size) {
  amc_float* vector;
  if (!(vector = (amc_float*) malloc(sizeof(*vector) * size))) {
    throw std::bad_alloc();
  }
  return vector;
}

inline amc_float** allocate_matrix(const int size1, const int size2) {
  amc_float** matrix;
  if (!(matrix = (amc_float**) malloc_array(sizeof(*matrix), 2, size1, size2))){
    throw std::bad_alloc();
  }
  return matrix;
}

inline amc_float** allocate_matrix(const int size) {
  return allocate_matrix(size, size);
}

inline void zero_vector(amc_float* const matrix, const int size) {
  for (int i = 0; i < size; ++i) {
    matrix[i]= 0;
  }
}

inline void zero_matrix(amc_float** const matrix, const int size) {
  for (int i = 0; i < size; ++i) {
    for(int j = 0; j < size; ++j) {
      matrix[i][j] = 0;
    }
  }
}

void CircuitSolver::solve_circuit() {
  prepare_circuit();
  int solution_number = 0;
  amc_float inner_step_s = config.get_t_step_s() / config.get_internal_steps();
  for (amc_float t = 0; t <= config.get_t_stop_s(); t+= config.get_t_step_s()) {
    for (int i = 0; i < config.get_internal_steps(); ++i) {
      update_circuit(t + i*inner_step_s);
      lu_decomposition(matrix.A, matrix.L, matrix.size);
      solve_lu(matrix.L, matrix.A, matrix.x, matrix.b, matrix.c, matrix.size);
    }
    solutions[solution_number][0] = t;
    std::memcpy(solutions[solution_number] + 1, matrix.x + 1, matrix.size - 1);
    ++solution_number;
  }
}

Tran CircuitSolver::find_first_tran_statement() const {
  std::vector<Statement::Handler>& statements = netlist.get_statements();
  std::vector< std::vector<Statement::Handler> >::iterator it;
  Tran* tran;
  for (it = statements.begin(); it != statements.end(); ++it) {
    if ((tran = dynamic_cast<Tran*>(*it)) != NULL) {
      return *tran;
    }
  }
  throw IncompleteNetList("No analysis statement found on netlist");
}

void CircuitSolver::prepare_circuit() {
  num_extra_lines = 0;
  use_ic = config.get_uic();
  const std::vector<Element::Handler>& elements = netlist.get_elements();
  for(unsigned i = 0; i != elements.size(); ++i) {
    num_extra_lines += elements[i]->get_num_of_currents();
  }
  matrix.size = 1 + netlist.get_number_of_nodes() + num_extra_lines;
  num_solution_samples =
      static_cast<int>(config.get_t_stop_s()/config.get_t_step_s()) + 1;

  matrix.A = allocate_matrix(matrix.size);
  matrix.L = allocate_matrix(matrix.size);
  matrix.b = allocate_vector(matrix.size);
  matrix.x = allocate_vector(matrix.size);
  matrix.c = allocate_vector(matrix.size);

  solutions = allocate_matrix(num_solution_samples, matrix.size);

  zero_vector(matrix.x, matrix.size);
}

void CircuitSolver::update_circuit(amc_float time) {
  zero_matrix(matrix.A, matrix.size);
  zero_vector(matrix.b, matrix.size);

  const std::vector<Element::Handler>& elements = netlist.get_elements();
  int next_line = matrix.size - num_extra_lines;

  for (unsigned i = 0; i != elements.size(); ++i) {
    elements[i]->place_stamp(time, matrix.A, matrix.x, matrix.b,
                             config.get_admo_order(), next_line, use_ic);
    next_line += elements[i]->get_num_of_currents();
  }
  use_ic = false;
}

}  // namespace amcircuit
