/**
 *  \author  Olivia Quinet
 */

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <random>

#include "shape2.hpp"
#include "shape2_visitor.hpp"

#include "generate_shapes.hpp"
#include "get_sum.hpp"

SCENARIO("I can create instances with type-erasure with vtable") {
  auto shape1 = pipeline_tev2::Shape(shape::Square(1));
  REQUIRE(shape1.area() == 1);
  REQUIRE(shape1.perimeter() == 4);
  auto shape2 = std::move(shape1);
  REQUIRE(shape2.area() == 1);
  REQUIRE(shape2.perimeter() == 4);
  auto shape3 = shape2;
  REQUIRE(shape3.area() == 1);
  REQUIRE(shape3.perimeter() == 4);
  shape2 = shape3;
  REQUIRE(shape2.area() == 1);
  REQUIRE(shape2.perimeter() == 4);
  REQUIRE(shape3.area() == 1);
  REQUIRE(shape3.perimeter() == 4);
  shape1 = shape2;
  REQUIRE(shape1.area() == 1);
  REQUIRE(shape1.perimeter() == 4);
  REQUIRE(shape2.area() == 1);
  REQUIRE(shape2.perimeter() == 4);
  REQUIRE(shape3.area() == 1);
  REQUIRE(shape3.perimeter() == 4);
}

SCENARIO("I can benchmark type-erasure with vtable") {
  auto shapes = generate_shapes<pipeline_tev2::Shape>();
  auto get_sum_area = [&shapes]() {
    double sum{};
    for (const auto &shape : shapes)
      sum += shape.area();
    return sum;
  };
  auto get_sum_perimeter = [&shapes]() {
    double sum{};
    for (const auto &shape : shapes)
      sum += shape.perimeter();
    return sum;
  };
  auto get_sumvisitor = [&shapes](auto &&visitor) {
    double sum{};
    for (const auto &shape : shapes)
      sum += te2::visit(visitor, shape);
    return sum;
  };
  auto areavisitor1 =
      pipeline_tev2::Visitor1::create([](const auto &x) { return area(x); });
  auto perimetervisitor1 = pipeline_tev2::Visitor1::create(
      [](const auto &x) { return perimeter(x); });
  auto areavisitor2 =
      pipeline_tev2::Visitor2::create([](const auto &x) { return area(x); });
  auto perimetervisitor2 = pipeline_tev2::Visitor2::create(
      [](const auto &x) { return perimeter(x); });

  REQUIRE(get_sum_area() == get_sumvisitor(areavisitor1));
  REQUIRE(get_sum_area() == get_sumvisitor(areavisitor2));
  REQUIRE(get_sum_perimeter() == get_sumvisitor(perimetervisitor1));
  REQUIRE(get_sum_perimeter() == get_sumvisitor(perimetervisitor2));

  BENCHMARK("get_sum_area") { return get_sum_area(); };
  BENCHMARK("get_sum_perimeter") { return get_sum_perimeter(); };

  BENCHMARK("get_sum_area with visitor1") {
    return get_sumvisitor(areavisitor1);
  };
  BENCHMARK("get_sum_perimeter with visitor1") {
    return get_sumvisitor(perimetervisitor1);
  };
  BENCHMARK("get_sum_area with visitor2") {
    return get_sumvisitor(areavisitor2);
  };
  BENCHMARK("get_sum_perimeter with visitor2") {
    return get_sumvisitor(perimetervisitor2);
  };
}
