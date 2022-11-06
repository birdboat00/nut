#include <catch2/catch_test_macros.hpp>

#include <nut.hh>

TEST_CASE("types", "[types]") {
  REQUIRE(sizeof(nut::u8) == 1);
  REQUIRE(sizeof(nut::i8) == 1);
  REQUIRE(sizeof(nut::u16) == 2);
  REQUIRE(sizeof(nut::i16) == 2);
  REQUIRE(sizeof(nut::u32) == 4);
  REQUIRE(sizeof(nut::i32) == 4);
  REQUIRE(sizeof(nut::u64) == 8);
  REQUIRE(sizeof(nut::i64) == 8);
  REQUIRE(sizeof(nut::f32) == 4);
  REQUIRE(sizeof(nut::f64) == 8);
}


struct size_of_test_struct {
  nut::u32 a;
  nut::u16 b;
  nut::u8 c;
};

static bool toggle_bool_test_bool = false;

const std::array<nut::i32, 5> array_byte_size_test_array = { 1, 2, 3, 4, 5 };
enum class underlying_test_enum : nut::u32 { a = 0, b = 10, c = 20 };

TEST_CASE("utility functions", "[utility]") {
  SECTION("size_of<T>") {
    REQUIRE(nut::size_of<nut::u8>::value == sizeof(nut::u8));
    REQUIRE(nut::size_of<nut::u32>::value == sizeof(nut::u32));
    REQUIRE(nut::size_of<size_of_test_struct>::value == sizeof(size_of_test_struct));
  }

  SECTION("array_byte_size") {
    REQUIRE(nut::array_byte_size(array_byte_size_test_array)
      == nut::size_of<nut::i32>::value * array_byte_size_test_array.size());
  }

  SECTION("toggle bool", "[toggle-bool]") {
    REQUIRE(nut::toggle_bool(toggle_bool_test_bool) == true);
    REQUIRE(toggle_bool_test_bool == true);
  }

  SECTION("to_underlying" "[to-underlying]") {
    REQUIRE(nut::to_underlying(underlying_test_enum::a) == static_cast<nut::u32>(underlying_test_enum::a));
    REQUIRE(std::is_same_v<nut::u32, decltype(nut::to_underlying(underlying_test_enum::a))>);
  }
}

TEST_CASE("id", "[id]") {
  const auto id1 = nut::id("nut", "id1", false);
  const auto idt1 = nut::id("nut", "idt1", true);
  const auto id2 = nut::id::of("nut", "id2");
  const auto idt2 = nut::id::tag_of("nut", "idt2");
  const auto id3 = nut::id::of("nut", "id3/sub");
  const auto idt3 = nut::id::tag_of("nut", "idt3/sub");
  
  SECTION("base") {
    REQUIRE(id1.ns() == "nut");
    REQUIRE(id2.ns() == "nut");
    REQUIRE(id3.ns() == "nut");

    REQUIRE(id1.path() == "id1");
    REQUIRE(id2.path() == "id2");
    REQUIRE(id3.path() == "id3/sub");

    REQUIRE(id1.tag() == false);
    REQUIRE(id2.tag() == false);
    REQUIRE(id3.tag() == false);

    REQUIRE(id1.full() == "nut:id1");
    REQUIRE(id2.full() == "nut:id2");
    REQUIRE(id3.full() == "nut:id3/sub");

    REQUIRE(id1.as_filepath() == "nut/id1");
    REQUIRE(id2.as_filepath() == "nut/id2");
    REQUIRE(id3.as_filepath() == "nut/id3/sub");
  }

  SECTION("tags") {
    REQUIRE(idt1.ns() == "nut");
    REQUIRE(idt2.ns() == "nut");
    REQUIRE(idt3.ns() == "nut");

    REQUIRE(idt1.path() == "idt1");
    REQUIRE(idt2.path() == "idt2");
    REQUIRE(idt3.path() == "idt3/sub");

    REQUIRE(idt1.tag() == true);
    REQUIRE(idt2.tag() == true);
    REQUIRE(idt3.tag() == true);

    REQUIRE(idt1.full() == "#nut:idt1");
    REQUIRE(idt2.full() == "#nut:idt2");
    REQUIRE(idt3.full() == "#nut:idt3/sub");

    REQUIRE(idt1.as_filepath() == "nut/idt1");
    REQUIRE(idt2.as_filepath() == "nut/idt2");
    REQUIRE(idt3.as_filepath() == "nut/idt3/sub");
  }
}