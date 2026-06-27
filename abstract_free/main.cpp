#include <print>

struct abstract_type {
  virtual void foo() = 0;
  virtual ~abstract_type() {
    std::println("abstract_type::~abstract_type() called");
  };
};

struct concrete_type : abstract_type {
  void foo() override { std::println("concrete_type::foo() called"); }
  ~concrete_type() override {
    std::println("concrete_type::~concrete_type() called");
  }
  int a;
};

int main() {
  concrete_type *conc = new concrete_type;
  abstract_type *abs = conc;
  abs->foo();
  delete conc;

  return 0;
}
