#pragma once

#include <array>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <ranges>
#include <span>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>
#include <cassert>

namespace nut {
  using u8 = ::std::uint8_t;
  using i8 = ::std::int8_t;
  using u16 = ::std::uint16_t;
  using i16 = ::std::int16_t;
  using u32 = ::std::uint32_t;
  using i32 = ::std::int32_t;
  using u64 = ::std::uint64_t;
  using i64 = ::std::int64_t;
  using f32 = float;
  using usize = ::std::size_t;
  static_assert(sizeof(f32) == 4, "f32 has to be 4 bytes");
  using f64 = double;
  static_assert(sizeof(f64) == 8, "f64 has to be 8 byte");
  using str = ::std::string;
  using strview = ::std::string_view;
  using wstr = ::std::wstring;
  using wstrview = ::std::wstring_view; 

  namespace env {
    using ArgsContainer = ::std::vector<strview>;
    static ArgsContainer __g_args;

    inline auto args() -> const ArgsContainer& { return __g_args; }
    inline auto init_args(const int argc, const char** argv) {
      __g_args = ::std::vector<strview>(argv, argv + argc);
    }
  }

  template<typename T, typename Sz = usize>
  struct size_of : std::integral_constant<Sz, sizeof(T)> {};
  template<typename T>
  using size_of_u32 = size_of<T, u32>;

  template<typename Sz = size_t, typename A>
  inline constexpr auto array_byte_size(A&& a) -> Sz {
    return static_cast<Sz>(
      sizeof(typename std::remove_cvref_t<decltype(a)>::value_type)
      * std::size(a)
    );
  }

  template<typename T>
  inline auto zero_memory(void* dest) noexcept -> ::std::optional<void*> {
    if (dest == nullptr) { return std::nullopt; }
    auto count_bytes = sizeof(T);
    u8* ptr { static_cast<u8*>(dest) };
    for (; count_bytes != 0; ++ptr, --count_bytes) {
      *ptr = 0;
    }
    return dest;
  }
  template<typename T>
  inline auto secure_zero_memory(void* dest) noexcept -> std::optional<void*> {
    if (dest == nullptr) { return std::nullopt; }
    auto count_bytes = sizeof(T);
    volatile u8* ptr { static_cast<volatile u8*>(dest) };
    for (; count_bytes != 0; ++ptr, --count_bytes) {
      *ptr = 0;
    }
    return dest;
  }

  constexpr inline auto toggle_bool(bool& b) noexcept -> bool {
    b = !b;
    return b;
  }
}