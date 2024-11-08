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

  template <typename Tp> struct value_model : public value_concept {
    template <typename Vp,
              typename = // To prevent overriding copy/move constructor
              std::enable_if_t<
                  !std::is_base_of<value_model, std::decay_t<Vp>>::value>>
    explicit value_model(Vp &&vp) : v(std::forward<Vp>(vp)) {}
    value_model(const value_model &) = default;
    PIMPL clone() const final {
      return unique_ptr_strategy::make_pimpl_clone(*this);
    }
    Tp v;
  };

  template <typename Tp, typename... Args>
  static auto make_pimpl(Tp &&x, Args &&...args) {
    using TP = std::decay_t<Tp>;
    using VP = value_model<TP>;
    return std::make_unique<VP>(std::forward<Tp>(x),
                                std::forward<Args>(args)...);
  }

  template <typename Vp> static auto make_pimpl_clone(const Vp &x) {
    return std::make_unique<std::decay_t<Vp>>(x);
  }

  static PIMPL clone_pimpl(const PIMPL &_pimpl) { return _pimpl->clone(); }

  template <typename Tp> struct cast_vp {
    using TP = std::decay_t<Tp>;
    using VP = value_model<TP>;
    static const TP *ptr(const void *p) noexcept {
      return std::addressof(value(p));
    }
    static TP *ptr(void *p) noexcept { return std::addressof(value(p)); }
    static const TP &value(const void *p) noexcept {
      return static_cast<const VP *>(p)->v;
    }
    static TP &value(void *p) noexcept { return static_cast<VP *>(p)->v; }
  };
};

} // namespace te2::pimpl

#endif
