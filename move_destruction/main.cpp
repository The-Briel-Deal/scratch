#include <cstdio>
#include <utility>
class foo {
public:
  foo(int num) {
    this->num = num;
    printf("foo %d constructor called\n", this->num);
  }
  foo(foo &&other) { printf("foo %d move constructor called\n", this->num); }
  foo &operator=(foo &&other) {
    printf("foo %d move assignment operator called\n", this->num);
    return *this;
  }
  ~foo() { printf("foo %d destructor called\n", this->num); }

private:
  int num;
};

int main() {
  foo foo_var1(1);
  foo foo_var2(2);
  foo foo_var3(3);
  foo_var1 = std::move(foo_var2);
  foo_var1 = std::move(foo_var3);
  printf("end\n");
  return 0;
}
