/**
 *  \author  Olivia Quinet
 */

#ifndef _RHOMBUS_HPP
#define _RHOMBUS_HPP

#include <cmath>
#include <numbers>

namespace shape {

struct Rhombus {
  double diag1{0}, diag2{0};

  explicit Rhombus(const double _diag1, const double _diag2)
      : diag1(_diag1), diag2(_diag2) {}
  Rhombus(const Rhombus &) = default;
  Rhombus(Rhombus &&) = default;

  friend double area(const Rhombus &x) noexcept {
    return .5 * x.diag1 * x.diag2;
  }
  friend double perimeter(const Rhombus &x) noexcept {
    double c = sqrt(x.diag1 * x.diag1 + x.diag2 * x.diag2);
    return c;
  };
};

}; // namespace shape

#endif
