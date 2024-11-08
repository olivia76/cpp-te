/**
 *  \author  Olivia Quinet
 */

#ifndef VISITOR_HPP
#define VISITOR_HPP

#include <te/default_visitor.hpp>

#include "circle.hpp"
#include "rectangle.hpp"
#include "rhombus.hpp"
#include "square.hpp"
#include "triangle.hpp"

#include <functional>

namespace pipeline_tev {

template <typename _Tp> using VISITOR_FCT = std::function<double(_Tp _x)>;
struct Visitor : public VISITOR_STRATEGY::visitor<
                     Visitor, VISITOR_FCT, const shape::Square &,
                     const shape::Circle &, const shape::Rectangle &,
                     const shape::RightTriangle &, const shape::Rhombus &> {};

} // namespace pipeline_tev

#endif
