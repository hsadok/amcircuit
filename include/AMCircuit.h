//
// Created by Hugo Sadok on 2/6/16.
//

#ifndef AMCIRCUIT_AMCIRCUIT_H
#define AMCIRCUIT_AMCIRCUIT_H

namespace amcircuit {

typedef double amc_float; // defining type for better control over precision

static const int NEWTON_RAPHSON_CYCLE_LIMIT = 40;
static const int NEWTON_RAPHSON_IA_RETRIES = 10;
static const amc_float ACCEPTABLE_NR_ERROR = 1E-6;
static const amc_float MAX_NR_GUESS = 1E3;

// The following  is used to decrease the internal step so that it can be used
// to find the initial conditions it must be < 1
static const amc_float IC_SCALING_STEP = 1e-8;

static const int NUM_STEPS_IC = 20;


} // namespace amcircuit

#endif //AMCIRCUIT_AMCIRCUIT_H

