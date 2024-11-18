/**
 *  \author  Olivia Quinet
 */

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <random>

#include "shape2ti.hpp"

#include "generate_shapes.hpp"
#include "get_sum.hpp"

SCENARIO("I can create instances with type-erasure with vtable+ti") {
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

template <typename Tp> using VISITOR_FCT1 = double (*)(Tp x);
template <typename Tp> using VISITOR_FCT2 = std::function<double(Tp x)>;

SCENARIO("I can benchmark type-erasure with vtable+ti") {
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
  BENCHMARK("get_sum_area") { return get_sum_area(); };
  BENCHMARK("get_sum_perimeter") { return get_sum_perimeter(); };

  /*WHEN("Using visitor 1") {
    using Visitor1 = VISITOR_STRATEGY::visitor<
        VISITOR_FCT1, const shape::Square &, const shape::Circle &,
        const shape::Rectangle &, const shape::RightTriangle &,
        const shape::Rhombus &>;
    auto areavisitor1 = Visitor1::create( //
        [](const auto &x) { return area(x); });
    auto perimetervisitor1 = Visitor1::create( //
        [](const auto &x) { return perimeter(x); });

    REQUIRE(get_sum_area() == get_sumvisitor(areavisitor1));
    REQUIRE(get_sum_perimeter() == get_sumvisitor(perimetervisitor1));

    BENCHMARK("get_sum_area with visitor+ti (fct ptr)") {
      return get_sumvisitor(areavisitor1);
    };
    BENCHMARK("get_sum_perimeter with visitor+ti (fct ptr)") {
      return get_sumvisitor(perimetervisitor1);
    };
  }

  WHEN("Using visitor 2") {
    using Visitor2 = VISITOR_STRATEGY::visitor<
        VISITOR_FCT2, const shape::Square &, const shape::Circle &,
        const shape::Rectangle &, const shape::RightTriangle &,
        const shape::Rhombus &>;
    auto areavisitor2 = Visitor2::create( //
        [](const auto &x) { return area(x); });
    auto perimetervisitor2 =
        Visitor2::create([](const auto &x) { return perimeter(x); });
    // auto perimetervisitor2 = Visitor2::create(ComputePerimeter{});

    REQUIRE(get_sum_area() == get_sumvisitor(areavisitor2));
    REQUIRE(get_sum_perimeter() == get_sumvisitor(perimetervisitor2));

    BENCHMARK("get_sum_area with visitor+ti (std::function)") {
      return get_sumvisitor(areavisitor2);
    };
    BENCHMARK("get_sum_perimeter with visitor+ti (std::function)") {
      return get_sumvisitor(perimetervisitor2);
    };
  }*/

  WHEN("Using visitor 3") {
    auto areavisitor3 = te2::visitor::create_ti<
        const shape::Square &, const shape::Circle &, const shape::Rectangle &,
        const shape::RightTriangle &, const shape::Rhombus &>(
        [](const auto &x) { return area(x); });
    auto perimetervisitor3 = te2::visitor::create_ti<
        const shape::Square &, const shape::Circle &, const shape::Rectangle &,
        const shape::RightTriangle &, const shape::Rhombus &>(
        [](const auto &x) { return perimeter(x); });

    REQUIRE(get_sum_area() == get_sumvisitor(areavisitor3));
    REQUIRE(get_sum_perimeter() == get_sumvisitor(perimetervisitor3));

    BENCHMARK("get_sum_area with visitor+ti (3)") {
      return get_sumvisitor(areavisitor3);
    };
    BENCHMARK("get_sum_perimeter with visitor+ti (3)") {
      return get_sumvisitor(perimetervisitor3);
    };
  }
}
