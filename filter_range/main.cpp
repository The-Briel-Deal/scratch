#include <functional>
#include <print>
#include <ranges>
#include <vector>
class A {
public:
  virtual void takes_view(std::ranges::input_range auto foo) {
    std::println("A::takes_view({})", foo);
  }
};
class B : A {
public:
  virtual void takes_view(std::ranges::input_range auto foo) override {
    std::println("B::takes_view({})", foo);
  }
};

int main() {
  std::vector<int> num_vec = {1, 4, 2, 3};
  std::ranges::filter_view filtered(num_vec,
                                    [](int num) { return num % 2 == 1; });
  std::ranges::subrange<
    std::ranges::filter_view<std::ranges::ref_view<std::vector<int>>,
                             std::function<bool(int)>>::_Iterator<false>,
    std::ranges::filter_view<std::ranges::ref_view<std::vector<int>>,
                             (lambda)>::_Iterator<false>> sr = std::ranges::subrange(filtered.begin(), filtered.end());
  B b{};
  b.takes_view(filtered);
}
