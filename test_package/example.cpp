#include "shape.hpp"
#include "shape2.hpp"

#include "circle.hpp"
#include "rectangle.hpp"
#include "rhombus.hpp"
#include "square.hpp"
#include "triangle.hpp"

#include <spdlog/spdlog.h>

namespace pipeline_tev {

template <typename _Tp> using VISITOR_FCT = std::function<double(_Tp _x)>;
struct Visitor : public VISITOR_STRATEGY::visitor<
                     Visitor, VISITOR_FCT, const shape::Square &,
                     const shape::Circle &, const shape::Rectangle &,
                     const shape::RightTriangle &, const shape::Rhombus &> {};

} // namespace pipeline_tev

namespace pipeline_tev2 {

template <typename _Tp> using VISITOR_FCT = std::function<double(_Tp _x)>;
struct Visitor : public VISITOR_STRATEGY::visitor<
                     Visitor, VISITOR_FCT, const shape::Square &,
                     const shape::Circle &, const shape::Rectangle &,
                     const shape::RightTriangle &, const shape::Rhombus &> {};
} // namespace pipeline_tev2

int main() {
  {
    using namespace pipeline_tev;

    auto shape1 = Shape(shape::Circle(1));
    auto area_visitor = Visitor::create( //
        [](const auto &x) { return area(x); });
    auto perimeter_visitor = Visitor::create( //
        [](const auto &x) { return perimeter(x); });

    spdlog::info("area: {} with visitor: {}", shape1.area(),
                 visit(area_visitor, shape1));
    spdlog::info("perimeter: {} with visitor: {}", shape1.perimeter(),
                 visit(perimeter_visitor, shape1));
  }
  {
    using namespace pipeline_tev2;

    auto shape1 = Shape(shape::Circle(1));
    auto area_visitor = Visitor::create( //
        [](const auto &x) { return area(x); });
    auto perimeter_visitor = Visitor::create( //
        [](const auto &x) { return perimeter(x); });

    spdlog::info("area: {} with visitor: {}", shape1.area(),
                 visit(area_visitor, shape1));
    spdlog::info("perimeter: {} with visitor: {}", shape1.perimeter(),
                 visit(perimeter_visitor, shape1));
  }

  return 0;
}
