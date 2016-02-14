//
// Created by Hugo Sadok on 2/13/16.
//

#ifndef AMCIRCUIT_CIRCUITSOLVER_H
#define AMCIRCUIT_CIRCUITSOLVER_H

#include <vector>

#include "Netlist.h"
#include "Statement.h"

namespace amcircuit {

class CircuitSolver {
 public:
  explicit CircuitSolver(Netlist* netlist);
  ~CircuitSolver();

 private:
  Tran find_first_tran_statement() const;
  void assembly_circuit();
  void prepare_circuit();
  void solve_circuit();
  void update_circuit(amc_float time);
  CircuitSolver(const CircuitSolver& other);
  CircuitSolver& operator=(const CircuitSolver& other);

  Netlist& netlist;
  Tran config;
  bool use_ic;
  int num_extra_lines;
  int num_solution_samples;
  struct {
    amc_float** A;
    amc_float* b;
    amc_float** L;
    amc_float* x;
    amc_float* c;
    int size;
  } matrix;
  amc_float** solutions;
};

}  // namespace amcircuit

#endif //AMCIRCUIT_CIRCUITSOLVER_H
