//
// Created by Hugo Sadok on 2/6/16.
//

#ifndef AMCIRCUIT_STATEMENT_H
#define AMCIRCUIT_STATEMENT_H

#include <string>
#include <sstream>

#include "AMCircuit.h"
#include "ResourceHandler.h"

namespace amcircuit {

class Statement {
 public:
  Statement();
  Statement(const std::string& params);
  virtual ~Statement() = 0;

  typedef ResourceHandler<Statement> Handler;
  static Statement::Handler get_statement(std::string params);

 protected:
  std::stringstream line_stream;

 private:
  Statement(const Statement& other);
  Statement& operator=(const Statement& other);
};


// Statement only works with the `TRAN` keyword used for transient analysis
// It assumes the Adams-Moulton method for integration
// It also assumes that UIC (Use initial conditions) is always present
// Example input:
// .TRAN 40 8E-2 ADMO4 1 UIC
class Tran : public Statement {
 public:
  explicit Tran(amc_float t_stop_s, amc_float t_step_s, int admo_order,
                int internal_steps);
  explicit Tran(const std::string& params);

 protected:
  amc_float t_stop_s;
  amc_float t_step_s;
  int admo_order;
  int internal_steps;
};

} // namespace amcircuit

#endif //AMCIRCUIT_STATEMENT_H
