/**
 *  \author  Olivia Quinet
 */

#ifndef SHAPE2_HPP
#define SHAPE2_HPP

#include "te2/default_visitor.hpp"
#include "te2/te2.hpp"

namespace pipeline_tev2 {

struct Visitor1;
struct Visitor2;
struct Visitor3;

struct shape_vtbl {
  double (*do_area)(const void *);
  double (*do_perimeter)(const void *);
  double (*accept_visitor1)(const void *, const Visitor1 &);
  double (*accept_visitor2)(const void *, const Visitor2 &);
  void (*accept_visitor3)(const void *, const Visitor3 &, void *result);

  template <typename... Args>
  auto accept_visitor(auto *pimpl, const Visitor1 &visitor,
                      Args &&...args) const {
    return (*accept_visitor1)(pimpl, visitor, std::forward<Args>(args)...);
  }
  template <typename... Args>
  auto accept_visitor(auto *pimpl, const Visitor2 &visitor,
                      Args &&...args) const {
    return (*accept_visitor2)(pimpl, visitor, std::forward<Args>(args)...);
  }
  template <typename... Args>
  auto accept_visitor(auto *pimpl, const Visitor3 &visitor,
                      Args &&...args) const {
    return (*accept_visitor3)(pimpl, visitor, std::forward<Args>(args)...);
  }

  template <typename CastT> static shape_vtbl create() {
    return {
        .do_area = [](const void *p) { return area(CastT::value(p)); },
        .do_perimeter =
            [](const void *p) { return perimeter(CastT::value(p)); },
        .accept_visitor1 =
            [](const void *p, const Visitor1 &visitor) {
              return visitor(CastT::value(p));
            },
        .accept_visitor2 =
            [](const void *p, const Visitor2 &visitor) {
              return visitor(CastT::value(p));
            },
        .accept_visitor3 =
            [](const void *p, const Visitor3 &visitor, void *result) {
              return visitor(CastT::value(p), result);
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
