/**
 *  \author  Olivia Quinet
 *  \file te2/te2.hpp
 */

#ifndef TE2_HPP
#define TE2_HPP

#include <te2/pimpl.hpp>

namespace te2 {

auto visit(auto &&visitor, auto &&x) { return x.accept(visitor); }

template <typename VTBL_, typename VisitorStrategy,
          typename PimplStrategy = te2::pimpl::unique_ptr_strategy>
class base {
private:
  using PIMPL = typename PimplStrategy::PIMPL;
  using VTBL = std::decay_t<VTBL_>;
  using PVTBL = const VTBL *;

  PIMPL m_pimpl;
  PVTBL m_pvtbl;
  [[no_unique_address]] VisitorStrategy m_visitor_strategy;

  template <typename Tp> static PVTBL create_vtbl() {
    using VP = typename PimplStrategy::template cast_vp<Tp>;
    static const VTBL vtbl = VTBL::template create<VP>();
    return &vtbl;
  }

protected:
  void *pimpl() noexcept { return m_pimpl.get(); }
  const void *pimpl() const noexcept { return m_pimpl.get(); }
  auto *_pimpl() noexcept { return m_pimpl.get(); }
  const auto *_pimpl() const noexcept { return m_pimpl.get(); }
  const VTBL &vtbl() const noexcept { return *m_pvtbl; }

  template <typename Tp,
            typename = // To prevent overriding copy/move constructor
            std::enable_if_t<!std::is_base_of<base, std::decay_t<Tp>>::value>>
  explicit base(Tp &&tp)
      : m_pimpl(PimplStrategy::make_pimpl(std::forward<Tp>(tp))),
        m_pvtbl(create_vtbl<Tp>()) {}

  base(base &&) noexcept = default;
  base(const base &rhs)
      : m_pimpl(PimplStrategy::clone_pimpl(rhs.m_pimpl)), m_pvtbl(rhs.m_pvtbl),
        m_visitor_strategy(rhs.m_visitor_strategy) {}

  base &operator=(base &&) noexcept = default;
  base &operator=(const base &rhs) {
    base copy(rhs);
    swap(*this, copy);
    return *this;
  }

  friend void swap(base &lhs, base &rhs) noexcept {
    using std::swap;
    swap(lhs.m_pimpl, rhs.m_pimpl);
    swap(lhs.m_pvtbl, rhs.m_pvtbl);
    swap(lhs.m_visitor_strategy, rhs.m_visitor_strategy);
  }

public:
  template <typename Visitor, typename... Args>
  auto accept(Visitor &&visitor, Args &&...args) {
    return m_visitor_strategy(vtbl(), _pimpl(), std::forward<Visitor>(visitor),
                              std::forward<Args>(args)...);
  }
  template <typename Visitor, typename... Args>
  auto accept(Visitor &&visitor, Args &&...args) const {
    return m_visitor_strategy(vtbl(), _pimpl(), std::forward<Visitor>(visitor),
                              std::forward<Args>(args)...);
  }
};

} // namespace te2

#endif
