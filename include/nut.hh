#pragma once

#include "base.hh"
#include "id.hh"
#include "io.hh"
#include "scope.hh"
#include "typename.hh"

namespace nut {
  template<typename... Args>
  [[noreturn]] inline auto panic(const str& fmt, Args&&... args) -> void {
    do {
      std::cerr << std::vformat(fmt, std::make_format_args(args...));
      abort();
    } while(false);
  }
  [[noreturn]] inline auto unreachable() -> void { panic("entered unreachable code"); }
  [[noreturn]] inline auto todo() -> void { panic("not yet implemented"); }
  [[noreturn]] inline auto unimplemented() -> void { panic("not implemented"); }

  template<typename... Ts> struct overload : Ts... { using Ts::operator()...; };
}