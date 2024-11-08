/**
 *  \author  Olivia Quinet
 */

#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include <numbers>

namespace shape {

struct Rectangle {
  double length{0}, width{0};

  explicit Rectangle(const double _length, const double _width)
      : length(_length), width(_width) {}
  Rectangle(const Rectangle &) = default;
  Rectangle(Rectangle &&) = default;

  friend double area(const Rectangle &x) noexcept { return x.length * x.width; }
  friend double perimeter(const Rectangle &x) noexcept {
    return 2 * (x.length + x.width);
  };
};

}; // namespace shape

#endif
