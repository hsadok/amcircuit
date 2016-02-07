//
// Created by Hugo Sadok on 2/6/16.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include <ctype.h>

#include "Netlist.h"
#include "AMCircuitException.h"

namespace amcircuit {

Netlist::Netlist(const std::string &file_name, const unsigned max_lines) :
    file_name(file_name), current_line(1) {
  processFile();
}

Netlist::Netlist(const Netlist &other) {

}

Netlist::~Netlist() {

}

Netlist &Netlist::operator=(const Netlist &other) {

}

void Netlist::processFile() {
  std::ifstream data(file_name);
  if (!data) throw FileNotFound("Cannot open file \"" + file_name + "\"");
  std::string line;
  title = std::getline(data, line);
  while (std::getline(data, line)) {
    handleLine(line);
  }
}

void Netlist::handleLine(const std::string& line) {
  // The first character defines if the line describes an element, a comment or
  // the simulation parameters
  switch (line[0]) {
    case '.': { // simulation parameters

      break;
    }
    case '*': { // comment, do nothing
      break;
    }
    default: {
      try {
        elements.push_back(getElementFromString(line));
      } catch (BadFileException e) {
        throw BadFileException("Line: " + std::to_string(current_line) + "  " +
                                   e.what());
      }
    }
  }
  current_line++;
}

Element& getElementFromString(std::string element_string) {
  std::stringstream line_stream(element_string);
  std::string element_name;
  Element* element;
  if (std::getline(line_stream, element_name, ' ')) {
    switch (toupper(element_name[0])) {
      case 'R': { // resistor
        int node1, node2;
        amc_float R;
        line_stream >> node1 >> node2 >> R;
        element = new Resistor(element_name, node1, node2, R);
        return *element;
      }
      case 'N': { // non-linear resistor
        int node1, node2;
        std::vector<amc_float> R(8);
        line_stream >> node1 >> node2 >> R[0] >> R[1] >> R[2] >> R[3] >> R[4] >>
            R[5] >> R[6] >> R[7];
        element = new NonLinearResistor(element_name, node1, node2, R);
        return *element;
      }
      case '$': { // voltage controlled switch
        int node1, node2;
        std::vector<amc_float> R(8);
        line_stream >> node1 >> node2 >> R[0] >> R[1] >> R[2] >> R[3] >> R[4] >>
        R[5] >> R[6] >> R[7];
        element = new VoltageControlledSwitch(element_name, node1, node2, R);
        return *element;
      }
      case 'L': { // inductor
        break;
      }
      case 'C': { // capacitor
        break;
      }
      case 'E': { // voltage controlled voltage source
        break;
      }
      case 'F': { // current controlled current source
        break;
      }
      case 'G': { // voltage controlled current source
        break;
      }
      case 'H': { // current controlled voltage source
        break;
      }
      case 'I': { // current source
        break;
      }
      case 'V': { // voltage source
        break;
      }
      case 'O': { // ideal OpAmp
        break;
      }
      default: {
      }
    }
  }
  throw BadFileException("Invalid line \"" + element_string + "\"");
}

} // namespace amcircuit
