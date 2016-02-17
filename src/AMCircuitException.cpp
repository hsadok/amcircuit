//
// Created by Hugo Sadok on 2/6/16.
//

#include <string>
#include "AMCircuitException.h"

namespace amcircuit {

AMCircuitException::AMCircuitException(const std::string& desc)
    : std::runtime_error(desc) { }

FileNotFound::FileNotFound(const std::string& desc)
    : AMCircuitException(desc) { }

BadFileException::BadFileException(const std::string& desc)
    : AMCircuitException(desc) { }

BadElementString::BadElementString(const std::string& desc)
    : AMCircuitException(desc) { }

BufferTooSmall::BufferTooSmall(const std::string& desc)
    : AMCircuitException(desc) { }

IncompleteNetList::IncompleteNetList(const std::string& desc)
    : AMCircuitException(desc) { }

InvalidIntegrationMethod::InvalidIntegrationMethod(const std::string& desc)
    : AMCircuitException(desc) { }

SingularSystem::SingularSystem(const std::string& desc)
    : AMCircuitException(desc) { }

}  // namespace amcircuit
