//
// Created by Hugo Sadok on 2/13/16.
//

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
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
    : netlist(*netlist), config(find_first_tran_statement()),
      num_extra_lines(get_num_extra_lines()), system_size(get_system_size()),
      stamp_params(system_size) {
  srand (static_cast<unsigned>(time(0)));
  prepare_circuit();
  solve_circuit();
}

CircuitSolver::~CircuitSolver() {
  free_array(solutions, 2, num_solution_samples);
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

inline void CircuitSolver::add_solution(int index, amc_float time){
  solutions[index][0] = time;
  memcpy(solutions[index] + 1, stamp_params.x + 1,
         (system_size - 1) * sizeof(amc_float));
}

inline bool converged(amc_float* last_solution, amc_float* new_solution,
                      const int system_size) {
  for (int i = 0; i < system_size; ++i) {
    if (std::abs(last_solution[i] - new_solution[i]) > ACCEPTABLE_NR_ERROR) {
      return false;
    }
  }
  return true;
}

inline void retry_initial(amc_float* last_solution, amc_float* new_solution,
                          const int system_size) {
  for (int i = 0; i < system_size; ++i) {
    if (std::abs(last_solution[i] - new_solution[i]) > ACCEPTABLE_NR_ERROR) {
      new_solution[i] = - MAX_NR_GUESS + static_cast <amc_float> (rand()) /
                           ( static_cast <amc_float> (RAND_MAX/(2.0*RAND_MAX)));
    }
  }
}

inline void swap_vectors(amc_float*& a, amc_float*& b) {
  amc_float* aux = a;
  a = b;
  b = aux;
}

inline void CircuitSolver::calculate_till_converge(const amc_float initial_time,
                                                   const amc_float time_step,
                                                   const int steps) {
  amc_float t = initial_time;
  for (int i = 0; i < steps; ++i) {
    int iterations = 0;
    int ia_retries = 0;
    stamp_params.new_nr_cycle = true;
    while (1) {
      update_circuit(t);
      solve_system(stamp_params.A, stamp_params.b, system_size);

      if (converged(stamp_params.last_nr_trial, stamp_params.b, system_size)) {
        break;
      }

      ++iterations;
      if (iterations > NEWTON_RAPHSON_CYCLE_LIMIT) {
        iterations = 0;
        ++ia_retries;
        retry_initial(stamp_params.last_nr_trial, stamp_params.b, system_size);
        if (ia_retries > NEWTON_RAPHSON_IA_RETRIES) {
          throw NewtonRaphsonFailed(to_str(
                "Newton-Raphson failed to converge after "
                << NEWTON_RAPHSON_IA_RETRIES << " initialization retries."));
        }
      }
      swap_vectors(stamp_params.last_nr_trial, stamp_params.b);
      stamp_params.new_nr_cycle = false;
    }
    stamp_params.use_ic = false;
    swap_vectors(stamp_params.x, stamp_params.b);
    t += time_step;
  }
}

void CircuitSolver::solve_circuit() {
  amc_float t = 0;
  amc_float inner_step_s = config.get_t_step_s() / config.get_internal_steps();
  for (int i = 0; i < num_solution_samples; ++i) {
    calculate_till_converge(t, inner_step_s, config.get_internal_steps());
    add_solution(i, t);
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

int CircuitSolver::get_num_extra_lines() {
  int num_extra_lines = 0;
  const std::vector<Element::Handler>& elements = netlist.get_elements();
  for(unsigned i = 0; i != elements.size(); ++i) {
    num_extra_lines += elements[i]->get_num_of_currents();
  }
  return num_extra_lines;
}

int CircuitSolver::get_system_size() {
  return 1 + netlist.get_number_of_nodes() + num_extra_lines;
}

void CircuitSolver::prepare_circuit() {
  num_solution_samples =
      static_cast<int>(config.get_t_stop_s()/config.get_t_step_s()) + 1;

  solutions = allocate_matrix(num_solution_samples, system_size);

  stamp_params.method_order = config.get_admo_order();
  stamp_params.step_s = config.get_t_step_s()/config.get_internal_steps();
  stamp_params.use_ic = config.get_uic();
}

void CircuitSolver::update_circuit(amc_float time) {
  zero_matrix(stamp_params.A, system_size);
  zero_vector(stamp_params.b, system_size);

  std::vector<Element::Handler>& elements = netlist.get_elements();
  int next_line = system_size - num_extra_lines;

  stamp_params.time = time;

  for (unsigned i = 0; i != elements.size(); ++i) {
    int num_of_currents = elements[i]->get_num_of_currents();
    if (num_of_currents > 0) {
      stamp_params.currents_position = next_line;
      next_line += num_of_currents;
    } else {
      stamp_params.currents_position = -1;
    }
    elements[i]->place_stamp(stamp_params);
  }
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
