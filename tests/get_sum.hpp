/**
 *  \author  Olivia Quinet
 */

#ifndef _GET_SUM_HPP_
#define _GET_SUM_HPP_

#include <numeric>

namespace {
double get_sum(const auto &_shape, auto &&_fn) {
  constexpr double ZERO{0};
  return std::accumulate(
      _shape.begin(), _shape.end(), ZERO,
      [_fn](const auto &sum, const auto &shape) { return sum + _fn(shape); });
}
} // namespace

#endif
