/**
 *  \author  Olivia Quinet
 *  \file te2/typeinfo_visitor.hpp
 */

#ifndef TE2_TYPEINFO_VISITOR_HPP
#define TE2_TYPEINFO_VISITOR_HPP

#include <memory>
#include <type_traits>
#include <typeindex>

#include <te2/typeinfo_pimpl.hpp>

namespace te2::visitor {

template <template <typename> typename VisitorCallableTemplate, class... Ts>
struct type_info_visitor {
  template <typename Tp> struct visit_type {
    VisitorCallableTemplate<Tp> do_visit;
    const te2::type_info_pimpl::value_ti ti =
        te2::type_info_pimpl::value_ti::get<Tp>();
    template <typename... Args> auto operator()(Tp x, Args &&...args) const {
      return do_visit(x, std::forward<Args>(args)...);
    }
  };
  template <class... Ts_> struct overloaded : visit_type<Ts_>... {
    using visit_type<Ts_>::operator()...;
  };
  using visitor_methods = overloaded<Ts...>;
  visitor_methods methods;
  using TYPES = std::tuple<Ts...>;

  template <typename... Args> auto operator()(Args &&...args) const {
    return methods(std::forward<Args>(args)...);
  }

  template <typename Fn> constexpr static auto create(Fn &&fn) {
    type_info_visitor v;
    ((static_cast<visit_type<Ts> &>(v.methods).do_visit = fn), ...);
    return v;
  }

  template <typename... Args>
  auto call(const auto &_ti, const void *_ptr, Args &&...args) const {
    constexpr size_t N = std::tuple_size_v<TYPES>;
    if constexpr (N == 0)
      return;
    else
      return call_i_n<0, N>(_ti, _ptr, std::forward<Args>(args)...);
  }

  template <std::size_t I, std::size_t N, typename... Args>
  auto call_i_n(const auto &_ti, const void *_ptr, Args &&...args) const {
    using TS = std::tuple_element_t<I, TYPES>;
    using VP = visit_type<TS>;
    using TP = std::remove_reference_t<TS>;
    const auto &method = static_cast<const VP &>(methods);
    if (_ti == method.ti) {
      const TP *ptr = static_cast<const TP *>(_ptr);
      return method.do_visit(*ptr, std::forward<Args>(args)...);
    } else {
      if constexpr (I + 1 < N)
        return call_i_n<I + 1, N>(_ti, _ptr, std::forward<Args>(args)...);
      else
        error_not_valid_visitor();
    }
  }

  [[noreturn]] static void error_not_valid_visitor() {
    throw std::runtime_error("Did not find a valid visitor");
  }
};

struct type_info_visitor_strategy {
  template <template <typename> typename VisitorCallableTemplate, class... Ts>
  using visitor = type_info_visitor<VisitorCallableTemplate, Ts...>;

  template <typename Visitor, typename... Args>
  auto operator()([[maybe_unused]] const auto &vtbl, auto *pimpl,
                  Visitor &&visitor, Args &&...args) const {
    return visitor.call(pimpl->ti, pimpl->get_ptr(),
                        std::forward<Args>(args)...);
  }
};

} // namespace te2::visitor

#endif
