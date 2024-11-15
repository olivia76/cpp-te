#ifndef SHEEP_HPP
#define SHEEP_HPP

#include <iostream>

class Sheep {
  friend void make_sound(const Sheep &x) { std::cout << "Baah" << std::endl; }
};

#endif
