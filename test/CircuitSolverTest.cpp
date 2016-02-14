//
// Created by Hugo Sadok on 2/14/16.
//

#include <string>

#include "catch.hpp"

#include "CircuitSolver.h"
#include "helpers.h"

using namespace amcircuit;

// Getting rid of unused-value warning from GCC and clang
// It's a useful warning but doesn't make sense for test
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"

SCENARIO("The CircuitSolver must calculate the circuit parameters",
         "[circuit_solver]") {
  GIVEN("A netlist without the analysis statement") {
    const std::string netlist_file_name = to_str(
        get_executable_path() << "/../test/support/defective_simples.net");
    Netlist nl = Netlist(netlist_file_name);
    WHEN("instantiating the CircuitSolver class") {
      THEN("an exception should be raised") {
        REQUIRE_THROWS(CircuitSolver cs(&nl));
      }
    }
  }
  GIVEN("A simple netlist") {
    const std::string netlist_file_name = to_str(
        get_executable_path() << "/../test/support/simplesR.net");
    Netlist nl = Netlist(netlist_file_name);
    WHEN("instantiating the CircuitSolver class") {
      CircuitSolver cs(&nl);
      std::stringstream ss;
      cs.write_to_stream(ss);
      std::string expected_output = "t 1 2 jV0200\n"
                                    "0 10 5 -0.005\n"
                                    "0.01 10 5 -0.005\n"
                                    "0.02 10 5 -0.005\n"
                                    "0.03 10 5 -0.005\n"
                                    "0.04 10 5 -0.005\n";
      REQUIRE( ss.str() == expected_output );
    }
  }
}
#pragma GCC diagnostic pop
