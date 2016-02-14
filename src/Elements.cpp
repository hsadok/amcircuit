//
// Created by Hugo Sadok on 2/6/16.
//

#include <vector>
#include <iostream>

#include "Elements.h"
#include "AMCircuitException.h"

namespace amcircuit {

Element::Element(const std::string& params) : line_stream(params),
                                              num_of_currents(0) {
  line_stream >> name;
}

inline Element::~Element() { } // not to be implemented by Element

// TODO There are ways of making this better, none of which I'm wishing to
//      implement right now. (use a map that can be modified from outside and
//      even allowing subclasses to include themselves)
Element::Handler Element::get_element(const std::string& element_string) {
  switch (::toupper(element_string[0])) {
    case 'R': return Handler(new Resistor(element_string));
    case 'N': return Handler(new NonLinearResistor(element_string));
    case '$': return Handler(new VoltageControlledSwitch(element_string));
    case 'L': return Handler(new Inductor(element_string));
    case 'C': return Handler(new Capacitor(element_string));
    case 'E': return Handler(new VoltageControlledVoltageSource(element_string));
    case 'F': return Handler(new CurrentControlledCurrentSource(element_string));
    case 'G': return Handler(new VoltageControlledCurrentSource(element_string));
    case 'H': return Handler(new CurrentControlledVoltageSource(element_string));
    case 'I': return Handler(new CurrentSource(element_string));
    case 'V': return Handler(new VoltageSource(element_string));
    case 'O': return Handler(new IdealOpAmp(element_string));
    default:break;
  }
  throw BadElementString("Invalid string \"" + element_string + "\"");
}

std::string Element::get_name() const {
  return name;
}

int Element::get_num_of_currents() const {
  return num_of_currents;
}

void flush_values() { }

DoubleTerminalElement::DoubleTerminalElement(const std::string& name, int node1,
                                             int node2)
    : Element(name), node1(node1), node2(node2) { }

DoubleTerminalElement::DoubleTerminalElement(const std::string& params)
    : Element(params) {
  line_stream >> node1 >> node2;
}

int DoubleTerminalElement::get_node1() const {
  return node1;
}

int DoubleTerminalElement::get_node2() const {
  return node2;
}

SimpleSourceElement::SimpleSourceElement(const std::string& name, int node_p,
                                         int node_n)
    : Element(name), node_p(node_p), node_n(node_n) { }

SimpleSourceElement::SimpleSourceElement(const std::string& params)
    : Element(params) {
  line_stream >> node_p >> node_n;
}

int SimpleSourceElement::get_node_p() const {
  return node_p;
}

int SimpleSourceElement::get_node_n() const {
  return node_n;
}

ArbitrarySourceElement::ArbitrarySourceElement(const std::string& name,
                                               int node_p, int node_n,
                                               Signal::Handler signal)
    : SimpleSourceElement(name, node_p, node_n), signal(signal) { }

ArbitrarySourceElement::ArbitrarySourceElement(const std::string& params)
    : SimpleSourceElement(params), signal(Signal::get_signal(line_stream)) { }

const Signal::Handler& ArbitrarySourceElement::get_signal() const {
  return signal;
}

ControlledElement::ControlledElement(const std::string& name, int node_p,
                                     int node_n, int node_ctrl_p,
                                     int node_ctrl_n)
    : Element(name), node_p(node_p), node_n(node_n), node_ctrl_p(node_ctrl_p),
      node_ctrl_n(node_ctrl_n) { }

ControlledElement::ControlledElement(const std::string& params)
    : Element(params) {
  line_stream >> node_p >> node_n >> node_ctrl_p >> node_ctrl_n;
}

int ControlledElement::get_node_p() const {
  return node_p;
}

int ControlledElement::get_node_n() const {
  return node_n;
}

int ControlledElement::get_node_ctrl_p() const {
  return node_ctrl_p;
}

int ControlledElement::get_node_ctrl_n() const {
  return node_ctrl_n;
}

Resistor::Resistor(const std::string& name, int node1, int node2, amc_float R)
    : DoubleTerminalElement(name, node1, node2), R(R) { }

Resistor::Resistor(const std::string& params) : DoubleTerminalElement(params) {
  line_stream >> R;
}

amc_float Resistor::get_R() const {
  return R;
}

NonLinearResistor::NonLinearResistor(const std::string& name, int node1,
                                     int node2, const std::vector<amc_float>& R)
    : DoubleTerminalElement(name, node1, node2), R(R) { }

NonLinearResistor::NonLinearResistor(const std::string& params)
    : DoubleTerminalElement(params), R(8) {
  line_stream >> R[0] >> R[1] >> R[2] >> R[3] >> R[4] >> R[5] >> R[6] >> R[7];
}

const std::vector<amc_float>& NonLinearResistor::get_R() const {
  return R;
}

VoltageControlledSwitch::VoltageControlledSwitch(
    const std::string& name, int node_p, int node_n, int node_ctrl_p,
    int node_ctrl_n, amc_float g_on, amc_float g_off, amc_float v_ref)
    : ControlledElement(name, node_p, node_n, node_ctrl_p, node_ctrl_n),
      g_on(g_on), g_off(g_off), v_ref(v_ref) { }

VoltageControlledSwitch::VoltageControlledSwitch(const std::string& params)
    : ControlledElement(params), g_on(0.0), g_off(0.0), v_ref(0.0) {
  line_stream >> g_on >> g_off >> v_ref;
}

amc_float VoltageControlledSwitch::get_g_on() const {
  return g_on;
}

amc_float VoltageControlledSwitch::get_g_off() const {
  return g_off;
}

amc_float VoltageControlledSwitch::get_v_ref() const {
  return v_ref;
}

Inductor::Inductor(const std::string& name, int node1, int node2, amc_float L,
                   amc_float initial_current)
    : DoubleTerminalElement(name, node1, node2), L(L),
      initial_current(initial_current) { }

Inductor::Inductor(const std::string& params) : DoubleTerminalElement(params) {
  line_stream >> L >> initial_current; // TODO initial_current is optional
}

amc_float Inductor::get_L() const {
  return L;
}

amc_float Inductor::get_initial_current() const {
  return initial_current;
}

Capacitor::Capacitor(const std::string& name, int node1, int node2, amc_float C,
                     amc_float initial_voltage)
    : DoubleTerminalElement(name, node1, node2), C(C),
      initial_voltage(initial_voltage) { }

Capacitor::Capacitor(const std::string& params)
    : DoubleTerminalElement(params) {
  line_stream >> C >> initial_voltage; // TODO initial_voltage is optional
}

amc_float Capacitor::get_C() const {
  return C;
}

amc_float Capacitor::get_initial_voltage() const {
  return initial_voltage;
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

amc_float VoltageControlledVoltageSource::get_Av() const {
  return Av;
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

amc_float CurrentControlledCurrentSource::get_Ai() const {
  return Ai;
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

amc_float VoltageControlledCurrentSource::get_Gm() const {
  return Gm;
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

amc_float CurrentControlledVoltageSource::get_Rm() const {
  return Rm;
}

CurrentSource::CurrentSource(const std::string& name, int node_p, int node_n,
                             Signal::Handler signal)
    : ArbitrarySourceElement(name, node_p, node_n, signal) { }

CurrentSource::CurrentSource(const std::string& params)
    : ArbitrarySourceElement(params) { }

VoltageSource::VoltageSource(const std::string& name, int node_p, int node_n,
                             Signal::Handler signal)
    : ArbitrarySourceElement(name, node_p, node_n, signal) { }

VoltageSource::VoltageSource(const std::string& params)
    : ArbitrarySourceElement(params) { }

IdealOpAmp::IdealOpAmp(const std::string& name, int out_p, int out_n, int in_p,
                       int in_n)
    : Element(name), out_p(out_p), out_n(out_n), in_p(in_p), in_n(in_n) { }

IdealOpAmp::IdealOpAmp(const std::string& params) :  Element(params) {
  line_stream >> out_p >> out_n >> in_p >> in_n;
}

int IdealOpAmp::get_out_p() const {
  return out_p;
}

int IdealOpAmp::get_out_n() const {
  return out_n;
}

int IdealOpAmp::get_in_p() const {
  return in_p;
}

int IdealOpAmp::get_in_n() const {
  return in_n;
}

}  // namespace amcircuit
