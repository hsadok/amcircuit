//
// Created by Hugo Sadok on 2/7/16.
//

#include <cmath>
#include <iostream>
#include "Signal.h"
#include "helpers.h"
#include "AMCircuitException.h"

namespace amcircuit {

Signal::Signal() { }

Signal::Signal(const std::string& params) : line_stream(params) { }

inline Signal::~Signal() { }

Signal::Handler Signal::get_signal(std::string params) {
  std::stringstream params_stream(params);
  std::string type;
  params_stream >> type;
  getline(params_stream, params);
  type = str_upper(type);
  if (type == "DC") return Handler(new DC(params));
  else if (type == "SIN") return Handler(new Sin(params));
  else if (type == "PULSE") return Handler(new Pulse(params));
  throw BadElementString("Invalid signal \"" + params + "\"");
}

Signal::Handler Signal::get_signal(std::istream& stream) {
  std::string params;
  stream >> std::ws;
  getline(stream, params);
  return get_signal(params);
}

DC::DC(const amc_float value) : value(value) { }

DC::DC(const std::string& params) : Signal(params) {
  line_stream >> value;
}

amc_float DC::get_value(amc_float) const {
  return value;
}

const amc_float Sin::PI = std::atan(1) * 4;

Sin::Sin(amc_float offset, amc_float amplitude, amc_float freq_hz,
         amc_float time_delay, amc_float damping_factor, amc_float phase_deg,
         int cycles)
    : offset(offset), amplitude(amplitude), freq_hz(freq_hz),
      time_delay(time_delay), damping_factor(damping_factor),
      phase_deg(phase_deg), cycles(cycles) { }

Sin::Sin(const std::string& params) : Signal(params) {
  line_stream >> offset >> amplitude >> freq_hz >> time_delay >>
      damping_factor >> phase_deg >> cycles;
}

amc_float Sin::get_offset() const {
  return offset;
}

amc_float Sin::get_amplitude() const {
  return amplitude;
}

amc_float Sin::get_freq_hz() const {
  return freq_hz;
}

amc_float Sin::get_time_delay() const {
  return time_delay;
}

amc_float Sin::get_damping_factor() const {
  return damping_factor;
}

amc_float Sin::get_phase_deg() const {
  return phase_deg;
}

int Sin::get_cycles() const {
  return cycles;
}

amc_float Sin::get_value(amc_float time) const {
  amc_float t;
  if (time <= time_delay) {
    t = 0;
  } else {
    t = time - time_delay;
  }
  if (t*freq_hz > cycles) {
    t = 0;
  }

  return offset + amplitude * std::exp(-damping_factor * t)
                  * std::sin(2*PI*freq_hz*t+PI/180*phase_deg);
}

Pulse::Pulse(amc_float initial, amc_float pulsed, amc_float delay_time,
             amc_float rise_time, amc_float fall_time, amc_float pulse_width,
             amc_float period, int cycles)
    : initial(initial), pulsed(pulsed), delay_time(delay_time),
      rise_time(rise_time), fall_time(fall_time), pulse_width(pulse_width),
      period(period), cycles(cycles) { }

Pulse::Pulse(const std::string& params) : Signal(params) {
  line_stream >> initial >> pulsed >> delay_time >> rise_time >> fall_time >>
      pulse_width >> period >> cycles;
}

amc_float Pulse::get_initial() const {
  return initial;
}

amc_float Pulse::get_pulsed() const {
  return pulsed;
}

amc_float Pulse::get_delay_time() const {
  return delay_time;
}

amc_float Pulse::get_rise_time() const {
  return rise_time;
}

amc_float Pulse::get_fall_time() const {
  return fall_time;
}

amc_float Pulse::get_pulse_width() const {
  return pulse_width;
}

amc_float Pulse::get_period() const {
  return period;
}

int Pulse::get_cycles() const {
  return cycles;
}

amc_float Pulse::get_value(amc_float time) const {
  amc_float t;
  if (time > period*cycles) {
    t = 0;
  } else {
    t = fmod(time, period);
  }

  t -= delay_time;
  if (t <= 0) {
    return initial;
  }

  if (t < rise_time) {
    return t * (pulsed - initial) / rise_time + initial;
  }
  t -= rise_time;

  t -= pulse_width;
  if (t <= 0) {
    return pulsed;
  }

  if (t < fall_time) {
    return t * (initial - pulsed) / fall_time + pulsed;
  }

  return initial;
}

}  // namespace amcircuit
