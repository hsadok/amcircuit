//
// Created by Hugo Sadok on 2/6/16.
//

#include <iostream>
#include <string>

#include "Netlist.h"
#include "CircuitSolver.h"
#include "helpers.h"

using namespace amcircuit;

void show_usage(std::string program_name) {
  std::cout << "usage: " << program_name << " <netlist_file> [output_file]"
            << std::endl;
}

int main(int argc, char const *argv[]) {
  if (argc < 2 || argc > 3) {
    show_usage(argv[0]);
    return 1;
  }

  const std::string netlist_file_name = argv[1];
  std::string output_file_name;
  if (argc == 2) {
    output_file_name = netlist_file_name + ".out";
    std::cout << "2!" << std::endl;
  } else {
    output_file_name = argv[2];
  }

  Netlist nl = Netlist(netlist_file_name);
  CircuitSolver cs(&nl);
  cs.write_to_file(output_file_name);

  return 0;
}
