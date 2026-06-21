#include <cassert>
#include <cstddef>
#include <print>
#include <string>
#include <variant>

int main() {
  std::variant<int *, std::nullptr_t> v = nullptr;
  std::println("{}", std::holds_alternative<std::nullptr_t>(v));
  std::println("{}", std::holds_alternative<int *>(v));
  v = (int *)0;
  std::println("{}", std::holds_alternative<std::nullptr_t>(v));
  std::println("{}", std::holds_alternative<int *>(v));
}
