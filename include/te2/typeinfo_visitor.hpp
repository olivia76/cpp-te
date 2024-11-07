/**
 *  \author  Olivia Quinet
 *  \file te2/typeinfo_visitor.hpp
 */

#ifndef _TE2_TYPEINFO_VISITOR_HPP_
#define _TE2_TYPEINFO_VISITOR_HPP_

#include <memory>
#include <type_traits>
#include <typeindex>

#include <te2/value.hpp>

namespace te2::visitor {

template <typename Derived, template <typename> typename VISITOR_FCT,
          class... Ts>
struct type_info_visitor {
  template <typename _Tp> struct visit_type {
    VISITOR_FCT<_Tp> do_visit;
    detail::value_ti ti{};
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
    using TP = std::remove_reference_t<TS>;
    static_cast<VP &>(_vm).do_visit = _fn;
    static_cast<VP &>(_vm).ti = typeid(TP);
    if constexpr (I + 1 < N)
      create_i_n<I + 1, N>(std::forward<_Fn>(_fn), _vm);
  }

  template <typename... Args>
  auto call(const auto &_ti, const void *_ptr, Args &&..._args) const {
    constexpr size_t N = std::tuple_size_v<TYPES>;
    if constexpr (N == 0)
      return;
    else
      return call_i_n<0, N>(_ti, _ptr, std::forward<Args>(_args)...);
  }

  template <std::size_t I, std::size_t N, typename... Args>
  auto call_i_n(const auto &_ti, const void *_ptr, Args &&..._args) const {
    using TS = std::tuple_element_t<I, TYPES>;
    using VP = visit_type<TS>;
    using TP = std::remove_reference_t<TS>;
    const auto &method = static_cast<const VP &>(methods);
    if (_ti == method.ti) {
      const TP *ptr = static_cast<const TP *>(_ptr);
      return method.do_visit(*ptr, std::forward<Args>(_args)...);
    } else {
      if constexpr (I + 1 < N)
        return call_i_n<I + 1, N>(_ti, _ptr, std::forward<Args>(_args)...);
      else
        error_not_valid_visitor();
    }
  }

  [[noreturn]] static void error_not_valid_visitor() {
    throw std::runtime_error("Did not find a valid visitor");
  }
};

struct type_info_visitor_strategy {
  template <typename Derived, template <typename> typename VISITOR_FCT,
            class... Ts>
  using visitor = type_info_visitor<Derived, VISITOR_FCT, Ts...>;

  template <typename _Visitor, typename... _Args>
  auto operator()([[maybe_unused]] const auto &_vtbl, auto *_pimpl,
                  _Visitor &&_visitor, _Args &&..._args) const {
    auto *ptr =
        reinterpret_cast<const char *>(_pimpl) + sizeof(detail::value_base);
    return _visitor.call(_pimpl->ti, ptr, std::forward<_Args>(_args)...);
  }
};

} // namespace te2::visitor

#endif
