#pragma once

#include "base.hh"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace nut::io {
  template<typename... Args>
  inline auto println(const str& fmt, Args&&... args) -> void {
    std::cout << std::vformat(fmt, std::make_format_args(args...)) << '\n';
  }

  template<typename... Args>
  inline auto print(const str& fmt, Args&&... args) -> void {
    std::cout << std::vformat(fmt, std::make_format_args(args...));
  }

  template<typename... Args>
  inline auto eprintln(const str& fmt, Args&&... args) -> void {
    std::cerr << std::vformat(fmt, std::make_format_args(args...)) << '\n';
  }

  template<typename... Args>
  inline auto dprintln(const str& fmt, Args&&... args) -> void {
    #ifdef _WIN32
    OutputDebugStringA(std::vformat(fmt + '\n', std::make_format_args(args...)).c_str());
    #else
    println(fmt, std::forward<Args...>(args...));
    #endif
  }
}