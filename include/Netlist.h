//
// Created by Hugo Sadok on 2/6/16.
//

#ifndef AMCIRCUIT_NETLIST_H
#define AMCIRCUIT_NETLIST_H

#include <string>
#include <vector>
#include <memory>

#include "Elements.h"
#include "Statement.h"

namespace amcircuit {

class Netlist {
 public:
  explicit Netlist(const std::string& file_name);
//  Netlist(const Netlist& other);
//  ~Netlist();
//  Netlist& operator=(const Netlist& other);
  const std::vector<Element::Handler>& get_elements() const;
  const std::vector<Statement::Handler>& get_statements() const;

 private:
  const std::string file_name;
  std::string title;
  int current_line;
  std::vector<Element::Handler> elements;
  std::vector<Statement::Handler> statements;

  void process_file();
  void handle_line(const std::string& line);
};

}  // namespace amcircuit

#endif  // AMCIRCUIT_NETLIST_H
