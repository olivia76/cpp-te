/**
 *  \author  Olivia Quinet
 */

#include "opengldrawer.hpp"

#include <iostream>

void OpenGlDrawer::operator()(const Sheep &_x) const {
  std::cout << "OpenGlDrawer::Sheep" << std::endl;
}

void OpenGlDrawer::operator()(const Cow &_x) const {
  std::cout << "OpenGlDrawer::Cow" << std::endl;
}
