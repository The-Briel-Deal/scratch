#include <print>
#include <type_traits>

struct float_or_int_t {
  int int_num;
  float float_num;
};
template <typename T> T _foo(float_or_int_t &val);
template <> int &_foo<int &>(float_or_int_t &val) { return val.int_num; }
template <> float &_foo<float &>(float_or_int_t &val) { return val.float_num; }
template <typename T> T foo() {
  static float_or_int_t val = {.int_num = 2, .float_num = 6.7f};
  return _foo<T>(val);
}

int main() {
  {
    int &i = foo<int &>();
    float &f = foo<float &>();
    std::println("i = {}, f = {}", i, f);
    i++;
    f += 7.0f;
  }
  {
    int &i = foo<int &>();
    float &f = foo<float &>();
    std::println("i = {}, f = {}", i, f);
    i++;
    f += 7.0f;
  }
}
