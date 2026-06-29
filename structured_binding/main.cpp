#include <print>
#include <utility>
using std::println;
int main() {
  std::pair<int, int> ipair = {1, 2};
  println("ipair.x = {}, ipair.y = {}", ipair.first, ipair.second);
  ipair.second++;
  println("ipair.x = {}, ipair.y = {}", ipair.first, ipair.second);
  auto &[x, y] = ipair;
  y++;
  println("ipair.x = {}, ipair.y = {}", ipair.first, ipair.second);
  println("x = {}, y = {}", x, y);
  ipair.second++;
  println("x = {}, y = {}", x, y);

  return 0;
}
