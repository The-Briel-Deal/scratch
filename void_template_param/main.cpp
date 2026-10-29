#include <concepts>
#include <print>
#include <type_traits>
template <class T>
concept IsVoid =
    requires { typename std::enable_if<std::same_as<T, void>, void>::type; };

template <class T>
concept IsNotVoid =
    requires { typename std::enable_if<!std::same_as<T, void>, void>::type; };

template <typename T1, typename T2> struct Foo {
  template <typename FT1, typename FT2>
  void f(FT1 a, FT2 b)
    requires IsNotVoid<FT2> && std::same_as<FT2, T2>
  {
    std::println("f(T1, T2)");
  }
  template <typename FT1 = T1, typename FT2 = T2>
  void f(FT1 a)
    requires IsVoid<FT2> && std::same_as<FT2, T2>
  {
    std::println("f(T1)");
  }
};

int main() {
  Foo<int, void> foo{};
  foo.f(1);
  Foo<int, float> bar{};
  bar.f(1, 1.0f);
}
