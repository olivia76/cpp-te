/**
 *  \author  Olivia Quinet
 */

#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <numbers>

namespace shape {

struct Circle {
  double radius{0};

  explicit Circle(const double _radius) : radius(_radius) {}
  Circle(const Circle &) = default;
  Circle(Circle &&) = default;

  friend double area(const Circle &x) noexcept {
    return std::numbers::pi * x.radius * x.radius;
  }
  friend double perimeter(const Circle &x) noexcept {
    return 2 * std::numbers::pi * x.radius;
  };
};

}; // namespace shape

#endif
