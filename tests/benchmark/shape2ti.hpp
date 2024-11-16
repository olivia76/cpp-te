/**
 *  \author  Olivia Quinet
 */

#ifndef SHAPE2_HPP
#define SHAPE2_HPP

#include "te2/te2.hpp"
#include "te2/typeinfo_visitor.hpp"

namespace pipeline_tev2 {

struct shape_vtbl {
  double (*do_area)(const void *);
  double (*do_perimeter)(const void *);

  template <typename CastT> static shape_vtbl create() {
    return {
        .do_area = [](const void *p) { return area(CastT::value(p)); },
        .do_perimeter =
            [](const void *p) { return perimeter(CastT::value(p)); },
    };
  }
};

using VISITOR_STRATEGY = te2::visitor::type_info_visitor_strategy;
using BASE = te2::base<shape_vtbl, VISITOR_STRATEGY,
                       te2::type_info_pimpl::unique_ptr_strategy>;
struct Shape : public BASE {
  template <typename ShapeT>
  explicit Shape(ShapeT &&shape) : BASE(std::forward<ShapeT>(shape)) {}

  auto area() const { return vtbl().do_area(pimpl()); }
  auto perimeter() const { return vtbl().do_perimeter(pimpl()); }
};

} // namespace pipeline_tev2

#endif
