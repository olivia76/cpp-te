#include "shape.hpp"
#include "shape_visitor.hpp"

#include "shape2.hpp"
#include "shape2_visitor.hpp"

#include "circle.hpp"
#include "rectangle.hpp"
#include "rhombus.hpp"
#include "square.hpp"
#include "triangle.hpp"

#include "opengldrawer.hpp"

#include <spdlog/spdlog.h>

#include <vector>

int main() {
  using namespace pipeline_tev;
  // using namespace pipeline_tev2;

  using Shapes = std::vector<Shape>;

  auto shapes =
      Shapes{shape::Circle(1), shape::Square(2), shape::Rectangle(3, 4),
             shape::RightTriangle(5, 6), shape::Rhombus(7, 8)};

  // Accessing interface methods
  for (const auto &shape : shapes) {
    spdlog::info("Area: {}", shape.area());
    spdlog::info("Perimeter: {}", shape.perimeter());
  }

  // Using the visitor to draw shapes
  {
    OpenGlDrawer drawer;
    auto openGLdrawer_visitor =
        Visitor::create([&drawer](const auto &shape) { drawer(shape); });
    for (const auto &shape : shapes) {
      visit(openGLdrawer_visitor, shape);
    }
  }

  return 0;
}
