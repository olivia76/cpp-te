/**
 *  \author  Olivia Quinet
 *  \file te/te.hpp
 */

#ifndef _TE_HPP_
#define _TE_HPP_

#include <te/pimpl.hpp>

namespace te {

auto visit(auto &&_visitor, auto &&_x) { return _x.accept(_visitor); }

template <typename _Concept, typename _VisitorStrategy, typename _PimplStrategy>
class base {
private:
  using PIMPL = typename _PimplStrategy::template PIMPL<_Concept>;

  PIMPL m_pimpl;
  [[no_unique_address]] _VisitorStrategy m_visitor_strategy;

  template <typename _Tp, typename... _Args>
  static PIMPL create_pimpl(_Tp &&_tp, _Args &&..._args) {
    using MP = typename _Concept::template model<std::decay_t<_Tp>>;
    return _PimplStrategy::template make_pimpl<MP>(
        std::forward<_Tp>(_tp), std::forward<_Args>(_args)...);
  }

protected:
  const PIMPL &pimpl() const noexcept { return m_pimpl; }
  PIMPL &pimpl() noexcept { return m_pimpl; }

  template <typename _Tp, typename... _Args,
            typename = // To prevent overriding copy/move constructor
            std::enable_if_t<!std::is_base_of<base, std::decay_t<_Tp>>::value>>
  explicit base(_Tp &&_tp, _Args &&..._args)
      : m_pimpl(create_pimpl(std::forward<_Tp>(_tp),
                             std::forward<_Args>(_args)...)) {}

  base(base &&_rhs) noexcept = default;
  base(const base &_rhs)
      : m_pimpl(_PimplStrategy::clone_pimpl(_rhs.pimpl())),
        m_visitor_strategy(_rhs.m_visitor_strategy) {}

  base &operator=(base &&_rhs) noexcept = default;
  base &operator=(const base &_rhs) {
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
