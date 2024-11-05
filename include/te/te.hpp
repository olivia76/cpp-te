/**
 *  \author  Olivia Quinet
 *  \file te/te.hpp
 */

#ifndef _TE_HPP_
#define _TE_HPP_

#include <memory>
#include <type_traits>
#include <typeindex>

namespace te {

namespace detail {

template <typename _Tp> using PIMPL = std::unique_ptr<_Tp>;

} // namespace detail

auto visit(auto &&_visitor, auto &&_x) { return _x.accept(_visitor); }

class te_base {};

template <typename _Concept, typename _VisitorStrategy>
class base : protected te_base {
private:
  using PIMPL = detail::PIMPL<_Concept>;

  PIMPL m_pimpl;
  [[no_unique_address]] _VisitorStrategy m_visitor_strategy;

  template <typename _Tp, typename... _Args>
  static PIMPL create_pimpl(_Tp &&_tp, _Args &&..._args) {
    using MP = typename _Concept::template model<std::decay_t<_Tp>>;
    return std::make_unique<MP>(std::forward<_Tp>(_tp),
                                std::forward<_Args>(_args)...);
  }

protected:
  const PIMPL &pimpl() const noexcept { return m_pimpl; }
  PIMPL &pimpl() noexcept { return m_pimpl; }

  template <typename _Tp, typename... _Args,
            typename = std::enable_if_t<!std::is_base_of<te_base, _Tp>::value>>
  explicit base(_Tp &&_tp, _Args &&..._args)
      : m_pimpl(create_pimpl(std::forward<_Tp>(_tp),
                             std::forward<_Args>(_args)...)) {}

  base(base<_Concept, _VisitorStrategy> &&_rhs) noexcept = default;
  base(const base<_Concept, _VisitorStrategy> &_rhs)
      : m_pimpl(_rhs.pimpl()->clone()),
        m_visitor_strategy(_rhs.m_visitor_strategy) {}

  base &operator=(base<_Concept, _VisitorStrategy> &&_rhs) noexcept = default;
  base &operator=(const base<_Concept, _VisitorStrategy> &_rhs) {
    using std::swap;
    base copy(_rhs);
    swap(*this, copy);
    return *this;
  }

  friend void swap(base &_lhs, base &_rhs) noexcept {
    using std::swap;
    swap(_lhs.m_pimpl, _rhs.m_pimpl);
    swap(_lhs.m_visitor_strategy, _rhs.m_visitor_strategy);
  }

public:
  template <typename Visitor, typename... Args>
  auto accept(Visitor &&_visitor, Args &&..._args) {
    return m_visitor_strategy(pimpl(), std::forward<Visitor>(_visitor),
                              std::forward<Args>(_args)...);
  }
  template <typename Visitor, typename... Args>
  auto accept(Visitor &&_visitor, Args &&..._args) const {
    return m_visitor_strategy(pimpl(), std::forward<Visitor>(_visitor),
                              std::forward<Args>(_args)...);
  }
};

} // namespace te

#endif
