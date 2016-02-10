//
// Created by Hugo Sadok on 2/6/16.
//

#include "AMCircuitException.h"

namespace amcircuit {

AMCircuitException::AMCircuitException(const std::string& description)
    : std::runtime_error(description) { }

FileNotFound::FileNotFound(const std::string& description)
    : AMCircuitException(description) { }

BadFileException::BadFileException(const std::string& description)
    : AMCircuitException(description) { }

BadElementString::BadElementString(const std::string& description)
    : AMCircuitException(description) { }

} // namespace amcircuit
