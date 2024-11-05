/**
 *  \author  Olivia Quinet
 */

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <random>

#include "circle.hpp"
#include "rectangle.hpp"
#include "rhombus.hpp"
#include "square.hpp"
#include "triangle.hpp"

#include <variant>

namespace pipeline_vv {

using Shape = std::variant<shape::Square, shape::Circle, shape::Rectangle,
                           shape::RightTriangle, shape::Rhombus>;

}

#include "generate_shapes.hpp"
#include "get_sum.hpp"

SCENARIO("I can benchmark std::variant+std::visit") {
  auto shapes = generate_shapes<pipeline_vv::Shape>();
  auto get_sum_visitor = [&shapes](auto &&visitor) {
    return get_sum(
        shapes, [&](const auto &shape) { return std::visit(visitor, shape); });
  };
  auto area_visitor = [](const auto &x) { return area(x); };
  auto perimeter_visitor = [](const auto &x) { return perimeter(x); };

  BENCHMARK("get_sum_area") { get_sum_visitor(area_visitor); };
  BENCHMARK("get_sum_perimeter") { get_sum_visitor(perimeter_visitor); };
}
