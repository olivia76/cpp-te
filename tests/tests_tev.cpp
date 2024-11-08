/**
 *  \author  Olivia Quinet
 */

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <random>

#include "shape.hpp"
#include "shape_visitor.hpp"

#include "generate_shapes.hpp"
#include "get_sum.hpp"

SCENARIO("I can create instances with type-erasure") {
  using namespace pipeline_tev;

  auto shape1 = Shape(shape::Square(1));
  REQUIRE(shape1.area() == 1);
  REQUIRE(shape1.perimeter() == 4);
  auto shape2 = std::move(shape1);
  REQUIRE(shape2.area() == 1);
  REQUIRE(shape2.perimeter() == 4);
  auto shape3 = shape2;
  REQUIRE(shape3.area() == 1);
  REQUIRE(shape3.perimeter() == 4);
  shape2 = shape3;
  shape3 = Shape(shape::RightTriangle(4, 3));
  REQUIRE(shape2.area() == 1);
  REQUIRE(shape2.perimeter() == 4);
  REQUIRE(shape3.area() == 6);
  REQUIRE(shape3.perimeter() == 12);
  shape1 = shape2;
  shape2 = Shape(shape::Rectangle(2, 3));
  REQUIRE(shape1.area() == 1);
  REQUIRE(shape1.perimeter() == 4);
  REQUIRE(shape2.area() == 6);
  REQUIRE(shape2.perimeter() == 10);
  REQUIRE(shape3.area() == 6);
  REQUIRE(shape3.perimeter() == 12);
}

SCENARIO("I can benchmark type-erasure") {
  using namespace pipeline_tev;

  auto shapes = generate_shapes<Shape>();
  auto get_sum_area = [&shapes]() {
    return get_sum(shapes, [](const auto &shape) { return shape.area(); });
  };
  auto get_sum_perimeter = [&shapes]() {
    return get_sum(shapes, [](const auto &shape) { return shape.perimeter(); });
  };
  auto get_sum_visitor = [&shapes](auto &&visitor) {
    return get_sum(shapes,
                   [&](const auto &shape) { return visit(visitor, shape); });
  };
  auto area_visitor1 = Visitor1::create( //
      [](const auto &x) { return area(x); });
  auto perimeter_visitor1 = Visitor1::create( //
      [](const auto &x) { return perimeter(x); });
  auto area_visitor2 = Visitor2::create( //
      [](const auto &x) { return area(x); });
  auto perimeter_visitor2 = Visitor1::create( //
      [](const auto &x) { return perimeter(x); });

  REQUIRE(get_sum_area() == get_sum_visitor(area_visitor1));
  REQUIRE(get_sum_area() == get_sum_visitor(area_visitor2));
  REQUIRE(get_sum_perimeter() == get_sum_visitor(perimeter_visitor1));
  REQUIRE(get_sum_perimeter() == get_sum_visitor(perimeter_visitor2));

  BENCHMARK("get_sum_area") { return get_sum_area(); };
  BENCHMARK("get_sum_perimeter") { return get_sum_perimeter(); };

  BENCHMARK("get_sum_area with visitor1") { //
    return get_sum_visitor(area_visitor1);
  };
  BENCHMARK("get_sum_perimeter with visitor1") { //
    return get_sum_visitor(perimeter_visitor1);
  };
  BENCHMARK("get_sum_area with visitor2") { //
    return get_sum_visitor(area_visitor2);
  };
  BENCHMARK("get_sum_perimeter with visitor2") { //
    return get_sum_visitor(perimeter_visitor2);
  };
}
