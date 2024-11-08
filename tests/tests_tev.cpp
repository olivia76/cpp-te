/**
 *  \author  Olivia Quinet
 */

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include <random>

#include "te/default_visitor.hpp"
#include "te/te.hpp"

namespace pipeline_tev {

struct Visitor1;
struct Visitor2;

using PIMPL_STRATEGY = te::pimpl::unique_ptr_strategy;

struct ShapeConcept {
  using PIMPL = PIMPL_STRATEGY::PIMPL<ShapeConcept>;

  virtual ~ShapeConcept() = default;

  virtual double do_area() const = 0;
  virtual double do_perimeter() const = 0;

  virtual double accept_visitor(const Visitor1 &_visitor) const = 0;
  virtual double accept_visitor(const Visitor2 &_visitor) const = 0;

  virtual PIMPL clone() const = 0;

  template <typename Tp> struct model;
};

template <typename Tp> struct ShapeConcept::model : public ShapeConcept {
  template <typename Vp>
  explicit model(Vp &&vp) : value(std::forward<Vp>(vp)) {}

  double do_area() const final { return area(value); }
  double do_perimeter() const final { return perimeter(value); }

  double accept_visitor(const Visitor1 &_visitor) const final {
    return _visitor(value);
  }
  double accept_visitor(const Visitor2 &_visitor) const final {
    return _visitor(value);
  }

  PIMPL clone() const final { return PIMPL_STRATEGY::make_pimpl_clone(*this); }

  Tp value;
};

using VISITOR_STRATEGY = te::visitor::default_visitor_strategy;
using BASE = te::base<ShapeConcept, VISITOR_STRATEGY, PIMPL_STRATEGY>;

struct Shape : public BASE {
  template <typename Vp> explicit Shape(Vp &&vp) : BASE(std::forward<Vp>(vp)) {}

  auto area() const { return pimpl()->do_area(); }
  auto perimeter() const { return pimpl()->do_perimeter(); }
};

} // namespace pipeline_tev

#include "circle.hpp"
#include "rectangle.hpp"
#include "rhombus.hpp"
#include "square.hpp"
#include "triangle.hpp"

namespace pipeline_tev {

template <typename Tp> using VISITOR_FCT1 = double (*)(Tp _x);
struct Visitor1 : public VISITOR_STRATEGY::visitor<
                      Visitor1, VISITOR_FCT1, const shape::Square &,
                      const shape::Circle &, const shape::Rectangle &,
                      const shape::RightTriangle &, const shape::Rhombus &> {};

template <typename Tp> using VISITOR_FCT2 = std::function<double(Tp _x)>;
struct Visitor2 : public VISITOR_STRATEGY::visitor<
                      Visitor2, VISITOR_FCT2, const shape::Square &,
                      const shape::Circle &, const shape::Rectangle &,
                      const shape::RightTriangle &, const shape::Rhombus &> {};

} // namespace pipeline_tev

#include "generate_shapes.hpp"
#include "get_sum.hpp"

SCENARIO("I can create instances with type-erasure") {
  auto shape1 = pipeline_tev::Shape(shape::Square(1));
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

SCENARIO("I can benchmark type-erasure") {
  auto shapes = generate_shapes<pipeline_tev::Shape>();
  auto get_sum_area = [&shapes]() {
    return get_sum(shapes, [](const auto &shape) { return shape.area(); });
  };
  auto get_sum_perimeter = [&shapes]() {
    return get_sum(shapes, [](const auto &shape) { return shape.perimeter(); });
  };
  auto get_sum_visitor = [&shapes](auto &&visitor) {
    return get_sum(
        shapes, [&](const auto &shape) { return te::visit(visitor, shape); });
  };
  auto area_visitor1 = pipeline_tev::Visitor1::create( //
      [](const auto &x) { return area(x); });
  auto perimeter_visitor1 = pipeline_tev::Visitor1::create( //
      [](const auto &x) { return perimeter(x); });
  auto area_visitor2 = pipeline_tev::Visitor2::create( //
      [](const auto &x) { return area(x); });
  auto perimeter_visitor2 = pipeline_tev::Visitor1::create( //
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
