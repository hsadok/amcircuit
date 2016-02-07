//
// Created by Hugo Sadok on 2/6/16.
//

#ifndef AMCIRCUIT_AMCIRCUITEXCEPTION_H
#define AMCIRCUIT_AMCIRCUITEXCEPTION_H

#include <string>

namespace amcircuit {

class AMCircuitException : public std::runtime_error {
public:
  explicit AMCircuitException(const std::string&);
};

class FileNotFound : public AMCircuitException {
public:
  explicit FileNotFound(const std::string&);
};

class BadFileException : public AMCircuitException {
public:
  explicit BadFileException(const std::string&);
};

}

#endif //AMCIRCUIT_AMCIRCUITEXCEPTION_H
