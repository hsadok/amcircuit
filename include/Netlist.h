//
// Created by Hugo Sadok on 2/6/16.
//

#ifndef AMCIRCUIT_NETLIST_H
#define AMCIRCUIT_NETLIST_H

#include <string>
#include <vector>

#include "Elements.h"

namespace amcircuit {

class Netlist {
public:
  Netlist(const std::string& file_name, const unsigned max_lines = 200);
  Netlist(const Netlist& other);
  ~Netlist();
  Netlist& operator=(const Netlist& other);
private:
  const std::string file_name;
  std::string title;
  int current_line;
  std::vector<Element&> elements;

  void processFile();
  void handleLine(const std::string& line);
  Element& getElementFromString(std::string element_string);
};

}

#endif //AMCIRCUIT_NETLIST_H
