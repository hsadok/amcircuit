//
// Created by Hugo Sadok on 2/7/16.
//

#ifndef AMCIRCUIT_SIGNAL_H
#define AMCIRCUIT_SIGNAL_H

#include <string>
#include <sstream>

#include "AMCircuit.h"

namespace amcircuit {

class Signal {
 public:
  Signal();
  explicit Signal(const std::string& params);
  virtual ~Signal() = 0;
  explicit Signal(const Signal& other);
  Signal& operator=(const Signal& other);
  static Signal* get_signal(const std::string params);
 protected:
  std::stringstream line_stream;
};

class DC : public Signal {
 public:
  explicit DC(const amc_float value);
  explicit DC(const std::string& params);
 protected:
  amc_float value;
};

class Sin : public Signal {
 public:
  Sin(amc_float offset, amc_float amplitude, amc_float freq_hz,
      amc_float time_delay, amc_float damping_factor, amc_float phase_deg,
      int cycles);
  explicit Sin(const std::string& params);
 protected:
  amc_float offset;
  amc_float amplitude;
  amc_float freq_hz;
  amc_float time_delay;
  amc_float damping_factor;
  amc_float phase_deg;
  int cycles;
};

class Pulse : public Signal {
 public:
  Pulse(amc_float initial, amc_float pulsed, amc_float delay_time,
        amc_float rise_time, amc_float fall_time, amc_float pulse_width,
        amc_float period, int cycles);
  explicit Pulse(const std::string& params);
 protected:
  amc_float initial;
  amc_float pulsed;
  amc_float delay_time;
  amc_float rise_time;
  amc_float fall_time;
  amc_float pulse_width;
  amc_float period;
  int cycles;
};

} // namespace amcircuit

#endif //AMCIRCUIT_SIGNAL_H
