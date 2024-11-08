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

  template <typename _Tp> struct model;
};

template <typename _Tp> struct ShapeConcept::model : public ShapeConcept {
  template <typename _Vp>
  explicit model(_Vp &&_value) : value(std::forward<_Vp>(_value)) {}

  double do_area() const final { return area(value); }
  double do_perimeter() const final { return perimeter(value); }

  double accept_visitor(const Visitor &_visitor) const final {
    return _visitor(value);
  }

  PIMPL clone() const final { return PIMPL_STRATEGY::make_pimpl_clone(*this); }

  _Tp value;
};

using VISITOR_STRATEGY = te::visitor::default_visitor_strategy;
using BASE = te::base<ShapeConcept, VISITOR_STRATEGY, PIMPL_STRATEGY>;

struct Shape : public BASE {
  template <typename _Vp>
  explicit Shape(_Vp &&_value) : BASE(std::forward<_Vp>(_value)) {}

  auto area() const { return pimpl()->do_area(); }
  auto perimeter() const { return pimpl()->do_perimeter(); }
};

} // namespace pipeline_tev

#endif
