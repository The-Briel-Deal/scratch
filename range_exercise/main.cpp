#include <print>
#include <ranges>
#include <vector>

namespace ranges = std::ranges;
namespace views = std::ranges::views;

int main() {
  std::vector vec{1, 2, 3, 4, 5, 6};
  auto v = vec | views::filter([](int i) { return i % 2 == 0; }) |
           views::transform([](auto e) { return e * e; });

  std::println("{}", *v.begin()); // should print 4
}
