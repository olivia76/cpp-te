/**
 *  \author  Olivia Quinet
 */

#ifndef OPENGLDRAWER_HPP
#define OPENGLDRAWER_HPP

#include "cow.hpp"
#include "sheep.hpp"

class OpenGlDrawer {
public:
  void operator()(const Sheep &_x) const;
  void operator()(const Cow &_x) const;
};

#endif
