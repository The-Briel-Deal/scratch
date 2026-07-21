#include <iostream>
#include <stdexcept>
#include <string>

class Player {
public:
  std::string Name;
};

class Party {
public:
  Party(Player A, Player B, Player C) : A{A}, B{B}, C{C} {}

  Player A, B, C;

  class Iterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Player;
    using difference_type = std::ptrdiff_t;
    using pointer = Player *;
    using reference = Player &;

    Iterator(Party *ptr = nullptr, size_t idx = 0) : party(ptr), idx(idx) {}

    Player &operator*() const {
      if (idx == 0)
        return party->A;
      if (idx == 1)
        return party->B;
      if (idx == 2)
        return party->C;
      throw std::out_of_range("Invalid index");
    }

    Player *operator->() const { return &**this; }

    Iterator &operator++() {
      ++idx;
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const Iterator &other) const {
      return party == other.party && idx == other.idx;
    }

    bool operator!=(const Iterator &other) const { return !(*this == other); }

  private:
    size_t idx;
    Party *party;
  };

  Iterator begin() { return Iterator(this, 0); }
  Iterator end() { return Iterator(this, 3); }
};

int main() {
  Party party{Player{"Anna"}, Player{"Bob"}, Player{"Cara"}};

  for (Player &p : party) {
    std::cout << p.Name << ", ";
  }

  auto it = party.begin();
  std::cout << '\n' << (it++)->Name;
  std::cout << '\n' << (it++)->Name;
  std::cout << '\n' << it->Name;
}
