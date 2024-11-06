/**
 *  \author  Olivia Quinet
 *  \file te2/default_visitor.hpp
 */

#ifndef _TE2_DEFAULT_VISITOR_HPP_
#define _TE2_DEFAULT_VISITOR_HPP_

#include <memory>
#include <type_traits>
#include <typeindex>

namespace te2::visitor {

template <typename Derived, template <typename> typename VISITOR_FCT,
          class... Ts>
struct default_visitor {
  template <typename _Tp> struct visit_type {
    VISITOR_FCT<_Tp> do_visit;
    template <typename... Args> auto operator()(_Tp _x, Args &&..._args) const {
      return do_visit(_x, std::forward<Args>(_args)...);
    }
  };
  template <class... _Ts> struct overloaded : visit_type<_Ts>... {
    using visit_type<Ts>::operator()...;
  };
  using visitor_methods = overloaded<Ts...>;
  visitor_methods methods;
  using TYPES = std::tuple<Ts...>;

  template <typename... Args> auto operator()(Args &&..._args) const {
    return methods(std::forward<Args>(_args)...);
  }

  template <typename _Fn> constexpr static Derived create(_Fn &&_fn) {
    Derived v;

    constexpr size_t N = std::tuple_size_v<TYPES>;
    if constexpr (N != 0)
      create_i_n<0, N>(std::forward<_Fn>(_fn), v.methods);
      
    return v;
  }
  template <std::size_t I, std::size_t N, typename _Fn>
  static constexpr void create_i_n(_Fn &&_fn, visitor_methods &_vm) {
    using TS = std::tuple_element_t<I, TYPES>;
    using VP = visit_type<TS>;
    static_cast<VP &>(_vm).do_visit = _fn;
    if constexpr (I + 1 < N)
      create_i_n<I + 1, N>(std::forward<_Fn>(_fn), _vm);
  }
};

struct default_visitor_strategy {
  template <typename Derived, template <typename> typename VISITOR_FCT,
            class... Ts>
  using visitor = default_visitor<Derived, VISITOR_FCT, Ts...>;

  template <typename _Visitor, typename... _Args>
  auto operator()(const auto &_vtbl, auto *_pimpl, _Visitor &&_visitor,
                  _Args &&..._args) const {
    return _vtbl.accept_visitor(_pimpl, std::forward<_Visitor>(_visitor),
                                std::forward<_Args>(_args)...);
  }
};

} // namespace te2::visitor

#endif
