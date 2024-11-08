/**
 *  \author  Olivia Quinet
 */

#ifndef SHAPE2_VISITOR_HPP
#define SHAPE2_VISITOR_HPP

#include "shape2.hpp"

#include "circle.hpp"
#include "rectangle.hpp"
#include "rhombus.hpp"
#include "square.hpp"
#include "triangle.hpp"

#include <functional>

namespace pipeline_tev2 {

template <typename Tp> using VISITOR_FCT1 = double (*)(Tp x);
struct Visitor1 : public VISITOR_STRATEGY::visitor<
                      Visitor1, VISITOR_FCT1, const shape::Square &,
                      const shape::Circle &, const shape::Rectangle &,
                      const shape::RightTriangle &, const shape::Rhombus &> {};

template <typename Tp> using VISITOR_FCT2 = std::function<double(Tp x)>;
struct Visitor2 : public VISITOR_STRATEGY::visitor<
                      Visitor2, VISITOR_FCT2, const shape::Square &,
                      const shape::Circle &, const shape::Rectangle &,
                      const shape::RightTriangle &, const shape::Rhombus &> {};

} // namespace pipeline_tev2

#endif
