//
// Created by Hugo Sadok on 2/6/16.
//
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "Netlist.h"
#include "helpers.h"

using namespace amcircuit;

SCENARIO("netlist should be correctly processed from a file", "[netlist]") {

  GIVEN("a correct netlist file name") {
    const std::string netlist_file_name = to_str(
        get_executable_path() << "/../test/support/simples.net");

    WHEN("specifying a nonexistent file") {
      THEN("it should raise an exception") {
        REQUIRE_THROWS(Netlist("kasjhcsakjhbcaslkj"));
      }
    }
    WHEN("provided with the correct netlist file") {
      THEN("it should have the correct number of elements") {
        // 9 elements
        // 6 resistors
        // 2 AmpOp
        // 1 power supply
        Netlist nl = Netlist(netlist_file_name);
        const std::vector<Element::Handler>& elements = nl.get_elements();
        REQUIRE(elements.size() == 9);

      }
    }
  }
}
