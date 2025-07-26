/**
 *  \author  Olivia Quinet
 *  \file te2/te2.hpp
 */

#ifndef TE2_HPP
#define TE2_HPP

#include <te2/pimpl.hpp>

namespace te2 {

template <typename... Args> auto visit(auto &&visitor, auto &&x, Args... args) {
  return x.accept(visitor, std::forward<Args>(args)...);
}

template <typename VTBL_, typename VisitorStrategy,
          typename PimplStrategy = te2::pimpl::unique_ptr_strategy>
class base {
private:
  using PIMPL = typename PimplStrategy::PIMPL;
  using VTBL = std::decay_t<VTBL_>;
  using PVTBL = const VTBL *;
  using CAN_CLONE = typename PimplStrategy::can_clone;

  PIMPL m_pimpl;
  PVTBL m_pvtbl;
  [[no_unique_address]] VisitorStrategy m_visitor_strategy;

  constexpr auto *raw_pimpl() noexcept { return m_pimpl.get(); }
  constexpr const auto *raw_pimpl() const noexcept { return m_pimpl.get(); }

  template <typename ValueT> static PVTBL create_vtbl() {
    using VT = std::remove_cvref_t<ValueT>;
    using CastT = typename PimplStrategy::template cast_value<VT>;
    static const VTBL vtbl = VTBL::template create<CastT>();
    return &vtbl;
  }

  constexpr explicit base(PIMPL &&_pimpl, PVTBL &&_pvtbl)
      : m_pimpl(std::move(_pimpl)), m_pvtbl(std::move(_pvtbl)) {}

protected:
  constexpr auto& access_pimpl() noexcept { return m_pimpl; }
  constexpr const auto& access_pimpl() const noexcept { return m_pimpl; }

  constexpr void *pimpl() noexcept { return m_pimpl.get(); }
  constexpr const void *pimpl() const noexcept { return m_pimpl.get(); }
  constexpr const VTBL &vtbl() const noexcept { return *m_pvtbl; }

  template <typename ValueT, typename... Args>
  requires(!std::is_base_of<base, std::decay_t<ValueT>>::value) explicit base(
      ValueT &&tp, Args &&...args)
      : m_pimpl(PimplStrategy::make_pimpl(std::forward<ValueT>(tp),
                                          std::forward<Args>(args)...)),
        m_pvtbl(create_vtbl<ValueT>()) {}

  template <typename DerivedT, typename ValueT, typename... Args>
  static auto create_from_arguments(Args &&...args) {
    base b;
    b.m_pimpl = PimplStrategy::template make_pimpl_from_arguments<ValueT>(
        std::forward<Args>(args)...);
    b.m_pvtbl = create_vtbl<ValueT>();
    return DerivedT(std::move(b));
  }

  constexpr explicit base() noexcept = default;

  constexpr base(base &&) noexcept = default;

  constexpr base(const base &rhs) requires(CAN_CLONE::value)
      : m_pimpl(PimplStrategy::clone_pimpl(rhs.m_pimpl)), m_pvtbl(rhs.m_pvtbl),
        m_visitor_strategy(rhs.m_visitor_strategy) {}

  constexpr base &operator=(base &&) noexcept = default;

  constexpr base &operator=(const base &rhs) requires(CAN_CLONE::value) {
    using std::swap;
    base copy(rhs);
    swap(m_pimpl, copy.m_pimpl);
    swap(m_pvtbl, copy.m_pvtbl);
    swap(m_visitor_strategy, copy.m_visitor_strategy);
    return *this;
  }

public:
  template <typename Visitor, typename... Args>
  auto accept(Visitor &&visitor, Args &&...args) {
    return m_visitor_strategy(vtbl(), raw_pimpl(),
                              std::forward<Visitor>(visitor),
                              std::forward<Args>(args)...);
  }
  template <typename Visitor, typename... Args>
  auto accept(Visitor &&visitor, Args &&...args) const {
    return m_visitor_strategy(vtbl(), raw_pimpl(),
                              std::forward<Visitor>(visitor),
                              std::forward<Args>(args)...);
  }
};

} // namespace te2

#endif
