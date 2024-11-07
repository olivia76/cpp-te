#include "shape.hpp"
#include "shape2.hpp"

#include "circle.hpp"
#include "rectangle.hpp"
#include "rhombus.hpp"
#include "square.hpp"
#include "triangle.hpp"

#include "visitor.hpp"
#include "visitor2.hpp"

#include <spdlog/spdlog.h>

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
