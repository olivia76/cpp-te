/**
 *  \author  Olivia Quinet
 */

#include "opengldrawer.hpp"

#include <spdlog/spdlog.h>

void OpenGlDrawer::operator()(const shape::Circle &_x) const {
  spdlog::info("OpenGlDrawer::Circle {}", fmt::ptr(this));
}
void OpenGlDrawer::operator()(const shape::Square &_x) const {
  spdlog::info("OpenGlDrawer::Square {}", fmt::ptr(this));
}
void OpenGlDrawer::operator()(const shape::Rectangle &_x) const {
  spdlog::info("OpenGlDrawer::Rectangle {}", fmt::ptr(this));
}
void OpenGlDrawer::operator()(const shape::RightTriangle &_x) const {
  spdlog::info("OpenGlDrawer::RightTriangle {}", fmt::ptr(this));
}
void OpenGlDrawer::operator()(const shape::Rhombus &_x) const {
  spdlog::info("OpenGlDrawer::Rhombus {}", fmt::ptr(this));
}
