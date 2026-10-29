#include <print>

template <typename T1, typename T2 = void> struct Foo {
  void f(T1 a, T2 b) { std::println("f(T1, T2)"); }
};
template <typename T1> struct Foo<T1> {
  void f(T1 a) { std::println("f(T1)"); }
};

int main() {
  Foo<int> foo{};
  foo.f(1);
  Foo<int, float> bar{};
  bar.f(1, 1.0f);
}
