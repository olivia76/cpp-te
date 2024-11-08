/**
 *  \author  Olivia Quinet
 */

#ifndef SHAPE2_HPP
#define SHAPE2_HPP

#include "te2/default_visitor.hpp"
#include "te2/te2.hpp"
#include "te2/typeinfo_visitor.hpp"

namespace pipeline_tev2 {

struct Visitor1;
struct Visitor2;

struct shape_vtbl {
  double (*do_area)(const void *);
  double (*do_perimeter)(const void *);
  double (*_accept_visitor1)(const void *, const Visitor1 &);
  double (*_accept_visitor2)(const void *, const Visitor2 &);

  template <typename... Args>
  auto accept_visitor(auto *pimpl, const Visitor1 &visitor,
                      Args &&...args) const {
    return (*_accept_visitor1)(pimpl, visitor, std::forward<Args>(args)...);
  }
  template <typename... Args>
  auto accept_visitor(auto *pimpl, const Visitor2 &visitor,
                      Args &&...args) const {
    return (*_accept_visitor2)(pimpl, visitor, std::forward<Args>(args)...);
  }

  template <typename VP> static shape_vtbl create() {
    return {
        .do_area = [](const void *p) { return area(VP::value(p)); },
        .do_perimeter = [](const void *p) { return perimeter(VP::value(p)); },
        ._accept_visitor1 =
            [](const void *p, const Visitor1 &visitor) {
              return visitor(VP::value(p));
            },
        ._accept_visitor2 =
            [](const void *p, const Visitor2 &visitor) {
              return visitor(VP::value(p));
            },
    };
  }
};

using VISITOR_STRATEGY = te2::visitor::default_visitor_strategy;
using BASE = te2::base<shape_vtbl, VISITOR_STRATEGY>;
//using VISITOR_STRATEGY = te2::visitor::type_info_visitor_strategy;
//using BASE = te2::base<shape_vtbl, VISITOR_STRATEGY,
//                        te2::type_info_pimpl::unique_ptr_strategy>;
struct Shape : public BASE {
  template <typename Vp> explicit Shape(Vp &&vp) : BASE(std::forward<Vp>(vp)) {}

  auto area() const { return vtbl().do_area(pimpl()); }
  auto perimeter() const { return vtbl().do_perimeter(pimpl()); }
};

} // namespace pipeline_tev2

#endif
