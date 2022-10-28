#pragma once

#include "base.hh"

namespace nut::scope {
  template<typename Fn>
  struct scope_guard {
  public:
    scope_guard(Fn fn) : _fn(std::move(fn)) { }
    scope_guard(scope_guard&& other) : _fn(std::move(other._fn)) {
      other.reset();
    }

    scope_guard& operator=(scope_guard&& other) {
      _fn = std::move(other._fn);
      other.reset();
    }

    ~scope_guard() {
      if (valid) _fn();
      valid = false;
    }

    void reset() {
      valid = false;
    }
  private:
    Fn _fn;
    bool valid = true;
  };
}