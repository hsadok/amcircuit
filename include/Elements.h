//
// Created by Hugo Sadok on 2/6/16.
//

#ifndef AMCIRCUIT_ELEMENT_H
#define AMCIRCUIT_ELEMENT_H

#include <string>

#include "AMCircuit.h"

namespace amcircuit {

class Element {
public:
  Element(const std::string& name);
  Element(const Element& other);
  Element& operator=(const Element& other);
  virtual ~Element() = 0;
private:
  const std::string name;
};

class DoubleTerminalElement : public Element {
public:
  DoubleTerminalElement(const std::string& name, int node1, int node2);
private:
  int node1;
  int node2;
};

class SimpleSourceElement : public Element {
public:
  SimpleSourceElement(const std::string& name);
private:
  amc_float node_p;
  amc_float node_n;
  class Value {
  public:
    enum Behaviours { DC, SIN, PULSE };
  };
};

class ControlledElement : public Element {
public:
  ControlledElement(const std::string& name, int node_p, int node_n,
                    int node_ctrl_p, int node_ctrl_n);
private:
  int node_p;
  int node_n;
  int node_ctrl_p;
  int node_ctrl_n;
};

class Resistor : public DoubleTerminalElement {
public:
  Resistor(const std::string& name, int node1, int node2, amc_float R);
private:
  amc_float R;
};

class NonLinearResistor : public DoubleTerminalElement {
public:
  NonLinearResistor(const std::string& name, int node1, int node2,
                    std::vector<amc_float> R);
private:
  std::vector<amc_float> R;
};

class VoltageControlledSwitch : public ControlledElement {
public:
  VoltageControlledSwitch(const std::string& name, int node_p, int node_n,
                          int node_ctrl_p, int node_ctrl_n, amc_float g_on,
                          amc_float g_off, amc_float v_ref);
private:
  amc_float g_on;
  amc_float g_off;
  amc_float v_ref;
};

class Inductor : public DoubleTerminalElement {
public:
  Inductor(const std::string& name, const std::string& params);
private:
  amc_float L;
  amc_float initial_current;
};

class Capacitor : public DoubleTerminalElement {
public:
  Capacitor(const std::string& name, const std::string& params);
private:
  amc_float C;
  amc_float initial_voltage;
};

class VoltageControlledVoltageSource : public ControlledElement {
public:
  VoltageControlledVoltageSource(const std::string& name,
                                 const std::string& params);
private:
  amc_float Av;
};

class CurrentControlledCurrentSource : public ControlledElement {
public:
  CurrentControlledCurrentSource(const std::string& name,
                                 const std::string& params);
private:
  amc_float Ai;
};

class VoltageControlledCurrentSource : public ControlledElement {
public:
  VoltageControlledCurrentSource(const std::string& name,
                                 const std::string& params);
private:
  amc_float Gm;
};

class CurrentControlledVoltageSource : public ControlledElement {
public:
  CurrentControlledVoltageSource(const std::string& name,
                                 const std::string& params);
private:
  amc_float Rm;
};

class CurrentSource : public SimpleSourceElement {
public:
  CurrentSource(const std::string& name,
                const std::string& params);
private:

};

class VoltageSource : public SimpleSourceElement {
public:
  VoltageSource(const std::string& name,
                const std::string& params);
};

class IdealOpAmp : public Element {
public:
  IdealOpAmp(const std::string& name,
             const std::string& params);
};

}

#endif //AMCIRCUIT_ELEMENT_H
