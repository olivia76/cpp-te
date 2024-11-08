/**
 *  \author  Olivia Quinet
 *  \file te2/te2.hpp
 */

#ifndef _TE2_HPP_
#define _TE2_HPP_

#include <te2/pimpl.hpp>

namespace te2 {

auto visit(auto &&_visitor, auto &&_x) { return _x.accept(_visitor); }

template <typename _VTBL, typename _VisitorStrategy,
          typename _PimplStrategy = te2::pimpl::unique_ptr_strategy>
class base {
private:
  using PIMPL = typename _PimplStrategy::PIMPL;
  using VTBL = std::decay_t<_VTBL>;
  using PVTBL = const VTBL *;

  PIMPL m_pimpl;
  PVTBL m_pvtbl;
  [[no_unique_address]] _VisitorStrategy m_visitor_strategy;

  template <typename _Tp> static PVTBL create_vtbl() {
    using VP = typename _PimplStrategy::template cast_vp<_Tp>;
    static const VTBL vtbl = VTBL::template create<VP>();
    return &vtbl;
  }

protected:
  void *pimpl() noexcept { return m_pimpl.get(); }
  const void *pimpl() const noexcept { return m_pimpl.get(); }
  auto *_pimpl() noexcept { return m_pimpl.get(); }
  const auto *_pimpl() const noexcept { return m_pimpl.get(); }
  const VTBL &vtbl() const noexcept { return *m_pvtbl; }

  template <typename _Tp,
            typename = // To prevent overriding copy/move constructor
            std::enable_if_t<!std::is_base_of<base, std::decay_t<_Tp>>::value>>
  explicit base(_Tp &&_tp)
      : m_pimpl(_PimplStrategy::make_pimpl(std::forward<_Tp>(_tp))),
        m_pvtbl(create_vtbl<_Tp>()) {}

  base(base &&_base) noexcept = default;
  base(const base &_base)
      : m_pimpl(_PimplStrategy::clone_pimpl(_base.m_pimpl)),
        m_pvtbl(_base.m_pvtbl), m_visitor_strategy(_base.m_visitor_strategy) {}

  base &operator=(base &&_rhs) noexcept = default;
  base &operator=(const base &_rhs) {
    base copy(_rhs);
    swap(*this, copy);
    return *this;
  }

  friend void swap(base &_lhs, base &_rhs) noexcept {
    using std::swap;
    swap(_lhs.m_pimpl, _rhs.m_pimpl);
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
