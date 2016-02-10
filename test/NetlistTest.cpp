//
// Created by Hugo Sadok on 2/6/16.
//
#include "catch.hpp"
#include <string>
#include "Netlist.h"

using namespace amcircuit;

SCENARIO( "netlist should be correctly processed from a file", "[netlist]" ) {

    GIVEN( "a correct netlist file name" ) {
        const std::string netlistFileName = "support/simples.cir";

        WHEN( "specifying a nonexistent file" ) {
            THEN( "it should raise an exception" ) {
                REQUIRE_THROWS(Netlist(std::string("kasjhcsakjhbcaslkj")));
            }
        }
        WHEN( "provided with the correct netlist file" ) {
            THEN( "it should have the correct number of elements" ) {
                // 9 elements
                // 6 resistors
                // 2 AmpOp
                // 1 power supply
            }
        }
    }
}
