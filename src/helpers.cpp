//
// Created by Hugo Sadok on 2/9/16.
//

#include <algorithm>
#include <string>

#include "helpers.h"
#include "AMCircuitException.h"

namespace amcircuit {

std::string str_upper(std::string str) {
  for (unsigned long i = 0; i < str.size(); ++i){
    str[i] = (char) toupper(str[i]);
  }
  return str;
}

#ifdef __APPLE__
#include <mach-o/dyld.h>
  std::string get_executable_path() {
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) != 0)
      throw BufferTooSmall(to_str("need size: " << size << std::endl));
    std::string string_path = std::string(path);
    return string_path.substr(0, string_path.find_last_of("\\/"));
  }
#elif __linux__
#include <unistd.h>
#include <limits.h>
  std::string get_executable_path() {
    char buff[PATH_MAX];
    ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff)-1);
    if (len == -1) {
      throw BufferTooSmall(to_str("need size: " << PATH_MAX << std::endl));
    }
    buff[len] = '\0';
    std::string string_path = std::string(buff);
    return string_path.substr(0, string_path.find_last_of("\\/"));
  }
#else  // Assume Windows, gets rid of Windows mess
#include <windows.h>
  std::string get_executable_path() {
    wchar_t buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
    string::size_type pos = string( buffer ).find_last_of( "\\/" );
    return std::string( buffer ).substr( 0, pos);
  }
#endif


} // namespace amcircuit
