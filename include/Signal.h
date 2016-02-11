//
// Created by Hugo Sadok on 2/7/16.
//

#ifndef AMCIRCUIT_SIGNAL_H
#define AMCIRCUIT_SIGNAL_H

#include <string>
#include <sstream>

#include "AMCircuit.h"
#include "ResourceHandler.h"

namespace amcircuit {

class Signal {
 public:
  Signal();
  Signal(const std::string& params);
  virtual ~Signal() = 0;

  typedef ResourceHandler<Signal> Handler;
  static Signal::Handler get_signal(std::string params);
  static Signal::Handler get_signal(std::istream& stream);

 protected:
  std::stringstream line_stream;

 private:
  explicit Signal(const Signal& other);
  Signal& operator=(const Signal& other);
};

class DC : public Signal {
 public:
  explicit DC(const amc_float value);
  explicit DC(const std::string& params);

  amc_float get_value() const;
 private:
  amc_float value;
};

class Sin : public Signal {
 public:
  Sin(amc_float offset, amc_float amplitude, amc_float freq_hz,
      amc_float time_delay, amc_float damping_factor, amc_float phase_deg,
      int cycles);
  explicit Sin(const std::string& params);

  amc_float get_offset() const;
  amc_float get_amplitude() const;
  amc_float get_freq_hz() const;
  amc_float get_time_delay() const;
  amc_float get_damping_factor() const;
  amc_float get_phase_deg() const;
  int get_cycles() const;

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

  amc_float get_initial() const;
  amc_float get_pulsed() const;
  amc_float get_delay_time() const;
  amc_float get_rise_time() const;
  amc_float get_fall_time() const;
  amc_float get_pulse_width() const;
  amc_float get_period() const;
  int get_cycles() const;

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
