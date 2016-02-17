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


} // namespace amcircuit

#endif //AMCIRCUIT_AMCIRCUIT_H

