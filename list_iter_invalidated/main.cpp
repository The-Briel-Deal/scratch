#include <cassert>
#include <list>
#include <print>

int main() {

  std::list<int> num_list = {1, 5, 129, -3};
  std::list<int>::const_iterator second_elem = next(num_list.begin());
  assert(*second_elem == 5);
  std::println("second_elem = {}", *second_elem);
  assert(*next(second_elem) == 129);
  std::println("second_elem = {}", *next(second_elem));

  num_list.insert(next(second_elem), 32);


  assert(*second_elem == 5);
  std::println("second_elem = {}", *second_elem);
  assert(*next(second_elem) == 32);
  std::println("second_elem = {}", *next(second_elem));
  return 0;
}
