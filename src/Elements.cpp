//
// Created by Hugo Sadok on 2/6/16.
//

#include <vector>

#include "Elements.h"
#include "AMCircuitException.h"

namespace amcircuit {

inline Element::~Element() { }

Resistor::Resistor(const std::string &name, int node1, int node2, amc_float R) :
    DoubleTerminalElement(name, node1, node2), R(R) { }

NonLinearResistor::NonLinearResistor(const std::string &name, int node1,
                                     int node2, std::vector<amc_float> R) :
    DoubleTerminalElement(name, node1, node2), R(R) { }

VoltageControlledSwitch::VoltageControlledSwitch(const std::string &name,
                                                 int node_p, int node_n,
                                                 int node_ctrl_p,
                                                 int node_ctrl_n,
                                                 amc_float g_on,
                                                 amc_float g_off,
                                                 amc_float v_ref) :
    ControlledElement(name, node_p, node_n, ) { }

} // namespace amcircuit
