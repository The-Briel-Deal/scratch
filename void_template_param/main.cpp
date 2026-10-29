#include <concepts>
#include <print>
#include <type_traits>

template <typename T1, typename T2> struct Foo {
  template <typename T3 = T1, typename T4 = T2>
  typename std::enable_if<!std::same_as<T4, void>, void>::type f(T3 a, T4 b) {
    std::println("f(T1, T2)");
  }
  template <typename T3 = T1, typename T4 = T2>
  typename std::enable_if<std::same_as<T4, void>, void>::type f(T3 a) {
    std::println("f(T1)");
  }
};

int main() {
  Foo<int, void> foo{};
  foo.f(1);
  Foo<int, float> bar{};
  bar.f(1, 1.0f);
}
