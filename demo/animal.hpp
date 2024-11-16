/**
 *  \author  Olivia Quinet
 */

#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <te/default_visitor.hpp>
#include <te/te.hpp>

namespace pipeline {

struct Visitor;

using PIMPL_STRATEGY = te::pimpl::unique_ptr_strategy;

struct AnimalConcept {
  using PIMPL = PIMPL_STRATEGY::PIMPL<AnimalConcept>;

  virtual ~AnimalConcept();

  virtual void do_make_sound() const = 0;

  virtual void accept_visitor(const Visitor &_visitor) const = 0;

  virtual PIMPL clone() const = 0;

  template <typename AnimalT> struct model;
};

template <typename AnimalT> struct AnimalConcept::model : public AnimalConcept {
  template <typename Vp>
  explicit model(Vp &&vp) : animal(std::forward<Vp>(vp)) {}

  void do_make_sound() const final { make_sound(animal); }

  void accept_visitor(const Visitor &visitor) const final {
    return visitor(animal);
  }

  PIMPL clone() const final { return PIMPL_STRATEGY::make_pimpl_clone(*this); }

  AnimalT animal;
};

using VISITOR_STRATEGY = te::visitor::default_visitor_strategy;
using BASE = te::base<AnimalConcept, VISITOR_STRATEGY, PIMPL_STRATEGY>;

struct Animal : public BASE {
  template <typename AnimalT>
  Animal(AnimalT &&animal) : BASE(std::forward<AnimalT>(animal)) {}

  auto make_sound() const { return pimpl()->do_make_sound(); }
};

} // namespace pipeline

#endif
