/**
 *  \author  Olivia Quinet
 */

#ifndef _TRIANGLE_HPP
#define _TRIANGLE_HPP

#include <numbers>

namespace shape {

struct RightTriangle {
  double base{0}, height{0};

  explicit RightTriangle(const double _base, const double _height)
      : base(_base), height(_height) {}
  RightTriangle(const RightTriangle &) = default;
  RightTriangle(RightTriangle &&) = default;

  friend double area(const RightTriangle &x) noexcept {
    return .5 * x.base * x.height;
  }
  friend double perimeter(const RightTriangle &x) noexcept {
    double c = sqrt(x.base * x.base + x.height * x.height);
    return x.base + x.height + c;
  };
};

}; // namespace shape

#endif
