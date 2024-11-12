/**
 *  \author  Olivia Quinet
 *  \file te/pimpl.hpp
 */

#ifndef TE_PIMPL_HPP
#define TE_PIMPL_HPP

#include <memory>

namespace te::pimpl {

struct unique_ptr_strategy {
  template <typename Vp> using PIMPL = std::unique_ptr<Vp>;
  template <typename Vp, typename ValueT, typename... Args>
  static auto make_pimpl(ValueT &&x, Args &&...args) {
    return std::make_unique<Vp>(std::forward<ValueT>(x),
                                std::forward<Args>(args)...);
  }

  template <typename Vp> static auto make_pimpl_clone(const Vp &x) {
    return std::make_unique<std::decay_t<Vp>>(x);
  }

  template <typename Pimpl> static auto clone_pimpl(const Pimpl &pimpl) {
    return pimpl->clone();
  }
};

} // namespace te::pimpl

#endif
