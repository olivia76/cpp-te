/**
 *  \author  Olivia Quinet
 */

#ifndef SQUARE_HPP
#define SQUARE_HPP

#include <numbers>

namespace shape {

struct Square {
  double side{0};

  explicit Square(const double _side) : side(_side) {}
  Square(const Square &) = default;
  Square(Square &&) = default;

  friend double area(const Square &x) noexcept { return x.side * x.side; }
  friend double perimeter(const Square &x) noexcept { return 4 * x.side; };
};

}; // namespace shape

#endif
