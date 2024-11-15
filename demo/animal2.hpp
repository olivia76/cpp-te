/**
 *  \author  Olivia Quinet
 */

#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <te2/default_visitor.hpp>
#include <te2/te2.hpp>

namespace pipeline {

struct Visitor;

struct AnimalVtbl {

  void (*do_make_sound)(const void *);
  void (*accept_visitor)(const void *, const Visitor &);

  template <typename CastT> static AnimalVtbl create() {
    return {
        .do_make_sound =
            [](const void *p) { return make_sound(CastT::value(p)); },
        .accept_visitor =
            [](const void *p, const Visitor &visitor) {
              return visitor(CastT::value(p));
            },
    };
  }
};

using VISITOR_STRATEGY = te2::visitor::default_visitor_strategy;
using BASE = te2::base<AnimalVtbl, VISITOR_STRATEGY>;

struct Animal : public BASE {
  template <typename AnimalT>
  Animal(AnimalT &&animal) : BASE(std::forward<AnimalT>(animal)) {}

  auto make_sound() const { return vtbl().do_make_sound(pimpl()); }
};

} // namespace pipeline

#endif
