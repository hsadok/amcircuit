//
// Created by Hugo Sadok on 2/13/16.
//

#ifndef AMCIRCUIT_CIRCUITSOLVER_H
#define AMCIRCUIT_CIRCUITSOLVER_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "Netlist.h"
#include "Statement.h"

namespace amcircuit {

class CircuitSolver {
 public:
  explicit CircuitSolver(Netlist* netlist);
  ~CircuitSolver();

  void write_to_stream(std::ostream& ostream) const;
  void write_to_file(const std::string& file_name) const;
  void write_to_screen() const;

 private:
  Tran& find_first_tran_statement();
  void assembly_circuit();
  int get_num_extra_lines();
  int get_system_size();
  void prepare_circuit();
  void calculate_till_converge(const amc_float initial_time,
                               const amc_float time_step, const int steps);
  void solve_circuit();
  void update_circuit(amc_float time);
  inline void add_solution(int index, amc_float time);
  std::string get_variables_header() const;
  CircuitSolver(const CircuitSolver& other);
  CircuitSolver& operator=(const CircuitSolver& other);

  Netlist& netlist;
  Tran& config;
  int num_extra_lines;
  int system_size;
  StampParameters stamp_params;
  int num_solution_samples;
  amc_float** solutions;
};

}  // namespace amcircuit

#endif //AMCIRCUIT_CIRCUITSOLVER_H
