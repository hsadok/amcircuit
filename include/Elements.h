//
// Created by Hugo Sadok on 2/6/16.
//

#ifndef AMCIRCUIT_ELEMENT_H
#define AMCIRCUIT_ELEMENT_H

#include <string>
#include <sstream>
#include <vector>

#include "AMCircuit.h"
#include "Signal.h"
#include "ResourceHandler.h"

namespace amcircuit {

class Element {
 public:
  explicit Element(const std::string& params);
  virtual ~Element() = 0;

  typedef ResourceHandler<Element> Handler;
  static Element::Handler get_element(const std::string& element_string);

  std::string get_name() const;
  virtual int get_num_of_currents() const;
  virtual void flush_values();
  virtual void place_stamp(amc_float time, amc_float** A, amc_float* x,
                           amc_float* b, int method_order,
                           int currents_position, bool uic=false);

 protected:
  std::stringstream line_stream;

 private:
  std::string name;
  int num_of_currents;
  Element(const Element& other);
  Element& operator=(const Element& other);
};

class DoubleTerminalElement : public Element {
 public:
  DoubleTerminalElement(const std::string& name, int node1, int node2);
  explicit DoubleTerminalElement(const std::string& params);
  int get_node1() const;
  int get_node2() const;
 private:
  int node1;
  int node2;
};

class SimpleSourceElement : public Element {
 public:
  SimpleSourceElement(const std::string& name, int node_p, int node_n);
  explicit SimpleSourceElement(const std::string& params);
  int get_node_p() const;
  int get_node_n() const;

 private:
  int node_p;
  int node_n;
};

class ArbitrarySourceElement : public SimpleSourceElement {
 public:
  ArbitrarySourceElement(const std::string& name, int node_p, int node_n,
                      Signal::Handler signal);
  explicit ArbitrarySourceElement(const std::string& params);
  const Signal::Handler& get_signal() const;
 protected:
  Signal::Handler signal;
};

class ControlledElement : public Element {
 public:
  ControlledElement(const std::string& name, int node_p, int node_n,
                    int node_ctrl_p, int node_ctrl_n);
  explicit ControlledElement(const std::string& params);
  int get_node_p() const;
  int get_node_n() const;
  int get_node_ctrl_p() const;
  int get_node_ctrl_n() const;

 private:
  int node_p;
  int node_n;
  int node_ctrl_p;
  int node_ctrl_n;
};

class Resistor : public DoubleTerminalElement {
 public:
  Resistor(const std::string& name, int node1, int node2, amc_float R);
  explicit Resistor(const std::string& params);

  amc_float get_R() const;

 private:
  amc_float R;
};

class NonLinearResistor : public DoubleTerminalElement {
 public:
  NonLinearResistor(const std::string& name, int node1, int node2,
                    const std::vector<amc_float>& R);
  explicit NonLinearResistor(const std::string& params);
  const std::vector<amc_float>& get_R() const;
 protected:
  std::vector<amc_float> R;
};

class VoltageControlledSwitch : public ControlledElement {
 public:
  VoltageControlledSwitch(const std::string& name, int node_p, int node_n,
                          int node_ctrl_p, int node_ctrl_n, amc_float g_on,
                          amc_float g_off, amc_float v_ref);
  explicit VoltageControlledSwitch(const std::string& params);

  amc_float get_g_on() const;
  amc_float get_g_off() const;
  amc_float get_v_ref() const;

 private:
  amc_float g_on;
  amc_float g_off;
  amc_float v_ref;
};

class Inductor : public DoubleTerminalElement {
 public:
  Inductor(const std::string& name, int node1, int node2, amc_float L,
           amc_float initial_current);
  explicit Inductor(const std::string& params);

  amc_float get_L() const;
  amc_float get_initial_current() const;

 private:
  amc_float L;
  amc_float initial_current;
};

class Capacitor : public DoubleTerminalElement {
 public:
  Capacitor(const std::string& name, int node1, int node2, amc_float C,
            amc_float initial_voltage);
  explicit Capacitor(const std::string& params);

  amc_float get_C() const;
  amc_float get_initial_voltage() const;

 private:
  amc_float C;
  amc_float initial_voltage;
};

class VoltageControlledVoltageSource : public ControlledElement {
 public:
  VoltageControlledVoltageSource(const std::string& name, int node_p,
                                 int node_n, int node_ctrl_p, int node_ctrl_n,
                                 amc_float Av);
  explicit VoltageControlledVoltageSource(const std::string& params);
  amc_float get_Av() const;

 private:
  amc_float Av;
};

class CurrentControlledCurrentSource : public ControlledElement {
 public:
  CurrentControlledCurrentSource(const std::string& name, int node_p,
                                 int node_n, int node_ctrl_p, int node_ctrl_n,
                                 amc_float Ai);
  explicit CurrentControlledCurrentSource(const std::string& params);
  amc_float get_Ai() const;

 private:
  amc_float Ai;
};

class VoltageControlledCurrentSource : public ControlledElement {
 public:
  VoltageControlledCurrentSource(const std::string& name, int node_p,
                                 int node_n, int node_ctrl_p, int node_ctrl_n,
                                 amc_float Gm);
  explicit VoltageControlledCurrentSource(const std::string& params);
  amc_float get_Gm() const;

 private:
  amc_float Gm;
};

class CurrentControlledVoltageSource : public ControlledElement {
 public:
  CurrentControlledVoltageSource(const std::string& name, int node_p,
                                 int node_n, int node_ctrl_p, int node_ctrl_n,
                                 amc_float Rm);
  explicit CurrentControlledVoltageSource(const std::string& params);
  amc_float get_Rm() const;

 private:
  amc_float Rm;
};

class CurrentSource : public ArbitrarySourceElement {
 public:
  CurrentSource(const std::string& name, int node_p, int node_n,
                Signal::Handler signal);
  explicit CurrentSource(const std::string& params);
};

class VoltageSource : public ArbitrarySourceElement {
 public:
  VoltageSource(const std::string& name, int node_p, int node_n,
                Signal::Handler signal);
  explicit VoltageSource(const std::string& params);
};

class IdealOpAmp : public Element {
 public:
  IdealOpAmp(const std::string& name, int out_p, int out_n, int in_p, int in_n);
  explicit IdealOpAmp(const std::string& params);

  int get_out_p() const;
  int get_out_n() const;
  int get_in_p() const;
  int get_in_n() const;

 private:
  int out_p;
  int out_n;
  int in_p;
  int in_n;
};

} // namespace amcircuit

#endif //AMCIRCUIT_ELEMENT_H
