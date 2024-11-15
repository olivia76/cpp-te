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
  using namespace pipeline_tev2;

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

/*namespace {
  struct ComputePerimeter {
    double operator()(const shape::Circle&x) const { return perimeter(x); }
    double operator()(const shape::Square&x) const { return perimeter(x); }
    double operator()(const shape::Rectangle&x) const { return perimeter(x); }
    double operator()(const shape::RightTriangle&x) const { return perimeter(x);
} double operator()(const shape::Rhombus&x) const { return perimeter(x); }
  };
}*/

SCENARIO("I can benchmark type-erasure with vtable") {
  using namespace pipeline_tev2;

  auto shapes = generate_shapes<Shape>();
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
      sum += visit(visitor, shape);
    return sum;
  };
  auto areavisitor1 = Visitor1::create( //
      [](const auto &x) { return area(x); });
  auto perimetervisitor1 = Visitor1::create( //
      [](const auto &x) { return perimeter(x); });
  auto areavisitor2 = Visitor2::create( //
      [](const auto &x) { return area(x); });
  auto perimetervisitor2 =
      Visitor2::create([](const auto &x) { return perimeter(x); });

  auto get_sumvisitor3 = [&shapes](auto &&visitor) {
    double sum{};
    for (const auto &shape : shapes) {
      visit(visitor, shape, &sum);
    }
    return sum;
  };

  auto areavisitor3 = Visitor3::create( //
      [](const auto &x, void *result) {
        *static_cast<double *>(result) += area(x);
      });
  auto perimetervisitor3 = Visitor3::create([](const auto &x, void *result) {
    *static_cast<double *>(result) += perimeter(x);
  });
  // auto perimetervisitor2 = Visitor2::create(ComputePerimeter{});

  REQUIRE(get_sum_area() == get_sumvisitor(areavisitor1));
  REQUIRE(get_sum_area() == get_sumvisitor(areavisitor2));
  REQUIRE(get_sum_area() == get_sumvisitor3(areavisitor3));
  REQUIRE(get_sum_perimeter() == get_sumvisitor(perimetervisitor1));
  REQUIRE(get_sum_perimeter() == get_sumvisitor(perimetervisitor2));
  REQUIRE(get_sum_perimeter() == get_sumvisitor3(perimetervisitor3));

  BENCHMARK("get_sum_area") { return get_sum_area(); };
  BENCHMARK("get_sum_perimeter") { return get_sum_perimeter(); };

  BENCHMARK("get_sum_area with visitor (fct ptr)") {
    return get_sumvisitor(areavisitor1);
  };
  BENCHMARK("get_sum_perimeter with visitor (fct ptr)") {
    return get_sumvisitor(perimetervisitor1);
  };
  BENCHMARK("get_sum_area with visitor (std::function)") {
    return get_sumvisitor(areavisitor2);
  };
  BENCHMARK("get_sum_perimeter with visitor (std::function)") {
    return get_sumvisitor(perimetervisitor2);
  };
  BENCHMARK("get_sum_area with visitor3 (std::function)") {
    return get_sumvisitor3(areavisitor3);
  };
  BENCHMARK("get_sum_perimeter with visitor3 (std::function)") {
    return get_sumvisitor3(perimetervisitor3);
  };
}
