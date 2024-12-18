/**
 *  \author  Olivia Quinet
 */

#ifndef ANIMAL2_VISITOR_HPP
#define ANIMAL2_VISITOR_HPP

#include "animal2.hpp"

#include "cow.hpp"
#include "sheep.hpp"

#include <functional>

namespace pipeline {

template <typename _Tp> using VISITOR_FCT = std::function<void(_Tp _x)>;
struct Visitor : public VISITOR_STRATEGY::visitor<Visitor, VISITOR_FCT,
                                                  const Sheep &, const Cow &> {
};

} // namespace pipeline
#endif
