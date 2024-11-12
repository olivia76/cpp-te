/**
 *  \author  Olivia Quinet
 */

#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <te/default_visitor.hpp>
#include <te/te.hpp>

namespace pipeline_tev {

struct Visitor;

using PIMPL_STRATEGY = te::pimpl::unique_ptr_strategy;

struct ShapeConcept {
  using PIMPL = PIMPL_STRATEGY::PIMPL<ShapeConcept>;

  virtual ~ShapeConcept();

  virtual double do_area() const = 0;
  virtual double do_perimeter() const = 0;

  virtual double accept_visitor(const Visitor &_visitor) const = 0;

  virtual PIMPL clone() const = 0;

  template <typename ShapeT> struct model;
};

template <typename ShapeT> struct ShapeConcept::model : public ShapeConcept {
  template <typename Vp>
  explicit model(Vp &&vp) : shape(std::forward<Vp>(vp)) {}

  double do_area() const final { return area(shape); }
  double do_perimeter() const final { return perimeter(shape); }

  double accept_visitor(const Visitor &visitor) const final {
    return visitor(shape);
  }

  PIMPL clone() const final { return PIMPL_STRATEGY::make_pimpl_clone(*this); }

  ShapeT shape;
};

using VISITOR_STRATEGY = te::visitor::default_visitor_strategy;
using BASE = te::base<ShapeConcept, VISITOR_STRATEGY, PIMPL_STRATEGY>;

struct Shape : public BASE {
  template <typename ShapeT>
  explicit Shape(ShapeT &&shape) : BASE(std::forward<ShapeT>(shape)) {}

  auto area() const { return pimpl()->do_area(); }
  auto perimeter() const { return pimpl()->do_perimeter(); }
};

} // namespace pipeline_tev

#endif
