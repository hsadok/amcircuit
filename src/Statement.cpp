//
// Created by Hugo Sadok on 2/6/16.
//

#include <string>

#include "Statement.h"
#include "AMCircuitException.h"
#include "helpers.h"

namespace amcircuit {

Statement::Statement() { }

Statement::Statement(const std::string& params) : line_stream(params) {}

inline Statement::~Statement() { }

Statement::Handler get_statement(std::string params) {
  if (params[0] == '.') {
    params = params.substr(1, std::string::npos);
  }
  params = params; // TODO str_upper
  if (params == "TRAN") return Statement::Handler(new Tran(params));
  throw BadElementString("Invalid string \"" + params + "\"");
}

Tran::Tran(amc_float t_stop_s, amc_float t_step_s, int admo_order,
           int internal_steps) : t_stop_s(t_stop_s), t_step_s(t_step_s),
                                 admo_order(admo_order),
                                 internal_steps(internal_steps) { }

Tran::Tran(const std::string& params) : Statement(params) {
  std::string admo_string;
  line_stream >> t_stop_s >> t_step_s >> admo_string >> internal_steps;
  admo_order = *admo_string.rbegin() - '0';
  // line_stream >> std::ws; // ignore whitespace, may be needed...
}

} // namespace amcircuit
