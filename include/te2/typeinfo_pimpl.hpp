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

struct value_ti {
  using TypeInfoRef = std::reference_wrapper<const std::type_info>;

  TypeInfoRef ti;
  size_t hc{};

  value_ti() : ti{typeid(void)} {}
  value_ti(const std::type_info &ti_) : ti(ti_), hc(ti_.hash_code()) {}

  bool operator==(const value_ti &rhs) const noexcept {
    return hc == rhs.hc && ti.get() == rhs.ti.get();
  }
};

struct unique_ptr_strategy {
  struct value_concept;
  using PIMPL = std::unique_ptr<value_concept>;

  struct value_concept {
    explicit value_concept(const std::type_info &ti_) : ti(ti_) {}
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

  template <typename Tp> struct value_model : public value_concept {
    template <typename Vp,
              typename = // To prevent overriding copy/move constructor
              std::enable_if_t<
                  !std::is_base_of<value_model, std::decay_t<Vp>>::value>>
    explicit value_model(Vp &&vp)
        : value_concept(typeid(Vp)), v(std::forward<Vp>(vp)) {
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"
#endif
      static_assert(offsetof(value_model<Tp>, v) == sizeof(value_concept));
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
    }
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

} // namespace te2::type_info_pimpl

#endif
