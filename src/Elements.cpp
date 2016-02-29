//
// Created by Hugo Sadok on 2/6/16.
//

#include <vector>
#include <iostream>
#include <algorithm>

#include "Elements.h"
#include "helpers.h"
#include "AMCircuitException.h"

namespace amcircuit {

StampParameters::StampParameters(int system_size) : system_size(system_size) {
  A = allocate_matrix(system_size);
  b = allocate_vector(system_size);
  x = allocate_vector(system_size);
  last_nr_trial = allocate_vector(system_size);

  zero_vector(x, system_size);
  zero_vector(last_nr_trial, system_size);

  method_order = 0;
  use_ic = false;
  new_nr_cycle = true;
  time = 0;
}

StampParameters::~StampParameters() {
  free_array(A, 2, system_size);
  free(b);
  free(x);
  free(last_nr_trial);
}

Element::Element(const std::string& params) : line_stream(params) {
  line_stream >> name;
}

inline Element::~Element() { } //  not to be implemented by Element

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
    : Element(name), node_p(node_p), node_n(node_n),
      node_ctrl_p(node_ctrl_p), node_ctrl_n(node_ctrl_n) { }

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

int Resistor::get_num_of_currents() const {
  return 0;
}

void Resistor::place_stamp(const StampParameters& p) {
  p.A[get_node1()][get_node1()] += 1/R;
  p.A[get_node2()][get_node2()] += 1/R;
  p.A[get_node1()][get_node2()] -= 1/R;
  p.A[get_node2()][get_node1()] -= 1/R;
}

NonLinearResistor::NonLinearResistor(const std::string& name, int node1,
                                     int node2,
                                     const std::vector<coordinate>& coordinates)
    : DoubleTerminalElement(name, node1, node2), coordinates(coordinates) {
  std::sort(this->coordinates.begin(), this->coordinates.end());
}

NonLinearResistor::NonLinearResistor(const std::string& params)
    : DoubleTerminalElement(params) {
  while(line_stream) {
    coordinate c;
    if (!(line_stream >> c.first)) {
      break;
    }
    if (!(line_stream >> c.second)) {
      break;
    }
    coordinates.push_back(c);
  }
  std::sort(coordinates.begin(), coordinates.end());
}

const std::vector<NonLinearResistor::coordinate>&
NonLinearResistor::get_coordinates() const {
  return coordinates;
}

int NonLinearResistor::get_num_of_currents() const {
  return 0;
}

void NonLinearResistor::place_stamp(const StampParameters& p) {
  amc_float voltage = p.last_nr_trial[get_node1()]-p.last_nr_trial[get_node2()];
  std::vector<coordinate>::iterator resistance_range =
      std::lower_bound(coordinates.begin() + 1, coordinates.end(),
                       coordinate(voltage, 0));
  if (resistance_range == coordinates.end()) {
    resistance_range = coordinates.end() - 1;
  }
  amc_float j1 = (resistance_range-1)->second;
  amc_float j2 = resistance_range->second;
  amc_float v1 = (resistance_range-1)->first;
  amc_float v2 = resistance_range->first;
  amc_float G = (j2 - j1)/(v2 - v1);
  amc_float I = j2 - G * v2;

  p.A[get_node1()][get_node1()] += G;
  p.A[get_node2()][get_node2()] += G;
  p.A[get_node1()][get_node2()] -= G;
  p.A[get_node2()][get_node1()] -= G;
  p.b[get_node1()] -= I;
  p.b[get_node2()] += I;
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

int VoltageControlledSwitch::get_num_of_currents() const {
  return 0;
}

void VoltageControlledSwitch::place_stamp(const StampParameters& p) {
  amc_float v_ctrl = p.last_nr_trial[get_node_ctrl_p()]
                           - p.last_nr_trial[get_node_ctrl_n()];
  amc_float G = v_ctrl < v_ref ? g_off : g_on;

  p.A[get_node_p()][get_node_p()] += G;
  p.A[get_node_n()][get_node_n()] += G;
  p.A[get_node_p()][get_node_n()] -= G;
  p.A[get_node_n()][get_node_p()] -= G;
}

Inductor::Inductor(const std::string& name, int node1, int node2, amc_float L,
                   amc_float initial_current)
    : DoubleTerminalElement(name, node1, node2), L(L),
      initial_current(initial_current) {
  initialize();
}

Inductor::Inductor(const std::string& params) : DoubleTerminalElement(params),
                                                initial_current(0) {
  std::string ic_string = "";
  line_stream >> L >> ic_string;
  if (ic_string.size() > 0) {
    if (ic_string.substr(0,3) != "IC=") {
      throw BadElementString("Invalid string \"" + params + "\"");
    }
    std::stringstream(ic_string.substr(3,std::string::npos)) >> initial_current;
  }
  initialize();
}

void Inductor::initialize() {
  past_voltages[2] = past_voltages[1] = past_voltages[0] = 0;
}

amc_float Inductor::get_L() const {
  return L;
}

amc_float Inductor::get_initial_current() const {
  return initial_current;
}

int Inductor::get_num_of_currents() const {
  return 1;
}

void Inductor::place_stamp(const StampParameters& p) {
  int method_order = p.method_order;
  if (p.use_ic) {
    method_order = 1;
    last_current = initial_current;
  }
  if(p.new_nr_cycle) {
    past_voltages[2] = past_voltages[1];
    past_voltages[1] = past_voltages[0];
    past_voltages[0] = p.x[get_node1()] - p.x[get_node2()];
    last_current = p.x[p.currents_position];
  }

  // Modelled using a resistor in series with a voltage source
  amc_float R;
  amc_float V;

  switch (method_order) {
    case 1: {
      R = L/p.step_s;
      V = R * last_current;
      break;
    }
    case 2: {
      R = 2 * L/p.step_s;
      V = R * last_current + past_voltages[0];
      break;
    }
    case 3: {
      R = 12.0/5.0 * L/p.step_s;
      V = R*last_current - 1.0/5.0 * past_voltages[1]
          + 8.0/5.0 * past_voltages[0];
      break;
    }
    case 4: {
      R = 8.0/3.0 * L/p.step_s;
      V = R * last_current + 1.0/9.0 * past_voltages[2]
          - 5.0/9.0 * past_voltages[1] + 19.0/9.0 * past_voltages[0];
      break;
    }
    default: {
      throw InvalidIntegrationMethod(
          to_str("Invalid Adams-Moulton order: " << p.method_order));
    }
  }

  p.A[get_node1()][p.currents_position] += 1;
  p.A[get_node2()][p.currents_position] -= 1;
  p.A[p.currents_position][get_node1()] -= 1;
  p.A[p.currents_position][get_node2()] += 1;
  p.A[p.currents_position][p.currents_position] += R;
  p.b[p.currents_position] += V;
}

Capacitor::Capacitor(const std::string& name, int node1, int node2, amc_float C,
                     amc_float initial_voltage)
    : DoubleTerminalElement(name, node1, node2), C(C),
      initial_voltage(initial_voltage) {
  initialize();
}

Capacitor::Capacitor(const std::string& params)
    : DoubleTerminalElement(params), initial_voltage(0) {
  std::string iv_string;
  line_stream >> C >> iv_string;
  if (iv_string.size() > 0) {
    if (iv_string.substr(0,3) != "IC=") {
      throw BadElementString("Invalid string \"" + params + "\"");
    }
    std::stringstream(iv_string.substr(3,std::string::npos)) >> initial_voltage;
  }
  initialize();
}

void Capacitor::initialize() {
  past_currents[2] = past_currents[1] = past_currents[0] = 0;
}

amc_float Capacitor::get_C() const {
  return C;
}

amc_float Capacitor::get_initial_voltage() const {
  return initial_voltage;
}

int Capacitor::get_num_of_currents() const {
  return 0;
}

void Capacitor::place_stamp(const StampParameters& p) {
  int method_order = p.method_order;
  if (p.use_ic) {
    method_order = 1;
    last_voltage = initial_voltage;
  }
  if (p.new_nr_cycle) {
    last_voltage = p.x[get_node1()] - p.x[get_node2()];
    past_currents[2] = past_currents[1];
    past_currents[1] = past_currents[0];
    past_currents[0] = last_G * last_voltage - last_I;
  }

  //  Modeled using a conductance G in parallel with a current source I
  amc_float G;
  amc_float I;

  switch (method_order) {
    case 1: {
      G = C/p.step_s;
      I = G * last_voltage;
      break;
    }
    case 2: {
      G = 2 * C/p.step_s;
      I = G * last_voltage + past_currents[0];
      break;
    }
    case 3: {
      G =  12.0/5.0*C/p.step_s;
      I = G * last_voltage - 1.0/5.0 * past_currents[1]
          + 8.0/5.0 * past_currents[0];
      break;
    }
    case 4: {
      G = 8.0/3.0*C/p.step_s;
      I = G * last_voltage + 1.0/9.0 * past_currents[2]
          - 5.0/9.0 * past_currents[1] + 19.0/9.0 * past_currents[0];
      break;
    }
    default: {
      throw InvalidIntegrationMethod(
          to_str("Invalid Adams-Moulton order: " << p.method_order));
    }
  }

  p.A[get_node1()][get_node1()] += G;
  p.A[get_node2()][get_node2()] += G;
  p.A[get_node1()][get_node2()] -= G;
  p.A[get_node2()][get_node1()] -= G;
  p.b[get_node1()] += I;
  p.b[get_node2()] -= I;

  last_G = G;
  last_I = I;
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

int VoltageControlledVoltageSource::get_num_of_currents() const {
  return 1;
}

void VoltageControlledVoltageSource::place_stamp(const StampParameters& p) {
  p.A[p.currents_position][get_node_p()] -= 1;
  p.A[p.currents_position][get_node_n()] += 1;
  p.A[p.currents_position][get_node_ctrl_p()] += Av;
  p.A[p.currents_position][get_node_ctrl_n()] -= Av;
  p.A[get_node_p()][p.currents_position] += 1;
  p.A[get_node_n()][p.currents_position] -= 1;
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

int CurrentControlledCurrentSource::get_num_of_currents() const {
  return 1;
}

void CurrentControlledCurrentSource::place_stamp(const StampParameters& p) {
  p.A[p.currents_position][get_node_ctrl_p()] -= 1;
  p.A[p.currents_position][get_node_ctrl_n()] += 1;
  p.A[get_node_p()][p.currents_position] += Ai;
  p.A[get_node_n()][p.currents_position] -= Ai;
  p.A[get_node_ctrl_p()][p.currents_position] += 1;
  p.A[get_node_ctrl_n()][p.currents_position] -= 1;
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


int VoltageControlledCurrentSource::get_num_of_currents() const {
  return 0;
}

void VoltageControlledCurrentSource::place_stamp(const StampParameters& p) {
  p.A[get_node_p()][get_node_ctrl_p()] += Gm;
  p.A[get_node_p()][get_node_ctrl_n()] -= Gm;
  p.A[get_node_n()][get_node_ctrl_p()] -= Gm;
  p.A[get_node_n()][get_node_ctrl_n()] += Gm;
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

int CurrentControlledVoltageSource::get_num_of_currents() const {
  return 2;
}

void CurrentControlledVoltageSource::place_stamp(const StampParameters& p) {
  p.A[p.currents_position][get_node_ctrl_p()] -= 1;
  p.A[p.currents_position][get_node_ctrl_n()] += 1;
  p.A[p.currents_position+1][get_node_p()] -= 1;
  p.A[p.currents_position+1][get_node_n()] += 1;
  p.A[p.currents_position+1][p.currents_position] += Rm;
  p.A[get_node_ctrl_p()][p.currents_position] += 1;
  p.A[get_node_ctrl_n()][p.currents_position] -= 1;
  p.A[get_node_p()][p.currents_position+1] += 1;
  p.A[get_node_n()][p.currents_position+1] -= 1;
}

CurrentSource::CurrentSource(const std::string& name, int node_p, int node_n,
                             Signal::Handler signal)
    : ArbitrarySourceElement(name, node_p, node_n, signal) { }

CurrentSource::CurrentSource(const std::string& params)
    : ArbitrarySourceElement(params) { }

int CurrentSource::get_num_of_currents() const {
  return 0;
}

void CurrentSource::place_stamp(const StampParameters& p) {
  p.b[get_node_p()] -= signal->get_value(p.time);
  p.b[get_node_n()] += signal->get_value(p.time);
}

VoltageSource::VoltageSource(const std::string& name, int node_p, int node_n,
                             Signal::Handler signal)
    : ArbitrarySourceElement(name, node_p, node_n, signal) { }

VoltageSource::VoltageSource(const std::string& params)
    : ArbitrarySourceElement(params) {}

int VoltageSource::get_num_of_currents() const {
  return 1;
}

void VoltageSource::place_stamp(const StampParameters& p) {
  p.A[get_node_p()][p.currents_position] += 1;
  p.A[get_node_n()][p.currents_position] -= 1;
  p.A[p.currents_position][get_node_p()] -= 1;
  p.A[p.currents_position][get_node_n()] += 1;
  p.b[p.currents_position] -= signal->get_value(p.time);
}

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

int IdealOpAmp::get_num_of_currents() const {
  return 1;
}

void IdealOpAmp::place_stamp(const StampParameters& p) {
  p.A[p.currents_position][in_p] += 1;
  p.A[p.currents_position][in_n] -= 1;
  p.A[out_p][p.currents_position] += 1;
  p.A[out_n][p.currents_position] -= 1;
}

}  // namespace amcircuit
