#pragma once

#include <filesystem>

#include "base.hh"

namespace nut {
  struct id {
  public:
    id() : _ns(""), _path(""), _tag(false) {}
    id(const str& ns, const str& path, const bool tag) : _ns(ns), _path(path), _tag(tag) {}

    inline static auto of(const str& ns, const str& path) -> id {
      return id(ns, path, false);
    }
    inline static auto tag_of(const str& ns, const str& path) -> id {
      return id(ns, path, true);
    }

    inline auto full() const -> str {
      return _tag
        ? std::format("#{}{}{}", _ns, kIdSeparator, _path)
        : std::format("{}{}{}", _ns, kIdSeparator, _path);
    }
    inline auto ns() const -> str { return _ns; }
    inline auto path() const -> str { return _path; }
    inline auto tag() const -> bool { return _tag; }
    
    inline auto as_filepath_str() const -> str { return std::format("{}/{}", _ns, _path); }
    inline auto as_filepath() const -> std::filesystem::path { return std::filesystem::path(std::format("{}/{}", _ns, _path)); }

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
  using namespace ::nut;
  
  str to_string(const id& id) {
    return id.full();
  }

  template<> struct hash<id> {
    usize operator()(const id& id) const noexcept {
      return hash<str>()(id.full());
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