/**
 *  \author  Olivia Quinet
 *  \file te2/default_visitor.hpp
 */

#ifndef TE2_DEFAULT_VISITOR_HPP
#define TE2_DEFAULT_VISITOR_HPP

#include <memory>
#include <type_traits>
#include <typeindex>

namespace te2::visitor {

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

  template <typename... Args> auto operator()(Args &&...args) const {
    return methods(std::forward<Args>(args)...);
  }

  template <typename Fn> constexpr static Derived create(Fn &&fn) {
    Derived v;
    ((static_cast<visit_type<Ts> &>(v.methods).do_visit = fn), ...);
    return v;
  }
};

struct default_visitor_strategy {
  template <typename Derived,
            template <typename> typename VisitorCallableTemplate, class... Ts>
  using visitor = default_visitor<Derived, VisitorCallableTemplate, Ts...>;

  template <typename Visitor, typename... Args>
  auto operator()(const auto &vtbl, auto *pimpl, Visitor &&visitor,
                  Args &&...args) const {
    return vtbl.accept_visitor(pimpl, std::forward<Visitor>(visitor),
                               std::forward<Args>(args)...);
  }
};

} // namespace te2::visitor

#endif
