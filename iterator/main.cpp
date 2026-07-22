#include <iterator>
#include <print>

class NumberRange {

public:
  struct NumberIter {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = int;
    using value_type = int;
    using reference = int &;
    using pointer = int *;

    int num;
    bool operator==(const NumberIter &num_iter) const {
      return this->num == num_iter.num;
    }
    bool operator!=(const NumberIter &num_iter) const {
      return this->num != num_iter.num;
    }
    NumberIter &operator++() {
      this->num++;
      return *this;
    }
    NumberIter operator++(int) {
      NumberIter tmp = *this;
      this->num++;
      return tmp;
    }

    const int &operator*() const { return this->num; }
  };

  NumberIter begin() { return {0}; }
  NumberIter end() { return {5}; }
};
static_assert(std::forward_iterator<NumberRange::NumberIter>);
static_assert(std::input_iterator<NumberRange::NumberIter>);

int main() {
  NumberRange num_iter;
  for (auto num : num_iter) {
    std::println("{}", num);
  }
}
