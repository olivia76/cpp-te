/**
 *  \author  Olivia Quinet
 */

#ifndef ANIMAL_VISITOR_HPP
#define ANIMAL_VISITOR_HPP

#include "animal.hpp"

#include "sheep.hpp"
#include "cow.hpp"

#include <functional>

namespace pipeline {

template <typename _Tp> using VISITOR_FCT = std::function<void(_Tp _x)>;
struct Visitor
    : public VISITOR_STRATEGY::visitor<Visitor, VISITOR_FCT,
               const Sheep &, const Cow&> {};

} // namespace pipeline
#endif
