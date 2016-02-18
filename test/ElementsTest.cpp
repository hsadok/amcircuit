//
// Created by Hugo Sadok on 2/7/16.
//

#include "catch.hpp"

#include "Elements.h"

using namespace amcircuit;

// Getting rid of unused-value warning from GCC and clang
// It's a useful warning but doesn't make sense for test
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"

SCENARIO("Elements should be created from a string", "[elements]") {
  GIVEN("A resistor string") {
    std::string str = "R0403 4 3 1";
    WHEN("Using the Resistor object") {
      Resistor* r = new Resistor(str);
      THEN("The resistor parameters should be specified") {
        REQUIRE(r->get_name() == "R0403");
        REQUIRE(r->get_node1() == 4);
        REQUIRE(r->get_node2() == 3);
        REQUIRE(r->get_R() == 1);
      }
      delete r;
    }
    WHEN("Using the get_element") {
      Element::Handler element = Element::get_element(str);
      THEN("I should have a Resistor object") {
        REQUIRE_NOTHROW(dynamic_cast<Resistor&>(*element));
      }
      AND_THEN("I should not have a Capacitor object") {
        REQUIRE_THROWS(dynamic_cast<Capacitor&>(*element));
      }
    }
  }
  GIVEN("A non linear resistor string") {
    std::string str = "N123 4 3 1 2 3 4 5 6 7 8";
    WHEN("Using the NonLinearResistor object") {
      NonLinearResistor* nlr = new NonLinearResistor(str);
      THEN("The non linear resistor parameters should be specified") {
        REQUIRE(nlr->get_name() == "N123");
        REQUIRE(nlr->get_node1() == 4);
        REQUIRE(nlr->get_node2() == 3);
        REQUIRE(nlr->get_coordinates()[0].first == 1);
        REQUIRE(nlr->get_coordinates()[0].second == 2);
        REQUIRE(nlr->get_coordinates()[1].first == 3);
        REQUIRE(nlr->get_coordinates()[1].second == 4);
        REQUIRE(nlr->get_coordinates()[2].first == 5);
        REQUIRE(nlr->get_coordinates()[2].second == 6);
        REQUIRE(nlr->get_coordinates()[3].first == 7);
        REQUIRE(nlr->get_coordinates()[3].second == 8);
      }
      delete nlr;
    }
    WHEN("Using the get_element") {
      Element::Handler element = Element::get_element(str);
      THEN("I should have a NonLinearResistor object") {
        REQUIRE_NOTHROW(dynamic_cast<NonLinearResistor&>(*element));
      }
      AND_THEN("I should not have a Capacitor object") {
        REQUIRE_THROWS(dynamic_cast<Capacitor&>(*element));
      }
    }

  }
  GIVEN("A voltage controlled switch string") {
    std::string str1 = "$000 4 3 2 1 10.3 5.2 3.3";
    std::string str2 = "$000 4 3 2 1";
    WHEN("Using the VoltageControlledSwitch object") {
      VoltageControlledSwitch* vcs1 = new VoltageControlledSwitch(str1);
      VoltageControlledSwitch* vcs2 = new VoltageControlledSwitch(str2);
      THEN("The voltage controlled switch parameters should be specified") {
        REQUIRE(vcs1->get_name() == "$000");
        REQUIRE(vcs1->get_node_p() == 4);
        REQUIRE(vcs1->get_node_n() == 3);
        REQUIRE(vcs1->get_node_ctrl_p() == 2);
        REQUIRE(vcs1->get_node_ctrl_n() == 1);
        REQUIRE(vcs1->get_g_on() == 10.3);
        REQUIRE(vcs1->get_g_off() == 5.2);
        REQUIRE(vcs1->get_v_ref() == 3.3);

        REQUIRE(vcs2->get_name() == "$000");
        REQUIRE(vcs2->get_node_p() == 4);
        REQUIRE(vcs2->get_node_n() == 3);
        REQUIRE(vcs2->get_node_ctrl_p() == 2);
        REQUIRE(vcs2->get_node_ctrl_n() == 1);
        REQUIRE(vcs2->get_g_on() == 0.0);
        REQUIRE(vcs2->get_g_off() == 0.0);
        REQUIRE(vcs2->get_v_ref() == 0.0);
      }
      delete vcs1;
      delete vcs2;
    }
    WHEN("Using the get_element") {
      Element::Handler element = Element::get_element(str1);
      THEN("I should have a VoltageControlledSwitch object") {
        REQUIRE_NOTHROW(dynamic_cast<VoltageControlledSwitch&>(*element));
      }
      AND_THEN("I should not have a Capacitor object") {
        REQUIRE_THROWS(dynamic_cast<Capacitor&>(*element));
      }
    }
  }
  GIVEN("An inductor string") {
    std::string str1 = "L123 4 3 3.7";
    std::string str2 =  "L124 5 4 3.8 IC=2.3";
    WHEN("Using the Inductor object") {
      Inductor* i1 = new Inductor(str1);
      Inductor* i2 = new Inductor(str2);
      THEN("The Inductor parameters should be specified") {
        REQUIRE(i1->get_name() == "L123");
        REQUIRE(i1->get_node1() == 4);
        REQUIRE(i1->get_node2() == 3);
        REQUIRE(i1->get_L() == 3.7);
        REQUIRE(i1->get_initial_current() == 0);

        REQUIRE(i2->get_name() == "L124");
        REQUIRE(i2->get_node1() == 5);
        REQUIRE(i2->get_node2() == 4);
        REQUIRE(i2->get_L() == 3.8);
        REQUIRE(i2->get_initial_current() == 2.3);
      }
      delete i1;
      delete i2;
    }
    WHEN("Using the get_element") {
      Element::Handler element = Element::get_element(str1);
      THEN("I should have a Inductor object") {
        REQUIRE_NOTHROW(dynamic_cast<Inductor&>(*element));
      }
      AND_THEN("I should not have a Capacitor object") {
        REQUIRE_THROWS(dynamic_cast<Capacitor&>(*element));
      }
    }
  }
  GIVEN("A capacitor string") {
    std::string str1 = "C123 4 3 3.7";
    std::string str2 = "C124 5 4 4.7 IC=2.1";
    WHEN("Using the Capacitor object") {
      Capacitor* c1 = new Capacitor(str1);
      Capacitor* c2 = new Capacitor(str2);
      THEN("The capacitor parameters should be specified") {
        REQUIRE(c1->get_name() == "C123");
        REQUIRE(c1->get_node1() == 4);
        REQUIRE(c1->get_node2() == 3);
        REQUIRE(c1->get_C() == 3.7);
        REQUIRE(c1->get_initial_voltage() == 0);

        REQUIRE(c2->get_name() == "C124");
        REQUIRE(c2->get_node1() == 5);
        REQUIRE(c2->get_node2() == 4);
        REQUIRE(c2->get_C() == 4.7);
        REQUIRE(c2->get_initial_voltage() == 2.1);
      }
      delete c1;
      delete c2;
    }
    WHEN("Using the get_element") {
      Element::Handler element = Element::get_element(str1);
      THEN("I should have a Capacitor object") {
        REQUIRE_NOTHROW(dynamic_cast<Capacitor&>(*element));
      }
      AND_THEN("I should not have a Inductor object") {
        REQUIRE_THROWS(dynamic_cast<Inductor&>(*element));
      }
    }
  }
  GIVEN("A voltage controlled voltage source string") {
    std::string str = "E123 4 3 2 1 20";
    WHEN("Using the VoltageControlledVoltageSource object") {
      VoltageControlledVoltageSource* vcvs = new VoltageControlledVoltageSource(
          str);
      THEN("The voltage controlled voltage source parameters should be specified") {
        REQUIRE(vcvs->get_name() == "E123");
        REQUIRE(vcvs->get_node_p() == 4);
        REQUIRE(vcvs->get_node_n() == 3);
        REQUIRE(vcvs->get_node_ctrl_p() == 2);
        REQUIRE(vcvs->get_node_ctrl_n() == 1);
        REQUIRE(vcvs->get_Av() == 20);
      }
      delete vcvs;
    }
    WHEN("Using the get_element") {
      Element::Handler element = Element::get_element(str);
      THEN("I should have a VoltageControlledVoltageSource object") {
        REQUIRE_NOTHROW(dynamic_cast<VoltageControlledVoltageSource&>(*element));
      }
      AND_THEN("I should not have a Capacitor object") {
        REQUIRE_THROWS(dynamic_cast<Capacitor&>(*element));
      }
    }
  }
  GIVEN("A current controlled current source string") {
    std::string str = "F123 4 3 2 1 20";
    WHEN("Using the CurrentControlledCurrentSource object") {
      CurrentControlledCurrentSource* cccs = new CurrentControlledCurrentSource(
          str);
      THEN("The current controlled current source parameters should be specified") {
        REQUIRE(cccs->get_name() == "F123");
        REQUIRE(cccs->get_node_p() == 4);
        REQUIRE(cccs->get_node_n() == 3);
        REQUIRE(cccs->get_node_ctrl_p() == 2);
        REQUIRE(cccs->get_node_ctrl_n() == 1);
        REQUIRE(cccs->get_Ai() == 20);
      }
      delete cccs;
    }
    WHEN("Using the get_element") {
      Element::Handler element = Element::get_element(str);
      THEN("I should have a CurrentControlledCurrentSource object") {
        REQUIRE_NOTHROW(dynamic_cast<CurrentControlledCurrentSource&>(*element));
      }
      AND_THEN("I should not have a Capacitor object") {
        REQUIRE_THROWS(dynamic_cast<Capacitor&>(*element));
      }
    }
  }
  GIVEN("A voltage controlled current source string") {
    std::string str = "G123 4 3 2 1 20";
    WHEN("Using the VoltageControlledCurrentSource object") {
      VoltageControlledCurrentSource* vccs = new VoltageControlledCurrentSource(
          str);
      THEN("The voltage controlled current source parameters should be specified") {
        REQUIRE(vccs->get_name() == "G123");
        REQUIRE(vccs->get_node_p() == 4);
        REQUIRE(vccs->get_node_n() == 3);
        REQUIRE(vccs->get_node_ctrl_p() == 2);
        REQUIRE(vccs->get_node_ctrl_n() == 1);
        REQUIRE(vccs->get_Gm() == 20);
      }
      delete vccs;
    }
    WHEN("Using the get_element") {
      Element::Handler element = Element::get_element(str);
      THEN("I should have a VoltageControlledCurrentSource object") {
        REQUIRE_NOTHROW(dynamic_cast<VoltageControlledCurrentSource&>(*element));
      }
      AND_THEN("I should not have a Capacitor object") {
        REQUIRE_THROWS(dynamic_cast<Capacitor&>(*element));
      }
    }
  }
  GIVEN("A current controlled voltage source string") {
    std::string str = "H123 4 3 2 1 20";
    WHEN("Using the CurrentControlledVoltageSource object") {
      CurrentControlledVoltageSource* ccvs = new CurrentControlledVoltageSource(
          str);
      THEN("The urrent controlled voltage source parameters should be specified") {
        REQUIRE(ccvs->get_name() == "H123");
        REQUIRE(ccvs->get_node_p() == 4);
        REQUIRE(ccvs->get_node_n() == 3);
        REQUIRE(ccvs->get_node_ctrl_p() == 2);
        REQUIRE(ccvs->get_node_ctrl_n() == 1);
        REQUIRE(ccvs->get_Rm() == 20);
      }
      delete ccvs;
    }
    WHEN("Using the get_element") {
      Element::Handler element = Element::get_element(str);
      THEN("I should have a CurrentControlledVoltageSource object") {
        REQUIRE_NOTHROW(dynamic_cast<CurrentControlledVoltageSource&>(*element));
      }
      AND_THEN("I should not have a Capacitor object") {
        REQUIRE_THROWS(dynamic_cast<Capacitor&>(*element));
      }
    }
  }
  GIVEN("A current source string") {
    std::string str = "I123 4 3 DC 12";
    WHEN("Using the CurrentSource object") {
      CurrentSource* cs = new CurrentSource(str);
      THEN("The current source parameters should be specified") {
        REQUIRE(cs->get_name() == "I123");
        REQUIRE(cs->get_node_p() == 4);
        REQUIRE(cs->get_node_n() == 3);
      }
      delete cs;
    }
    WHEN("Using the get_element") {
      Element::Handler element = Element::get_element(str);
      THEN("I should have a CurrentSource object") {
        REQUIRE_NOTHROW(dynamic_cast<CurrentSource&>(*element));
      }
      AND_THEN("I should not have a Capacitor object") {
        REQUIRE_THROWS(dynamic_cast<Capacitor&>(*element));
      }
    }
  }
  GIVEN("A voltage source string") {
    std::string str = "V123 4 3 DC 12";
    WHEN("Using the VoltageSource object") {
      VoltageSource* vs = new VoltageSource(str);
      THEN("The voltage source parameters should be specified") {
        REQUIRE(vs->get_name() == "V123");
        REQUIRE(vs->get_node_p() == 4);
        REQUIRE(vs->get_node_n() == 3);
      }
      delete vs;
    }
    WHEN("Using the get_element") {
      Element::Handler element = Element::get_element(str);
      THEN("I should have a VoltageSource object") {
        REQUIRE_NOTHROW(dynamic_cast<VoltageSource&>(*element));
      }
      AND_THEN("I should not have a Capacitor object") {
        REQUIRE_THROWS(dynamic_cast<Capacitor&>(*element));
      }
    }
  }
  GIVEN("An ideal AmpOp string") {
    std::string str = "O0300 3 0 1 0";
    WHEN("Using the IdealOpAmp object") {
      IdealOpAmp* ioa = new IdealOpAmp(str);
      THEN("The ideal AmpOp parameters should be specified") {
        REQUIRE(ioa->get_name() == "O0300");
        REQUIRE(ioa->get_out_p() == 3);
        REQUIRE(ioa->get_out_n() == 0);
        REQUIRE(ioa->get_in_p() == 1);
        REQUIRE(ioa->get_in_n() == 0);
      }
      delete ioa;
    }
    WHEN("Using the get_element") {
      Element::Handler element = Element::get_element(str);
      THEN("I should have a IdealOpAmp object") {
        REQUIRE_NOTHROW(dynamic_cast<IdealOpAmp&>(*element));
      }
      AND_THEN("I should not have a Capacitor object") {
        REQUIRE_THROWS(dynamic_cast<Capacitor&>(*element));
      }
    }
  }
}
#pragma GCC diagnostic pop
