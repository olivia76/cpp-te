/**
 *  \author  Olivia Quinet
 *  \file te/te.hpp
 */

#ifndef TE_HPP
#define TE_HPP

#include <te/pimpl.hpp>

namespace te {

template <typename... Args> auto visit(auto &&visitor, auto &&x, Args... args) {
  return x.accept(visitor, std::forward<Args>(args)...);
}

template <typename Concept, typename VisitorStrategy, typename PimplStrategy>
class base {
private:
  using PIMPL = typename PimplStrategy::template PIMPL<Concept>;

  PIMPL m_pimpl;
  [[no_unique_address]] VisitorStrategy m_visitor_strategy;

  template <typename ValueT, typename... Args>
  static PIMPL create_pimpl(ValueT &&value, Args &&...args) {
    using MP = typename Concept::template model<std::decay_t<ValueT>>;
    return PimplStrategy::template make_pimpl<MP>(std::forward<ValueT>(value),
                                                  std::forward<Args>(args)...);
  }

protected:
  const PIMPL &pimpl() const noexcept { return m_pimpl; }
  PIMPL &pimpl() noexcept { return m_pimpl; }

  template <
      typename ValueT, typename... Args,
      typename = // To prevent overriding copy/move constructor
      std::enable_if_t<!std::is_base_of<base, std::decay_t<ValueT>>::value>>
  explicit base(ValueT &&value, Args &&...args)
      : m_pimpl(create_pimpl(std::forward<ValueT>(value),
                             std::forward<Args>(args)...)) {}

  base(base &&_rhs) noexcept = default;
  base(const base &rhs)
      : m_pimpl(PimplStrategy::clone_pimpl(rhs.pimpl())),
        m_visitor_strategy(rhs.m_visitor_strategy) {}

  base &operator=(base &&) noexcept = default;
  base &operator=(const base &rhs) {
    using std::swap;
    base copy(rhs);
    swap(m_pimpl, copy.m_pimpl);
    swap(m_visitor_strategy, copy.m_visitor_strategy);
    return *this;
  }

public:
  template <typename Visitor, typename... Args>
  auto accept(Visitor &&visitor, Args &&...args) {
    return m_visitor_strategy(pimpl(), std::forward<Visitor>(visitor),
                              std::forward<Args>(args)...);
  }
  template <typename Visitor, typename... Args>
  auto accept(Visitor &&visitor, Args &&...args) const {
    return m_visitor_strategy(pimpl(), std::forward<Visitor>(visitor),
                              std::forward<Args>(args)...);
  }
};

} // namespace te

#endif
