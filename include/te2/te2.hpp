/**
 *  \author  Olivia Quinet
 *  \file te2/te2.hpp
 */

#ifndef _TE2_HPP_
#define _TE2_HPP_

#include <memory>
#include <type_traits>
#include <typeindex>

#include <te2/value.hpp>

namespace te2 {

auto visit(auto &&_visitor, auto &&_x) { return _x.accept(_visitor); }

class base_pimpl {
private:
  using PIMPL = detail::PIMPL;

  PIMPL m_pimpl;

  template <typename _Tp> static PIMPL make_pimpl(_Tp &&_tp) {
    using TP = std::decay_t<_Tp>;
    using VP = detail::value<TP>;
    return detail::make_pimpl<VP>(std::forward<_Tp>(_tp));
  }

  PIMPL clone() const { return m_pimpl->clone(); }

protected:
  void *pimpl() noexcept { return m_pimpl.get(); }
  const void *pimpl() const noexcept { return m_pimpl.get(); }
  const auto &ti() const noexcept { return m_pimpl->ti; }
  auto *_pimpl() noexcept { return m_pimpl.get(); }
  const auto *_pimpl() const noexcept { return m_pimpl.get(); }

  template <typename _Tp> struct cast_vp {
    using TP = std::decay_t<_Tp>;
    using VP = detail::value<TP>;
    static const TP *ptr(const void *_p) noexcept {
      return &(static_cast<const VP *>(_p)->v);
    }
    static TP *ptr(void *_p) noexcept { return &(static_cast<VP *>(_p)->v); }
    static const TP &value(const void *_p) noexcept { return *ptr(_p); }
    static TP &value(void *_p) noexcept { return *ptr(_p); }
  };

  template <
      typename _Tp,
      typename = // To prevent overriding copy/move constructor
      std::enable_if_t<!std::is_base_of<base_pimpl, std::decay_t<_Tp>>::value>>
  explicit base_pimpl(_Tp &&_tp)
      : m_pimpl(make_pimpl(std::forward<_Tp>(_tp))) {}

  base_pimpl(base_pimpl &&_base) noexcept = default;
  base_pimpl(const base_pimpl &_base) : m_pimpl(_base.clone()) {}

  friend void swap(base_pimpl &_lhs, base_pimpl &_rhs) noexcept {
    using std::swap;
    swap(_lhs.m_pimpl, _rhs.m_pimpl);
  }
};

template <typename _VTBL, typename _VisitorStrategy>
class base : protected base_pimpl {
private:
  using VTBL = std::decay_t<_VTBL>;
  using PVTBL = const VTBL *;

  PVTBL m_pvtbl;
  [[no_unique_address]] _VisitorStrategy m_visitor_strategy;

  template <typename _Tp> static PVTBL create_vtbl() {
    static const VTBL vtbl = VTBL::template create<cast_vp<_Tp>>();
    return &vtbl;
  }

protected:
  const VTBL &vtbl() const noexcept { return *m_pvtbl; }

  template <typename _Tp,
            typename = // To prevent overriding copy/move constructor
            std::enable_if_t<!std::is_base_of<base, std::decay_t<_Tp>>::value>>
  explicit base(_Tp &&_tp)
      : base_pimpl(std::forward<_Tp>(_tp)), m_pvtbl(create_vtbl<_Tp>()) {}

  base(base<_VTBL, _VisitorStrategy> &&_base) noexcept = default;
  base(const base<_VTBL, _VisitorStrategy> &_base)
      : base_pimpl(_base), m_pvtbl(_base.m_pvtbl),
        m_visitor_strategy(_base.m_visitor_strategy) {}

  base &operator=(base<_VTBL, _VisitorStrategy> &&_rhs) noexcept = default;
  base &operator=(const base<_VTBL, _VisitorStrategy> &_rhs) {
    base copy(_rhs);
    swap(*this, copy);
    return *this;
  }

  friend void swap(base &_lhs, base &_rhs) noexcept {
    using std::swap;
    swap(static_cast<base_pimpl &>(_lhs), static_cast<base_pimpl &>(_rhs));
    swap(_lhs.m_pvtbl, _rhs.m_pvtbl);
    swap(_lhs.m_visitor_strategy, _rhs.m_visitor_strategy);
  }

public:
  template <typename Visitor, typename... Args>
  auto accept(Visitor &&_visitor, Args &&..._args) {
    return m_visitor_strategy(vtbl(), _pimpl(), std::forward<Visitor>(_visitor),
                              std::forward<Args>(_args)...);
  }
  template <typename Visitor, typename... Args>
  auto accept(Visitor &&_visitor, Args &&..._args) const {
    return m_visitor_strategy(vtbl(), _pimpl(), std::forward<Visitor>(_visitor),
                              std::forward<Args>(_args)...);
  }
};

} // namespace te2

#endif
