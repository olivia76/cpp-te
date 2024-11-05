/**
 *  \author  Olivia Quinet
 */

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <random>

#include "te2/default_visitor.hpp"
#include "te2/te2.hpp"
#include "te2/typeinfo_visitor.hpp"

namespace pipeline_tev2 {

struct Visitor1;
struct Visitor2;

struct shape_vtbl {
  double (*do_area)(const void *);
  double (*do_perimeter)(const void *);
  double (*_accept_visitor1)(const void *, const Visitor1 &);
  double (*_accept_visitor2)(const void *, const Visitor2 &);

  template <typename... _Args>
  auto accept_visitor(auto *pimpl, const Visitor1 &_visitor,
                      _Args &&..._args) const {
    return (*_accept_visitor1)(pimpl, _visitor, std::forward<_Args>(_args)...);
  }
  template <typename... _Args>
  auto accept_visitor(auto *pimpl, const Visitor2 &_visitor,
                      _Args &&..._args) const {
    return (*_accept_visitor2)(pimpl, _visitor, std::forward<_Args>(_args)...);
  }

  template <typename VP> static shape_vtbl create() {
    return {
        .do_area = [](const void *p) { return area(VP::value(p)); },
        .do_perimeter = [](const void *p) { return perimeter(VP::value(p)); },
        ._accept_visitor1 =
            [](const void *p, const Visitor1 &_visitor) {
              return _visitor(VP::value(p));
            },
        ._accept_visitor2 =
            [](const void *p, const Visitor2 &_visitor) {
              return _visitor(VP::value(p));
            },
    };
  }
};

// using VISITOR_STRATEGY = te2::visitor::default_visitor_strategy;
using VISITOR_STRATEGY = te2::visitor::type_info_visitor_strategy;
using BASE = te2::base<shape_vtbl, VISITOR_STRATEGY>;
struct Shape : public BASE {
  template <typename _Vp>
  explicit Shape(_Vp &&_value) : BASE(std::forward<_Vp>(_value)) {}

  auto area() const { return vtbl().do_area(pimpl()); }
  auto perimeter() const { return vtbl().do_perimeter(pimpl()); }
};
} // namespace pipeline_tev2

#include "circle.hpp"
#include "rectangle.hpp"
#include "rhombus.hpp"
#include "square.hpp"
#include "triangle.hpp"

namespace pipeline_tev2 {

template <typename _Tp> using VISITOR_FCT1 = double (*)(_Tp _x);
struct Visitor1 : public VISITOR_STRATEGY::visitor<
                      Visitor1, VISITOR_FCT1, const shape::Square &,
                      const shape::Circle &, const shape::Rectangle &,
                      const shape::RightTriangle &, const shape::Rhombus &> {};

template <typename _Tp> using VISITOR_FCT2 = std::function<double(_Tp _x)>;
struct Visitor2 : public VISITOR_STRATEGY::visitor<
                      Visitor2, VISITOR_FCT2, const shape::Square &,
                      const shape::Circle &, const shape::Rectangle &,
                      const shape::RightTriangle &, const shape::Rhombus &> {};

} // namespace pipeline_tev2

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
  auto get_sum_visitor = [&shapes](auto &&visitor) {
    double sum{};
    for (const auto &shape : shapes)
      sum += te2::visit(visitor, shape);
    return sum;
  };
  auto area_visitor1 =
      pipeline_tev2::Visitor1::create([](const auto &x) { return area(x); });
  auto perimeter_visitor1 = pipeline_tev2::Visitor1::create(
      [](const auto &x) { return perimeter(x); });
  auto area_visitor2 =
      pipeline_tev2::Visitor2::create([](const auto &x) { return area(x); });
  auto perimeter_visitor2 = pipeline_tev2::Visitor2::create(
      [](const auto &x) { return perimeter(x); });

  REQUIRE(get_sum_area() == get_sum_visitor(area_visitor1));
  REQUIRE(get_sum_area() == get_sum_visitor(area_visitor2));
  REQUIRE(get_sum_perimeter() == get_sum_visitor(perimeter_visitor1));
  REQUIRE(get_sum_perimeter() == get_sum_visitor(perimeter_visitor2));

  BENCHMARK("get_sum_area") { get_sum_area(); };
  BENCHMARK("get_sum_perimeter") { get_sum_perimeter(); };

  BENCHMARK("get_sum_area with visitor1") { get_sum_visitor(area_visitor1); };
  BENCHMARK("get_sum_perimeter with visitor1") {
    get_sum_visitor(perimeter_visitor1);
  };
  BENCHMARK("get_sum_area with visitor2") { get_sum_visitor(area_visitor2); };
  BENCHMARK("get_sum_perimeter with visitor2") {
    get_sum_visitor(perimeter_visitor2);
  };
}
