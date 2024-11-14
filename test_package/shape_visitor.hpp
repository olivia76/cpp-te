/**
 *  \author  Olivia Quinet
 */

#ifndef SHAPE_VISITOR_HPP
#define SHAPE_VISITOR_HPP

#include "shape.hpp"

#include "circle.hpp"
#include "rectangle.hpp"
#include "rhombus.hpp"
#include "square.hpp"
#include "triangle.hpp"

#include <functional>

namespace pipeline_tev {

template <typename _Tp> using VISITOR_FCT = std::function<void(_Tp _x)>;
struct Visitor : public VISITOR_STRATEGY::visitor<
                     Visitor, VISITOR_FCT, const shape::Square &,
                     const shape::Circle &, const shape::Rectangle &,
                     const shape::RightTriangle &, const shape::Rhombus &> {};

} // namespace pipeline_tev

#endif
