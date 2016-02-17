//
// Created by Hugo Sadok on 2/13/16.
//

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <CircuitSolver.h>

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
  free_array(stamp_parameters.A, 2, system_size);
  free_array(solutions, 2, num_solution_samples);
  free(stamp_parameters.b);
  free(stamp_parameters.x);
}

void CircuitSolver::write_to_stream(std::ostream& ostream) const {
  ostream << get_variables_header() << std::endl;
  for (int sample = 0; sample < num_solution_samples; ++sample) {
    ostream << solutions[sample][0];
    for (int i = 1; i < system_size; ++i) {
      ostream << " " << solutions[sample][i];
    }
    ostream << std::endl;
  }
}

void CircuitSolver::write_to_file(const std::string& file_name) const {
  std::ofstream file;
  file.open(file_name.c_str());
  write_to_stream(file);
  file.close();
}

void CircuitSolver::write_to_screen() const {
  write_to_stream(std::cout);
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
  if (!(matrix = (amc_float**) malloc_array(sizeof(**matrix), 2, size1, size2))){
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
  amc_float t = 0;
  amc_float inner_step_s = config.get_t_step_s() / config.get_internal_steps();
  for (int i = 0; i < num_solution_samples; ++i) {
    amc_float inner_time = t;
    for (int j = 0; j < config.get_internal_steps(); ++j) {
      update_circuit(inner_time);
      solve_system(stamp_parameters.A, stamp_parameters.b, system_size);
      amc_float* aux = stamp_parameters.x;
      stamp_parameters.x = stamp_parameters.b;
      stamp_parameters.b = aux;
      inner_time += inner_step_s;
    }
    solutions[i][0] = t;
    memcpy(solutions[i] + 1, stamp_parameters.x + 1,
           (system_size - 1) * sizeof(amc_float));
    t += config.get_t_step_s();
  }
}

Tran& CircuitSolver::find_first_tran_statement() {
  std::vector<Statement::Handler>& statements = netlist.get_statements();
  std::vector<Statement::Handler>::iterator it;
  Tran* tran;
  for (it = statements.begin(); it != statements.end(); ++it) {
    if ((tran = dynamic_cast<Tran*>(&(**it))) != NULL) {
      return *tran;
    }
  }
  throw IncompleteNetList("No analysis statement found on netlist");
}

void CircuitSolver::prepare_circuit() {
  num_extra_lines = 0;
  const std::vector<Element::Handler>& elements = netlist.get_elements();
  for(unsigned i = 0; i != elements.size(); ++i) {
    num_extra_lines += elements[i]->get_num_of_currents();
  }
  system_size = 1 + netlist.get_number_of_nodes() + num_extra_lines;
  num_solution_samples =
      static_cast<int>(config.get_t_stop_s()/config.get_t_step_s()) + 1;

  stamp_parameters.A = allocate_matrix(system_size);
  stamp_parameters.b = allocate_vector(system_size);
  stamp_parameters.x = allocate_vector(system_size);
  solutions = allocate_matrix(num_solution_samples, system_size);

  zero_vector(stamp_parameters.x, system_size);

  stamp_parameters.method_order = config.get_admo_order();
  stamp_parameters.step_s = config.get_t_step_s()/config.get_internal_steps();
  stamp_parameters.use_ic = config.get_uic();
}

void CircuitSolver::update_circuit(amc_float time) {
  zero_matrix(stamp_parameters.A, system_size);
  zero_vector(stamp_parameters.b, system_size);

  std::vector<Element::Handler>& elements = netlist.get_elements();
  int next_line = system_size - num_extra_lines;

  stamp_parameters.time = time;

  for (unsigned i = 0; i != elements.size(); ++i) {
    int num_of_currents = elements[i]->get_num_of_currents();
    if (num_of_currents > 0) {
      stamp_parameters.currents_position = next_line;
      next_line += num_of_currents;
    } else {
      stamp_parameters.currents_position = -1;
    }
    elements[i]->place_stamp(stamp_parameters);
  }
  stamp_parameters.use_ic = false;
}

std::string CircuitSolver::get_variables_header() const {
  std::stringstream header;
  int num_nodes = system_size - num_extra_lines;

  header << "t";
  for (int i = 1; i < num_nodes; ++i) { header << " " << i; }

  std::vector<Element::Handler>& elements = netlist.get_elements();

  for (unsigned i = 0; i != elements.size(); ++i) {
    Element::Handler element = elements[i];
    int currents = element->get_num_of_currents();
    if (currents >= 1) {
      header << " j" << (currents > 1 ? "1" : "") << element->get_name();
    }
    for (int j = 2; j <= currents; ++j) {
      header << " j" << j << element->get_name();
    }
  }

  return header.str();
}

}  // namespace amcircuit
