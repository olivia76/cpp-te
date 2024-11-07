/**
 *  \author  Olivia Quinet
 */

#ifndef _SHAPE_HPP2_
#define _SHAPE_HPP2_

#include <te2/default_visitor.hpp>
#include <te2/te2.hpp>

namespace pipeline_tev2 {

struct Visitor;

struct shape_vtbl {
  double (*do_area)(const void *);
  double (*do_perimeter)(const void *);
  double (*accept_visitor)(const void *, const Visitor &);

  template <typename VP> static shape_vtbl create() {
    return {
        .do_area = [](const void *p) { return area(VP::value(p)); },
        .do_perimeter = [](const void *p) { return perimeter(VP::value(p)); },
        .accept_visitor =
            [](const void *p, const Visitor &_visitor) {
              return _visitor(VP::value(p));
            },
    };
  }
};

using VISITOR_STRATEGY = te2::visitor::default_visitor_strategy;
// using VISITOR_STRATEGY = te2::visitor::type_info_visitor_strategy;
using BASE = te2::base<shape_vtbl, VISITOR_STRATEGY>;
struct Shape : public BASE {
  template <typename _Vp>
  explicit Shape(_Vp &&_value) : BASE(std::forward<_Vp>(_value)) {}

  auto area() const { return vtbl().do_area(pimpl()); }
  auto perimeter() const { return vtbl().do_perimeter(pimpl()); }
};

} // namespace pipeline_tev2

#endif
