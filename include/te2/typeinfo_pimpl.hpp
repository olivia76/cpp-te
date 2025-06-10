/**
 *  \author  Olivia Quinet
 *  \file te2/pimpl.hpp
 */

#ifndef TE2_TYPEINFO_PIMPL_HPP
#define TE2_TYPEINFO_PIMPL_HPP

#include <memory>
#include <type_traits>
#include <typeindex>

namespace te2::type_info_pimpl {

class value_ti {
  using TypeInfoRef = std::reference_wrapper<const std::type_info>;

  TypeInfoRef ti;
  size_t hc{};

  constexpr value_ti() : ti{typeid(void)} {}
  explicit value_ti(const std::type_info &ti_) : ti(ti_), hc(ti_.hash_code()) {}

  template <typename TypeT> static value_ti _get() {
    static value_ti tir(typeid(TypeT));
    return tir;
  }

public:
  template <typename TypeT> static value_ti get() {
    using TP = std::remove_const_t<std::remove_reference_t<TypeT>>;
    return _get<TP>();
  }

  bool operator==(const value_ti &rhs) const noexcept {
    // Experimental : Two different types can have the same hash_code!!!  Remove
    // the commented part if needed.
    return hc == rhs.hc && ti.get() == rhs.ti.get();
  }
};

struct unique_ptr_strategy {
  struct value_concept;
  using PIMPL = std::unique_ptr<value_concept>;

  struct value_concept {
    explicit value_concept(value_ti ti_) : ti(ti_) {}
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

  template <typename ValueT> struct value_model : public value_concept {
    template <typename Vp>
    requires (!std::is_base_of<value_concept, std::decay_t<Vp>>::value)
    explicit value_model(Vp &&vp)
        : value_concept(value_ti::get<Vp>()), value(std::forward<Vp>(vp)) {
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"
#endif
      static_assert(offsetof(value_model<ValueT>, value) ==
                    sizeof(value_concept));
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
    }
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
    static const VT *ptr(const void *p) noexcept {
      return std::addressof(value(p));
    }
    static VT *ptr(void *p) noexcept { return std::addressof(value(p)); }
    static const VT &value(const void *p) noexcept {
      return static_cast<const VM *>(p)->value;
    }
    static VT &value(void *p) noexcept { return static_cast<VM *>(p)->value; }
  };
};

} // namespace te2::type_info_pimpl

#endif
