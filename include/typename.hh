#pragma once

#include "base.hh"

#include <typeinfo>

namespace nut::typeinfo {
  template<typename T>
  str full_type_name() {
    return typeid(T).name();
  }  
}