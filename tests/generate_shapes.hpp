/**
 *  \author  Olivia Quinet
 */

#ifndef GENERATE_SHAPES_HPP
#define GENERATE_SHAPES_HPP

#include <vector>

#include "circle.hpp"
#include "rectangle.hpp"
#include "rhombus.hpp"
#include "square.hpp"
#include "triangle.hpp"

namespace {

template <typename SHAPE>
auto generate_shapes(const size_t n = 10'000'000, const int nshapes = 5) {
  std::minstd_rand rand(std::random_device{}());
  std::uniform_int_distribution<> ishape(0, nshapes - 1);
  std::uniform_int_distribution<> dist(1, 1000);

  std::vector<SHAPE> shapes;
  shapes.reserve(n);
  for (size_t i = 0; i < n; ++i) {
    switch (ishape(rand)) {
    case 0: {
      double r = dist(rand);
      shapes.emplace_back(shape::Circle(r));
    } break;
    case 1: {
      double s = dist(rand);
      shapes.emplace_back(shape::Square(s));
    } break;
    case 2: {
      double l = dist(rand);
      double w = dist(rand);
      shapes.emplace_back(shape::Rectangle(l, w));
    } break;
    case 3: {
      double b = dist(rand);
      double h = dist(rand);
      shapes.emplace_back(shape::RightTriangle(b, h));
    } break;
    case 4: {
      double b = dist(rand);
      double h = dist(rand);
      shapes.emplace_back(shape::Rhombus(b, h));
    } break;
    default:
      throw "";
    }
  }
  return shapes;
};
} // namespace

#endif
