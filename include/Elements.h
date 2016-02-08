//
// Created by Hugo Sadok on 2/6/16.
//

#ifndef AMCIRCUIT_ELEMENT_H
#define AMCIRCUIT_ELEMENT_H

#include <string>
#include <sstream>

#include "AMCircuit.h"
#include "Signal.h"

namespace amcircuit {

class Element {
 public:
  Element(const std::string& params);
  virtual ~Element() = 0;

  Element(const Element& other);
  Element& operator=(const Element& other);

  static Element* get_element(const std::string& element_string);
 protected:
  std::string name;
  std::stringstream line_stream;
};

class DoubleTerminalElement : public Element {
 public:
  DoubleTerminalElement(const std::string& name, int node1, int node2);
  DoubleTerminalElement(const std::string& params);
 protected:
  int node1;
  int node2;
};

class SimpleSourceElement : public Element {
 public:
  SimpleSourceElement(const std::string& name, int node_p, int node_n,
                      Signal* signal);
  SimpleSourceElement(const std::string& params);
 protected:
  int node_p;
  int node_n;
  Signal* signal;
};

class ControlledElement : public Element {
 public:
  ControlledElement(const std::string& name, int node_p, int node_n,
                    int node_ctrl_p, int node_ctrl_n);
  ControlledElement(const std::string& params);
 protected:
  int node_p;
  int node_n;
  int node_ctrl_p;
  int node_ctrl_n;
};

class Resistor : public DoubleTerminalElement {
 public:
  Resistor(const std::string& name, int node1, int node2, amc_float R);
  explicit Resistor(const std::string& params);
 protected:
  amc_float R;
};

class NonLinearResistor : public DoubleTerminalElement {
 public:
  NonLinearResistor(const std::string& name, int node1, int node2,
                    const std::vector<amc_float>& R);
  explicit NonLinearResistor(const std::string& params);
 protected:
  std::vector<amc_float> R;
};

class VoltageControlledSwitch : public ControlledElement {
 public:
  VoltageControlledSwitch(const std::string& name, int node_p, int node_n,
                          int node_ctrl_p, int node_ctrl_n, amc_float g_on,
                          amc_float g_off, amc_float v_ref);
  explicit VoltageControlledSwitch(const std::string& params);
 protected:
  amc_float g_on;
  amc_float g_off;
  amc_float v_ref;
};

class Inductor : public DoubleTerminalElement {
 public:
  Inductor(const std::string& name, int node1, int node2, amc_float L,
           amc_float initial_current);
  explicit Inductor(const std::string& params);
 protected:
  amc_float L;
  amc_float initial_current;
};

class Capacitor : public DoubleTerminalElement {
 public:
  Capacitor(const std::string& name, int node1, int node2, amc_float C,
            amc_float initial_voltage);
  explicit Capacitor(const std::string& params);
 protected:
  amc_float C;
  amc_float initial_voltage;
};

class VoltageControlledVoltageSource : public ControlledElement {
 public:
  VoltageControlledVoltageSource(const std::string& name, int node_p,
                                 int node_n, int node_ctrl_p, int node_ctrl_n,
                                 amc_float Av);
  explicit VoltageControlledVoltageSource(const std::string& params);
 protected:
  amc_float Av;
};

class CurrentControlledCurrentSource : public ControlledElement {
 public:
  CurrentControlledCurrentSource(const std::string& name, int node_p,
                                 int node_n, int node_ctrl_p, int node_ctrl_n,
                                 amc_float Ai);
  explicit CurrentControlledCurrentSource(const std::string& params);
 protected:
  amc_float Ai;
};

class VoltageControlledCurrentSource : public ControlledElement {
 public:
  VoltageControlledCurrentSource(const std::string& name, int node_p,
                                 int node_n, int node_ctrl_p, int node_ctrl_n,
                                 amc_float Gm);
  explicit VoltageControlledCurrentSource(const std::string& params);
 protected:
  amc_float Gm;
};

class CurrentControlledVoltageSource : public ControlledElement {
 public:
  CurrentControlledVoltageSource(const std::string& name, int node_p,
                                 int node_n, int node_ctrl_p, int node_ctrl_n,
                                 amc_float Rm);
  explicit CurrentControlledVoltageSource(const std::string& params);
 protected:
  amc_float Rm;
};

class CurrentSource : public SimpleSourceElement {
 public:
  CurrentSource(const std::string& name, int node_p, int node_n,Signal* signal);
  explicit CurrentSource(const std::string& params);
};

class VoltageSource : public SimpleSourceElement {
 public:
  VoltageSource(const std::string& name, int node_p, int node_n,Signal* signal);
  explicit VoltageSource(const std::string& params);
};

class IdealOpAmp : public Element {
 public:
  IdealOpAmp(const std::string& name, int out_p, int out_n, int in_p, int in_n);
  IdealOpAmp(const std::string& params);
 protected:
  int out_p;
  int out_n;
  int in_p;
  int in_n;
};

} // namespace amcircuit

#endif //AMCIRCUIT_ELEMENT_H
