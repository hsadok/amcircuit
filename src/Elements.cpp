//
// Created by Hugo Sadok on 2/6/16.
//

#include <vector>
#include <ctype.h>

#include "Elements.h"
#include "AMCircuitException.h"

namespace amcircuit {

Element::Element(const std::string& params) : line_stream(params) {
  line_stream >> name;
}

inline Element::~Element() { } // not to be implemented

Element::Element(const Element& other) { }

Element& Element::operator=(const Element& other) { }

Element* Element::get_element(const std::string& element_string) {
  switch (toupper(element_string[0])) {
    case 'R': return new Resistor(element_string);
    case 'N': return new NonLinearResistor(element_string);
    case '$': return new VoltageControlledSwitch(element_string);
    case 'L': return new Inductor(element_string);
    case 'C': return new Capacitor(element_string);
    case 'E': return new VoltageControlledVoltageSource(element_string);
    case 'F': return new CurrentControlledCurrentSource(element_string);
    case 'G': return new VoltageControlledCurrentSource(element_string);
    case 'H': return new CurrentControlledVoltageSource(element_string);
    case 'I': return new CurrentSource(element_string);
    case 'V': return new VoltageSource(element_string);
    case 'O': return new IdealOpAmp(element_string);
    default:break;
  }
  throw BadElementString("Invalid string \"" + element_string + "\"");
}

DoubleTerminalElement::DoubleTerminalElement(const std::string& name, int node1,
                                             int node2)
    : Element(name), node1(node1), node2(node2) { }

DoubleTerminalElement::DoubleTerminalElement(const std::string& params)
    : Element(params) {
  line_stream >> node1 >> node2;
}

SimpleSourceElement::SimpleSourceElement(const std::string& name, int node_p,
                                         int node_n, Signal* signal)
    : Element(name), node_p(node_p), node_n(node_n), signal(signal) { }

SimpleSourceElement::SimpleSourceElement(const std::string& params)
    : Element(params), signal(Signal::get_signal(line_stream.str())) { }

ControlledElement::ControlledElement(const std::string& name, int node_p,
                                     int node_n, int node_ctrl_p,
                                     int node_ctrl_n)
    : Element(name), node_p(node_p), node_n(node_n), node_ctrl_p(node_ctrl_p),
      node_ctrl_n(node_ctrl_n) { }

ControlledElement::ControlledElement(const std::string& params)
    : Element(params) {
  line_stream >> node_p >> node_n >> node_ctrl_p >> node_ctrl_n;
}

Resistor::Resistor(const std::string& name, int node1, int node2, amc_float R)
    : DoubleTerminalElement(name, node1, node2), R(R) { }

Resistor::Resistor(const std::string& params) : DoubleTerminalElement(params) {
  line_stream >> R;
}

NonLinearResistor::NonLinearResistor(const std::string& name, int node1,
                                     int node2, const std::vector<amc_float>& R)
    : DoubleTerminalElement(name, node1, node2), R(R) { }

NonLinearResistor::NonLinearResistor(const std::string& params)
    : DoubleTerminalElement(params) {
  line_stream >> R[0] >> R[1] >> R[2] >> R[3] >> R[4] >> R[5] >> R[6] >> R[7];
}

VoltageControlledSwitch::VoltageControlledSwitch(
    const std::string& name, int node_p, int node_n, int node_ctrl_p,
    int node_ctrl_n, amc_float g_on, amc_float g_off, amc_float v_ref)
    : ControlledElement(name, node_p, node_n, node_ctrl_p, node_ctrl_n),
      g_on(g_on), g_off(g_off), v_ref(v_ref) { }

VoltageControlledSwitch::VoltageControlledSwitch(const std::string& params)
    : ControlledElement(params) {
  line_stream >> g_on >> g_off >> v_ref;
}

Inductor::Inductor(const std::string& name, int node1, int node2, amc_float L,
                   amc_float initial_current)
    : DoubleTerminalElement(name, node1, node2), L(L),
      initial_current(initial_current) { }

Inductor::Inductor(const std::string& params) : DoubleTerminalElement(params) {
  line_stream >> L >> initial_current; // TODO initial_current is optional
}

Capacitor::Capacitor(const std::string& name, int node1, int node2, amc_float C,
                     amc_float initial_voltage)
    : DoubleTerminalElement(name, node1, node2), C(C),
      initial_voltage(initial_voltage) { }

Capacitor::Capacitor(const std::string& params)
    : DoubleTerminalElement(params) {
  line_stream >> C >> initial_voltage; // TODO initial_voltage is optional
}

VoltageControlledVoltageSource::VoltageControlledVoltageSource(
    const std::string& name, int node_p, int node_n, int node_ctrl_p,
    int node_ctrl_n, amc_float Av)
    : ControlledElement(name, node_p, node_n, node_ctrl_p, node_ctrl_n),
      Av(Av) { }

VoltageControlledVoltageSource::VoltageControlledVoltageSource(
    const std::string& params) : ControlledElement(params) {
  line_stream >> Av;
}

CurrentControlledCurrentSource::CurrentControlledCurrentSource(
    const std::string& name, int node_p, int node_n, int node_ctrl_p,
    int node_ctrl_n, amc_float Ai)
    : ControlledElement(name, node_p, node_n, node_ctrl_p, node_ctrl_n),
      Ai(Ai) { }

CurrentControlledCurrentSource::CurrentControlledCurrentSource(
    const std::string& params) : ControlledElement(params) {
  line_stream >> Ai;
}

VoltageControlledCurrentSource::VoltageControlledCurrentSource(
    const std::string& name, int node_p, int node_n, int node_ctrl_p,
    int node_ctrl_n, amc_float Gm)
    : ControlledElement(name, node_p, node_n, node_ctrl_p, node_ctrl_n),
      Gm(Gm) { }

VoltageControlledCurrentSource::VoltageControlledCurrentSource(
    const std::string& params) : ControlledElement(params) {
  line_stream >> Gm;
}

CurrentControlledVoltageSource::CurrentControlledVoltageSource(
    const std::string& name, int node_p, int node_n, int node_ctrl_p,
    int node_ctrl_n, amc_float Rm)
    : ControlledElement(name, node_p, node_n, node_ctrl_p, node_ctrl_n),
      Rm(Rm) { }

CurrentControlledVoltageSource::CurrentControlledVoltageSource(
    const std::string& params) : ControlledElement(params) {
  line_stream >> Rm;
}

CurrentSource::CurrentSource(const std::string& name, int node_p, int node_n,
                             Signal* signal)
    : SimpleSourceElement(name, node_p, node_n, signal) { }

CurrentSource::CurrentSource(const std::string& params)
    : SimpleSourceElement(params) { }

VoltageSource::VoltageSource(const std::string& name, int node_p, int node_n,
                             Signal* signal)
    : SimpleSourceElement(name, node_p, node_n, signal) { }

VoltageSource::VoltageSource(const std::string& params)
    : SimpleSourceElement(params) { }

IdealOpAmp::IdealOpAmp(const std::string& name, int out_p, int out_n, int in_p,
                       int in_n)
    : Element(name), out_p(out_p), out_n(out_n), in_p(in_p), in_n(in_n){ }

IdealOpAmp::IdealOpAmp(const std::string& params) :  Element(params) {
  line_stream >> out_p >> out_n >> in_p >> in_n;
}

} // namespace amcircuit
