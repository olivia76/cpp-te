/**
 *  \author  Olivia Quinet
 *  \file te2/pimpl_sharedptr.hpp
 */

#ifndef TE2_PIMPL_SHAREDPTR_HPP
#define TE2_PIMPL_SHAREDPTR_HPP

#include <memory>

namespace te2::pimpl {

struct shared_ptr_strategy {
  using can_clone = std::true_type;

  struct value_concept;
  using PIMPL = std::shared_ptr<value_concept>;

  struct value_concept {
    virtual ~value_concept();
  };

  template <typename ValueT> struct value_model : public value_concept {
    template <typename Vp, typename... Args>
    requires(!std::is_base_of<value_concept, std::decay_t<Vp>>::
                 value) explicit value_model(Vp &&vp, Args &&...args)
        : value(std::forward<Vp>(vp), std::forward<Args>(args)...) {}
    ValueT value;
  };

  template <typename ValueT, typename... Args>
  static auto make_pimpl(ValueT &&x, Args &&...args) {
    using VT = std::decay_t<ValueT>;
    using VM = value_model<VT>;
    return std::make_shared<VM>(std::forward<ValueT>(x),
                                std::forward<Args>(args)...);
  }

  template <typename ValueT, typename... Args>
  static auto make_pimpl_from_arguments(Args &&...args) {
    using VT = std::decay_t<ValueT>;
    using VM = value_model<VT>;
    return std::make_shared<VM>(std::forward<Args>(args)...);
  }

  static PIMPL clone_pimpl(const PIMPL &_pimpl) { return _pimpl; }

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
