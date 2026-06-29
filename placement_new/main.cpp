#include <print>
using std::println;

int main() {
  int arr[10];
  arr[0] = 5;
  arr[1] = 3;
  int *foo = new (arr) int;
  int *bar = new (arr+1) int;
  println("foo = {}", *foo);
  println("bar = {}", *bar);
  return 0;
}
