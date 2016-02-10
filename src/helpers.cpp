//
// Created by Hugo Sadok on 2/9/16.
//

#include <algorithm>
#include "helpers.h"

namespace amcircuit {

std::string str_upper(std::string str) {
  for (unsigned long i = 0; i < str.size(); ++i){
    str[i] = (char) toupper(str[i]);
  }
  return str;
}

} // namespace amcircuit
