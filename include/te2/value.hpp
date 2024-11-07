/**
 *  \author  Olivia Quinet
 *  \file te2/value.hpp
 */

#ifndef _TE2_VALUE_HPP_
#define _TE2_VALUE_HPP_

#include <memory>
#include <type_traits>
#include <typeindex>

namespace te2::detail {

struct value_base;
using PIMPL = std::unique_ptr<value_base>;

template <typename _Vp, typename _Tp> inline auto make_pimpl(_Tp &&_x) {
  return std::make_unique<_Vp>(std::forward<_Tp>(_x));
}

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

struct value_base {
  explicit value_base(const std::type_info &_ti) : ti(_ti) {}
  virtual ~value_base();
  virtual PIMPL clone() const = 0;

  value_ti ti;
};

template <typename _Tp> struct value : public value_base {
  template <typename _Vp,
            typename = // To prevent overriding copy/move constructor
            std::enable_if_t<!std::is_base_of<value, std::decay_t<_Vp>>::value>>
  explicit value(_Vp &&_v) : value_base(typeid(_Vp)), v(std::forward<_Vp>(_v)) {
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"
#endif
    static_assert(offsetof(value<_Tp>, v) == sizeof(value_base));
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
  }
  value(const value &) = default;
  virtual PIMPL clone() const { return make_pimpl<value>(*this); }
  _Tp v;
};

} // namespace te2::detail

#endif
