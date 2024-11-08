/**
 *  \author  Olivia Quinet
 *  \file te/pimpl.hpp
 */

#ifndef _TE_PIMPL_HPP_
#define _TE_PIMPL_HPP_

#include <memory>

namespace te::pimpl {

struct unique_ptr_strategy {
  template <typename _Tp> using PIMPL = std::unique_ptr<_Tp>;
  template <typename _Vp, typename _Tp, typename... _Args>
  static auto make_pimpl(_Tp &&_x, _Args &&..._args) {
    return std::make_unique<_Vp>(std::forward<_Tp>(_x),
                                 std::forward<_Args>(_args)...);
  }

  template <typename _Vp> static auto make_pimpl_clone(const _Vp &_x) {
    return std::make_unique<std::decay_t<_Vp>>(_x);
  }

  template <typename _Pimpl> static auto clone_pimpl(const _Pimpl &_pimpl) {
    return _pimpl->clone();
  }
};

} // namespace te::pimpl

#endif
