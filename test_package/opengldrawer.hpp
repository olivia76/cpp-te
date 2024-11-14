/**
 *  \author  Olivia Quinet
 */

#ifndef OPENGLDRAWER_HPP
#define OPENGLDRAWER_HPP

#include "circle.hpp"
#include "rectangle.hpp"
#include "rhombus.hpp"
#include "square.hpp"
#include "triangle.hpp"

class OpenGlDrawer {
public:
  void operator()(const shape::Circle &_x) const;
  void operator()(const shape::Square &_x) const;
  void operator()(const shape::Rectangle &_x) const;
  void operator()(const shape::RightTriangle &_x) const;
  void operator()(const shape::Rhombus &_x) const;
};

#endif
