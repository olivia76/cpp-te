/**
 *  \author  Olivia Quinet
 *  \file te/default_visitor.hpp
 */

#ifndef TE_DEFAULT_VISITOR_HPP
#define TE_DEFAULT_VISITOR_HPP

#include <memory>
#include <type_traits>
#include <typeindex>

namespace te::visitor {

template <typename Derived,
          template <typename> typename VisitorCallableTemplate, class... Ts>
struct default_visitor {
  template <typename Tp> struct visit_type {
    VisitorCallableTemplate<Tp> do_visit;
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

  template <typename Fn> constexpr static Derived create(Fn &&fn) {
    Derived v;

    constexpr size_t N = std::tuple_size_v<TYPES>;
    if constexpr (N != 0)
      create_i_n<0, N>(std::forward<Fn>(fn), v.methods);

    return v;
  }
  template <std::size_t I, std::size_t N, typename Fn>
  static constexpr void create_i_n(Fn &&fn, visitor_methods &vm) {
    using TS = std::tuple_element_t<I, TYPES>;
    using VP = visit_type<TS>;
    static_cast<VP &>(vm).do_visit = fn;
    if constexpr (I + 1 < N)
      create_i_n<I + 1, N>(std::forward<Fn>(fn), vm);
  }
};

struct default_visitor_strategy {
  template <typename Derived,
            template <typename> typename VisitorCallableTemplate, class... Ts>
  using visitor = default_visitor<Derived, VisitorCallableTemplate, Ts...>;

  template <typename PIMPL, typename Visitor, typename... Args>
  auto operator()(PIMPL &&_pimpl, Visitor &&_visitor, Args &&...args) const {
    return _pimpl->accept_visitor(std::forward<Visitor>(_visitor),
                                  std::forward<Args>(args)...);
  }
};

} // namespace te::visitor

#endif
