//
// Created by Hugo Sadok on 2/6/16.
//

#include <string>
#include <sstream>

#include "Statement.h"
#include "AMCircuitException.h"
#include "helpers.h"

namespace amcircuit {

Statement::Statement() { }

Statement::Statement(const std::string& params) : line_stream(params) {}

inline Statement::~Statement() { }

Statement::Handler Statement::get_statement(std::string params) {
  if (params[0] == '.') {
    params = params.substr(1, std::string::npos);
  }
  std::stringstream params_stream(params);
  std::string type;
  params_stream >> type >> params;
  type = str_upper(type);
  if (type == "TRAN") return Statement::Handler(new Tran(params));
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
  uic = true;
}

amc_float Tran::get_t_stop_s() const {
  return t_stop_s;
}

amc_float Tran::get_t_step_s() const {
  return t_step_s;
}

int Tran::get_admo_order() const {
  return admo_order;
}

int Tran::get_internal_steps() const {
  return internal_steps;
}

bool Tran::get_uic() const {
  return uic;
}

} // namespace amcircuit
