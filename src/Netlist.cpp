//
// Created by Hugo Sadok on 2/6/16.
//

#include <fstream>
#include <iostream>

#include "Netlist.h"
#include "AMCircuitException.h"
#include "helpers.h"

namespace amcircuit {

Netlist::Netlist(const std::string& file_name) :
    file_name(file_name), current_line(1) {
  process_file();
}

//Netlist::Netlist(const Netlist& other) {
//
//}
//
//Netlist::~Netlist() {
//
//}
//
//Netlist& Netlist::operator=(const Netlist& other) {
//
//}

const std::vector<Element::Handler>& Netlist::get_elements() const {
  return elements;
}

const std::vector<Statement::Handler>& Netlist::get_statements() const {
  return statements;
}

void Netlist::process_file() {
  std::ifstream data(file_name.c_str());
  if (!data) throw FileNotFound("Cannot open file \"" + file_name + "\"");
  std::string line;
  std::getline(data, title);
  while (std::getline(data, line)) {
    handle_line(line);
  }
}

void Netlist::handle_line(const std::string& line) {
  // The first character defines if the line describes an element, a comment or
  // the simulation parameters
  try {
    switch (line[0]) {
      case '.': { // simulation parameters
        statements.push_back(Statement::get_statement(line));
        break;
      }
      case '*': { // comment, do nothing
        break;
      }
      default: { // regular elements
        elements.push_back(Element::get_element(line));
      }
    }
  } catch (BadElementString e) {
    throw BadFileException(to_str("Line: " << current_line << " \"" << line
                                  << "\""));
    current_line++;
  }
}

} // namespace amcircuit
