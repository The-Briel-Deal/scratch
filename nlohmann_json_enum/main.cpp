#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <print>
using nlohmann::json;

enum test_enum {
  TEST_ENUM_FOO,
  TEST_ENUM_BAR,
  TEST_ENUM_BAZ,
};
std::string to_string(test_enum e) {
  switch (e) {
  case TEST_ENUM_FOO:
    return "Foo";
  case TEST_ENUM_BAR:
    return "Bar";
  case TEST_ENUM_BAZ:
    return "Baz";
  }
  return "Unknown";
}

struct test_struct {
  int num;
  test_enum test;
};
void to_json(json &j, const test_struct &p) {
  j = json{{"num", p.num}, {"test", to_string(p.test)}};
}

// void from_json(const json &j, test_struct &p) {
//   j.at("name").get_to(p.name);
//   j.at("address").get_to(p.address);
//   j.at("age").get_to(p.age);
// }

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
  json j = val;
  std::println("json = {}", j.dump());
  return 0;
}
