//
// Created by Hugo Sadok on 2/9/16.
//

#ifndef AMCIRCUIT_HELPERS_H
#define AMCIRCUIT_HELPERS_H

#include <string>
#include <sstream>

namespace amcircuit {

std::string str_upper(std::string);

std::string get_executable_path();

#define to_str( x ) static_cast< std::ostringstream & >( \
  ( std::ostringstream() << std::dec << x ) ).str()

}  // namespace amcircuit

#endif //AMCIRCUIT_HELPERS_H
