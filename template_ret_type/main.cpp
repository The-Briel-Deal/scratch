#include <print>
#include <type_traits>

template <typename T> T foo() {
  static float float_num = 2.5f;
  static int integ_num = 2;
  if (std::is_same_v<T, float>) {
    return float_num;
  }
  if (std::is_same_v<T, int>) {
    return integ_num;
  }
}

int main() {
  float f = foo<float>();
  std::println("f = {}", f);
  int i = foo<int>();
  std::println("i = {}", i);
}
