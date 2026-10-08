#include <nlohmann/json.hpp>

enum test_enum {
  TEST_ENUM_FOO,
  TEST_ENUM_BAR,
  TEST_ENUM_BAZ,
};

struct test_struct {
  int num;
  test_enum test;
};

int main() {

  // I want this to be written to a json string like:
  //   ```
  //   {
  //     "num": 14,
  //     "test": "Bar"
  //   }
  //   ```
  test_struct val = {
      .num = 14,
      .test = TEST_ENUM_BAR,
  };
  return 0;
}
