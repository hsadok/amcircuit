//
// Created by Hugo Sadok on 2/11/16.
//

#include <string>

#include "catch.hpp"

#include "Statement.h"

using namespace amcircuit;

SCENARIO("Statements should be created from a string", "[statment]") {
  GIVEN("A Transient statement string") {
    std::string str = ".TRAN 40 8E-2 ADMO4 1 UIC";
    WHEN("using the Tran object") {
      std::string params(str);
      params.erase(0,6);
      Tran* tran(new Tran(params));
      THEN("The transient parameters should be specified") {
        REQUIRE(tran->get_t_stop_s() == 40.0);
        REQUIRE(tran->get_t_step_s() == 8E-2);
        REQUIRE(tran->get_admo_order() == 4);
        REQUIRE(tran->get_internal_steps() == 1);
      }
      delete tran;
    }
    WHEN("Using the get_statement") {
      AND_WHEN("the string has a dot in the beginning") {
        Statement::Handler element = Statement::get_statement(str);
        THEN("I should have a Statement object") {
          REQUIRE_NOTHROW(dynamic_cast<Statement&>(*element));
        }
      }
      AND_WHEN("the string doesn't have a dot in the beginning") {
        std::string no_dot_str(str);
        no_dot_str.erase(0, 1);
        Statement::Handler element = Statement::get_statement(no_dot_str);
        THEN("I should still have a Statement object") {
          REQUIRE_NOTHROW(dynamic_cast<Statement&>(*element));
        }
      }
    }
  }
}
