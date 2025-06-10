/**
 *  \author  Olivia Quinet
 *  \file te2/pimpl.hpp
 */

#ifndef TE2_PIMPL_HPP
#define TE2_PIMPL_HPP

#include <memory>

namespace te2::pimpl {

struct unique_ptr_strategy {
  struct value_concept;
  using PIMPL = std::unique_ptr<value_concept>;

  struct value_concept {
    virtual ~value_concept();
    virtual PIMPL clone() const = 0;
  };

  template <typename ValueT> struct value_model : public value_concept {
    template <typename Vp, typename... Args>
    requires (!std::is_base_of<value_concept, std::decay_t<Vp>>::value)
    explicit value_model(Vp &&vp, Args &&...args)
        : value(std::forward<Vp>(vp), std::forward<Args>(args)...) {}
    value_model(const value_model &) = default;
    PIMPL clone() const final {
      return unique_ptr_strategy::make_pimpl_clone(*this);
    }
    ValueT value;
  };

  template <typename ValueT, typename... Args>
  static auto make_pimpl(ValueT &&x, Args &&...args) {
    using VT = std::decay_t<ValueT>;
    using VM = value_model<VT>;
    return std::make_unique<VM>(std::forward<ValueT>(x),
                                std::forward<Args>(args)...);
  }

  template <typename Vp> static auto make_pimpl_clone(const Vp &x) {
    return std::make_unique<std::decay_t<Vp>>(x);
  }

  static PIMPL clone_pimpl(const PIMPL &_pimpl) { return _pimpl->clone(); }

  template <typename ValueT> struct cast_value {
    using VT = std::decay_t<ValueT>;
    using VM = value_model<VT>;
    static const VT &value(const void *p) noexcept {
      return static_cast<const VM *>(p)->value;
    }
    static VT &value(void *p) noexcept { return static_cast<VM *>(p)->value; }
  };
};

} // namespace te2::pimpl

#endif
