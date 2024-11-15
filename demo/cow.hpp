#ifndef COW_HPP
#define COW_HPP

#include <iostream>

class Cow {
  friend void make_sound(const Cow &x) { std::cout << "Mooo" << std::endl; }
};

#endif
