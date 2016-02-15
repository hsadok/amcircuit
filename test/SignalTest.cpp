//
// Created by Hugo Sadok on 2/10/16.
//

#include <string>

#include "catch.hpp"

#include "Signal.h"

using namespace amcircuit;

// Getting rid of unused-value warning from GCC and clang
// It's a useful warning but doesn't make sense for test
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"

SCENARIO("Sinals should be created from a string", "[signal]") {
  GIVEN("A DC signal string") {
    std::string str = "DC 20";
    WHEN("Using the DC object") {
      std::string params(str);
      params.erase(0,3);
      DC* dc(new DC(params));
      THEN("The DC parameters should be specified") {
        REQUIRE(dc->get_value(0) == 20.0);
      }
      delete dc;
    }
    WHEN("Using the get_signal") {
      Signal::Handler element = Signal::get_signal(str);
      THEN("I should have a DC object") {
        REQUIRE_NOTHROW(dynamic_cast<DC&>(*element));
      }
      AND_THEN("I should not have a Sin object") {
        REQUIRE_THROWS(dynamic_cast<Sin&>(*element));
      }
    }
  }
  GIVEN("A Sin signal string") {
    std::string str = "SIN 0 1 2E3 1E-2 0.2 20 3";
    WHEN("Using the Sin object") {
      std::string params(str);
      params.erase(0,4);
      Sin* sin = new Sin(params);
      THEN("The DC parameters should be specified") {
        REQUIRE(sin->get_offset() == 0.0);
        REQUIRE(sin->get_amplitude() == 1);
        REQUIRE(sin->get_freq_hz() == 2000.0);
        REQUIRE(sin->get_time_delay() == 0.01);
        REQUIRE(sin->get_damping_factor() == 0.2);
        REQUIRE(sin->get_phase_deg() == 20);
        REQUIRE(sin->get_cycles() == 3);
      }
      delete sin;
    }
    WHEN("Using the get_signal") {
      Signal::Handler element = Signal::get_signal(str);
      THEN("I should have a Sin object") {
        REQUIRE_NOTHROW(dynamic_cast<Sin&>(*element));
      }
      AND_THEN("I should not have a DC object") {
        REQUIRE_THROWS(dynamic_cast<DC&>(*element));
      }
    }
  }
  GIVEN("A Pulse signal string") {
    std::string str = "PULSE 0.1 5.8 1e-3 4e-3 6e-3 10e-3 30e-3 7";
    WHEN("Using the Pulse object") {
      std::string params(str);
      params.erase(0,6);
      Pulse* pulse = new Pulse(params);
      THEN("The Pulse parameters should be specified") {
        REQUIRE(pulse->get_initial() == 0.1);
        REQUIRE(pulse->get_pulsed() == 5.8);
        REQUIRE(pulse->get_delay_time() == 1e-3);
        REQUIRE(pulse->get_rise_time() == 4e-3);
        REQUIRE(pulse->get_fall_time() == 6e-3);
        REQUIRE(pulse->get_pulse_width() == 10e-3);
        REQUIRE(pulse->get_period() == 30e-3);
        REQUIRE(pulse->get_cycles() == 7);
      }
      delete pulse;
    }
    WHEN("Using the get_signal") {
      Signal::Handler element = Signal::get_signal(str);
      THEN("I should have a Pulse object") {
        REQUIRE_NOTHROW(dynamic_cast<Pulse&>(*element));
      }
      AND_THEN("I should not have a Sin object") {
        REQUIRE_THROWS(dynamic_cast<Sin&>(*element));
      }
    }
  }
}
#pragma GCC diagnostic pop
