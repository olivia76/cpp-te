/**
 *  \author  Olivia Quinet
 */

#ifndef GET_SUM_HPP
#define GET_SUM_HPP

#include <numeric>

namespace {
double get_sum(const auto &shapes, auto &&fn) {
  constexpr double ZERO{0};
  return std::accumulate(
      shapes.begin(), shapes.end(), ZERO,
      [&fn](const auto &sum, const auto &shape) { return sum + fn(shape); });
}
} // namespace

#endif
