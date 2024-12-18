/**
 *  \author  Olivia Quinet
 */

#ifndef SHAPE2_HPP
#define SHAPE2_HPP

#include <te2/default_visitor.hpp>
#include <te2/te2.hpp>

namespace pipeline_tev2 {

struct Visitor;

struct shape_vtbl {
  double (*do_area)(const void *);
  double (*do_perimeter)(const void *);
  void (*accept_visitor)(const void *, const Visitor &);

  template <typename CastT> static shape_vtbl create() {
    return {
        .do_area = [](const void *p) { return area(CastT::value(p)); },
        .do_perimeter =
            [](const void *p) { return perimeter(CastT::value(p)); },
        .accept_visitor =
            [](const void *p, const Visitor &visitor) {
              visitor(CastT::value(p));
            },
    };
  }
};

using VISITOR_STRATEGY = te2::visitor::default_visitor_strategy;
using BASE = te2::base<shape_vtbl, VISITOR_STRATEGY>;
struct Shape : public BASE {
  template <typename ShapeT>
  Shape(ShapeT &&shape) : BASE(std::forward<ShapeT>(shape)) {}

  auto area() const { return vtbl().do_area(pimpl()); }
  auto perimeter() const { return vtbl().do_perimeter(pimpl()); }
};

} // namespace pipeline_tev2

#endif
