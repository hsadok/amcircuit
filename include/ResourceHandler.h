//
// Created by Hugo Sadok on 2/9/16.
//

#ifndef AMCIRCUIT_RESOURCEHANDLER_H
#define AMCIRCUIT_RESOURCEHANDLER_H

// Template to handle resources
// This works pretty similar to a smart pointer. The reason I didn't something
// like `auto_ptr` here is that I didn't want to depend on C++11 or Boost.
template<typename T>
class ResourceHandler {
 public:
  ResourceHandler(T* resource_ptr) : resource_ptr(resource_ptr) {
    ref_counter = new int(1);
  }
  void add(const ResourceHandler& other) {
    ref_counter = other.ref_counter;
    resource_ptr = other.resource_ptr;
    (*ref_counter)++;
  }
  void remove() {
    if (*ref_counter > 1) {
      (*ref_counter)--;
    } else {
      delete resource_ptr;
      delete ref_counter;
    }
  }
  ResourceHandler(const ResourceHandler& other) {
    add(other);
  }
  ResourceHandler& operator=(const ResourceHandler& rhs) {
    if (resource_ptr != rhs.resource_ptr){
      remove();
      add(rhs);
    }

    return *this;
  }

  ~ResourceHandler() {
    remove();
  }
  T* operator->() {
    return resource_ptr;
  }
 private:
  T* resource_ptr;
  int* ref_counter;
};

#endif //AMCIRCUIT_RESOURCEHANDLER_H
