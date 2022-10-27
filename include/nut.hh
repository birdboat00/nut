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

#ifdef WIN32
#include <Windows.h>
#endif

namespace nut {
  using u8 = std::uint8_t;
  using i8 = std::int8_t;
  using u16 = std::uint16_t;
  using i16 = std::int16_t;
  using u32 = std::uint32_t;
  using i32 = std::int32_t;
  using u64 = std::uint64_t;
  using i64 = std::int64_t;
  using f32 = float;
  using usize = std::size_t;
  static_assert(sizeof(f32) == 4, "f32 has to be 4 bytes");
  using f64 = double;
  static_assert(sizeof(f64) == 8, "f64 has to be 8 byte");
  using str = std::string;
  using strview = std::string_view;
  using wstr = std::wstring;
  using wstrview = std::wstring_view; 

  namespace env {
    using ArgsContainer = std::vector<std::string_view>;
    static ArgsContainer __g_args;

    inline auto args() -> const ArgsContainer& { return __g_args; }
    inline auto init_args(const int argc, const char** argv) {
      __g_args = std::vector<std::string_view>(argv, argv + argc);
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
  inline auto zero_memory(void* dest) noexcept -> std::optional<void*> {
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

  template<typename... Args>
  inline auto println(const str& fmt, Args&&... args) -> void {
    std::cout << std::vformat(fmt, std::make_format_args(args...)) << '\n';
  }

  template<typename... Args>
  inline auto eprintln(const str& fmt, Args&&... args) -> void {
    std::cerr << std::vformat(fmt, std::make_format_args(args...)) << '\n';
  }

  template<typename... Args>
  inline auto dprintln(const str& fmt, Args&&... args) -> void {
    #ifdef WIN32
    OutputDebugStringA(std::vformat(fmt + '\n', std::make_format_args(args...)).c_str());
    #else
    println(fmt, std::forward<Args...>(args...));
    #endif
  }

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

  struct id {
  public:
    id() : _ns(""), _path(""), _tag(false) {}
    id(const str& ns, const str& path, const bool tag) : _ns(ns), _path(path), _tag(tag) {}
    // TODO: full id parsing constructor
    inline static auto of(const str& ns, const str& path) -> id {
      return id(ns, path, false);
    }
    inline static auto tag_of(const str& ns, const str& path) -> id {
      return id(ns, path, true);
    }
    // TODO: should remove this, to unsecure.
    inline static auto parse(const str& text) -> std::optional<id> {
      std::stringstream ss(text);
      std::string item;
      std::vector<std::string> splitted_strings;
      while(std::getline(ss, item, id::kIdSeparator)) {
        splitted_strings.push_back(item);
      }
      if (splitted_strings.size() != 2) {
        eprintln("id malformed");
        return std::nullopt;
      }
      const auto is_tag = splitted_strings.at(0).starts_with('#');
      if (is_tag) {
        splitted_strings.at(0) = splitted_strings.at(0).substr(1, splitted_strings.at(0).size() - 1);
      }
      return id(splitted_strings.at(0), splitted_strings.at(1), is_tag);
    }

    inline auto full() const -> str {
      return _tag
        ? std::format("#{}{}{}", _ns, kIdSeparator, _path)
        : std::format("{}{}{}", _ns, kIdSeparator, _path);
    }
    inline auto ns() const -> str { return _ns; }
    inline auto path() const -> str { return _path; }
    inline auto filepath() const -> str { return std::format("{}/{}", _ns, _path); }
    inline auto tag() const -> bool { return _tag; }

    auto operator==(const id& other) const -> bool {
      return _ns == other._ns && _path == other._path;
    }
  private:
    static const char kIdSeparator { ':' };
    str _ns, _path;
    bool _tag;
  };
  std::ostream& operator<<(std::ostream& os, const id& self) {
    os << self.full();
    return os;
  }
}
namespace std {
  using namespace nut;
  std::string to_string(const id& id) {
    return id.full();
  }
  template<> struct hash<id> {
    size_t operator()(const id& id) const noexcept {
      return std::hash<std::string>()(id.full());
    }
  };
  template<>
  struct formatter<id> {
    constexpr auto parse(format_parse_context& ctx) {
      return ctx.begin();
    }
    auto format(const id& id, format_context& ctx) {
      return format_to(ctx.out(), "{}", id.full());
    }
  };
}
