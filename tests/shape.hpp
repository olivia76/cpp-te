/**
 *  \author  Olivia Quinet
 */

#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "te/default_visitor.hpp"
#include "te/te.hpp"

namespace pipeline_tev {

struct Visitor1;
struct Visitor2;

using PIMPL_STRATEGY = te::pimpl::unique_ptr_strategy;

struct ShapeConcept {
  using PIMPL = PIMPL_STRATEGY::PIMPL<ShapeConcept>;

  virtual ~ShapeConcept() = default;

  virtual double do_area() const = 0;
  virtual double do_perimeter() const = 0;

  virtual double accept_visitor(const Visitor1 &_visitor) const = 0;
  virtual double accept_visitor(const Visitor2 &_visitor) const = 0;

  virtual PIMPL clone() const = 0;

  template <typename Tp> struct model;
};

template <typename Tp> struct ShapeConcept::model : public ShapeConcept {
  template <typename Vp>
  explicit model(Vp &&vp) : value(std::forward<Vp>(vp)) {}

  double do_area() const final { return area(value); }
  double do_perimeter() const final { return perimeter(value); }

  double accept_visitor(const Visitor1 &_visitor) const final {
    return _visitor(value);
  }
  double accept_visitor(const Visitor2 &_visitor) const final {
    return _visitor(value);
  }

  PIMPL clone() const final { return PIMPL_STRATEGY::make_pimpl_clone(*this); }

  Tp value;
};

using VISITOR_STRATEGY = te::visitor::default_visitor_strategy;
using BASE = te::base<ShapeConcept, VISITOR_STRATEGY, PIMPL_STRATEGY>;

struct Shape : public BASE {
  template <typename Vp> explicit Shape(Vp &&vp) : BASE(std::forward<Vp>(vp)) {}

  auto area() const { return pimpl()->do_area(); }
  auto perimeter() const { return pimpl()->do_perimeter(); }
};

} // namespace pipeline_tev

#endif
