#include "glaze/glaze.hpp"
#include <cassert>
#include <glaze/core/common.hpp>
#include <glaze/core/write.hpp>
#include <glaze/forward.hpp>
#include <print>

enum test_enum {
  TEST_ENUM_FOO,
  TEST_ENUM_BAR,
  TEST_ENUM_BAZ,
};

template <> struct glz::meta<test_enum> {
  static constexpr auto key = {"Foo", "Bar", "Baz"};
  static constexpr auto value = {TEST_ENUM_FOO, TEST_ENUM_BAR, TEST_ENUM_BAZ};
};

struct test_struct {
  int num;
  test_enum test;
};

int main() {
  test_struct val = {
      .num = 14,
      .test = TEST_ENUM_BAR,
  };

  auto ret = glz::write<glz::opts{}>(val);
  assert(ret.has_value());
  std::println("val's json str = {}", ret.value());

  return 0;
}
