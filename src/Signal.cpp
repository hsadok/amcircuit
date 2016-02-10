//
// Created by Hugo Sadok on 2/7/16.
//

#include "Signal.h"
#include "helpers.h"
#include "AMCircuitException.h"

namespace amcircuit {

Signal::Signal() { }

Signal::Signal(const std::string& params) : line_stream(params) { }

inline Signal::~Signal() { }

Signal::Handler Signal::get_signal(const std::string params) {
  std::string upper_params(params); // TODO str_upper
  if (upper_params == "DC") return Handler(new DC(params));
  else if (upper_params == "SIN") return Handler(new Sin(params));
  else if (upper_params == "PULSE") return Handler(new Pulse(params));
  throw BadElementString("Invalid string \"" + params + "\"");
}

DC::DC(const amc_float value) : value(value) { }

DC::DC(const std::string& params) : Signal(params) {
  line_stream >> value;
}

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

} // namespace amcircuit
