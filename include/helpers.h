//
// Created by Hugo Sadok on 2/9/16.
//

#ifndef AMCIRCUIT_HELPERS_H
#define AMCIRCUIT_HELPERS_H

#include <string>
#include <sstream>

std::string str_upper(std::string);

#define to_str( x ) static_cast< std::ostringstream & >( \
  ( std::ostringstream() << std::dec << x ) ).str()

#endif //AMCIRCUIT_HELPERS_H
