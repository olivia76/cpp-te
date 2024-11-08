/**
 *  \author  Olivia Quinet
 *  \file te2/pimpl.hpp
 */

#ifndef _TE2_TYPEINFO_PIMPL_HPP_
#define _TE2_TYPEINFO_PIMPL_HPP_

#include <memory>
#include <type_traits>
#include <typeindex>

namespace te2::type_info_pimpl {

struct value_ti {
  using TypeInfoRef = std::reference_wrapper<const std::type_info>;

  TypeInfoRef ti;
  size_t hc{};

  value_ti() : ti{typeid(void)} {}
  value_ti(const std::type_info &_ti) : ti(_ti), hc(_ti.hash_code()) {}

  bool operator==(const value_ti &_rhs) const noexcept {
    return hc == _rhs.hc && ti.get() == _rhs.ti.get();
  }
};

struct unique_ptr_strategy {
  struct value_concept;
  using PIMPL = std::unique_ptr<value_concept>;

  struct value_concept {
    explicit value_concept(const std::type_info &_ti) : ti(_ti) {}
    virtual ~value_concept();
    virtual PIMPL clone() const = 0;

    value_ti ti;

    const void *get_ptr() const noexcept {
      return reinterpret_cast<const char *>(this) + sizeof(value_concept);
    }
    void *get_ptr() noexcept {
      return reinterpret_cast<char *>(this) + sizeof(value_concept);
    }
  };

  template <typename _Tp> struct value_model : public value_concept {
    template <typename _Vp,
              typename = // To prevent overriding copy/move constructor
              std::enable_if_t<
                  !std::is_base_of<value_model, std::decay_t<_Vp>>::value>>
    explicit value_model(_Vp &&_v)
        : value_concept(typeid(_Vp)), v(std::forward<_Vp>(_v)) {
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"
#endif
      static_assert(offsetof(value_model<_Tp>, v) == sizeof(value_concept));
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
    }
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

} // namespace te2::type_info_pimpl

#endif
