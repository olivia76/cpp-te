/**
 *  \author  Olivia Quinet
 *  \file te2/pimpl.hpp
 */

#ifndef _TE2_PIMPL_HPP_
#define _TE2_PIMPL_HPP_

#include <memory>

namespace te2::pimpl {

struct unique_ptr_strategy {
  struct value_concept;
  using PIMPL = std::unique_ptr<value_concept>;

  struct value_concept {
    virtual ~value_concept();
    virtual PIMPL clone() const = 0;
  };

  template <typename _Tp> struct value_model : public value_concept {
    template <typename _Vp,
              typename = // To prevent overriding copy/move constructor
              std::enable_if_t<
                  !std::is_base_of<value_model, std::decay_t<_Vp>>::value>>
    explicit value_model(_Vp &&_v) : v(std::forward<_Vp>(_v)) {}
    value_model(const value_model &) = default;
    PIMPL clone() const final {
      return unique_ptr_strategy::make_pimpl_clone(*this);
    }
    _Tp v;
  };

  template <typename _Tp, typename... _Args>
  static auto make_pimpl(_Tp &&_x, _Args &&..._args) {
    using TP = std::decay_t<_Tp>;
    using VP = value_model<TP>;
    return std::make_unique<VP>(std::forward<_Tp>(_x),
                                std::forward<_Args>(_args)...);
  }

  template <typename _Vp> static auto make_pimpl_clone(const _Vp &_x) {
    return std::make_unique<std::decay_t<_Vp>>(_x);
  }

  static PIMPL clone_pimpl(const PIMPL &_pimpl) { return _pimpl->clone(); }

  template <typename _Tp> struct cast_vp {
    using TP = std::decay_t<_Tp>;
    using VP = value_model<TP>;
    static const TP *ptr(const void *_p) noexcept {
      return &(static_cast<const VP *>(_p)->v);
    }
    static TP *ptr(void *_p) noexcept { return &(static_cast<VP *>(_p)->v); }
    static const TP &value(const void *_p) noexcept { return *ptr(_p); }
    static TP &value(void *_p) noexcept { return *ptr(_p); }
  };
};

} // namespace te2::pimpl

#endif
