// decl
#ifndef TEMPLATE_CPP_DZ
#define TEMPLATE_CPP_DZ 1
#ifndef NO_TEMPLATE_IMPORT
#endif
// impl
#include "lib/io.h"
#include "lib/math/montgomery.h"
namespace modular {
namespace dynamic {
using i32 = int;
using u32 = unsigned int;
using u64 = unsigned long long;
u32 Md;
u32 r;
u32 r2;
u32 mod2;
void setDynamicMod(u32 Mod) {
  Md = Mod;
  r = mont_get_r(Md);
  r2 = -u64(Md) % Md;
  mod2 = Md << 1;
}
u32 getDynamicMod() {
  return Md;
}
struct Z {
  // implementation from https://loj.ac/s/1232785 by hly1204
  using i32 = int;
  using u32 = unsigned int;
  using u64 = unsigned long long;
  u32 v_;
  constexpr Z() : v_(0) {}
  template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
  constexpr Z(T v) : v_(mont_reduce(u64(v % i32(Md) + i32(Md)) * r2, r, Md)) {}
  constexpr Z(const Z&) = default;
  static u32 getMod() { return Md; }
  u32 get() const { return mont_norm(mont_reduce(v_, r, Md), Md); }
  template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
  explicit constexpr operator T() const {
    return T(get());
  }
  static u32 get_primitive_root_prime() {
    u32 tmp[32] = {};
    int cnt = 0;
    const u32 phi = Md - 1;
    u32 m = phi;
    for (u32 i = 2; i * i <= m; ++i) {
      if (m % i == 0) {
        tmp[cnt++] = i;
        do {
          m /= i;
        } while (m % i == 0);
      }
    }
    if (m != 1) tmp[cnt++] = m;
    for (Z res = 2;; res++) {
      bool f = true;
      for (int i = 0; i < cnt && f; ++i) f &= res.pow(phi / tmp[i]) != 1;
      if (f) return u32(res);
    }
  }
  template <typename T, std::enable_if_t<std::is_signed<T>::value, int> = 0>
  inline Z pow(T y) const {
    if (y < 0)
      y = y % (Md - 1) + (Md - 1);
    Z ret = 1, bas(*this);
    while (y) {
      if (y & 1)
        ret *= bas;
      bas *= bas;
      y >>= 1;
    }
    return ret;
  }
  template <typename T, std::enable_if_t<std::is_unsigned<T>::value, int> = 0>
  inline Z pow(T y) const {
    Z ret = 1, bas(*this);
    while (y) {
      if (y & 1)
        ret *= bas;
      bas *= bas;
      y >>= 1;
    }
    return ret;
  }
  template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
  inline i32 powVal(T y) const {
    return pow(y).get();
  }
  inline Z operator+() const {
    Z ret(v_);
    return ret;
  }
  inline Z operator-() const {
    Z ret;
    ret.v_ = (mod2 & -(v_ != 0)) - v_;
    return ret;
  }
  Z inv() const {
    i32 x1 = 1, x3 = 0, a = get(), b = Md;
    while (b) {
      i32 q = a / b, _x1 = x1, _a = a;
      x1 = x3, x3 = _x1 - x3 * q, a = b, b = _a - b * q;
    }
    return Z(x1);
  }
  inline i32 invVal() const {
    return inv().get();
  }
  inline Z operator+=(const Z& z) {
    v_ += z.v_ - mod2;
    v_ += mod2 & -(v_ >> 31);
    return *this;
  }
  inline Z operator-=(const Z& z) {
    v_ -= z.v_;
    v_ += mod2 & -(v_ >> 31);
    return *this;
  }
  inline Z operator*=(const Z& z) {
    v_ = mont_reduce(u64(v_) * z.v_, r, Md);
    return *this;
  }
  inline Z operator/=(const Z& z) {
    return operator*=(z.inv());
  }
  constexpr Z& operator=(const Z&) = default;
  friend Z operator+(const Z& lhs,
                     const Z& rhs) {
    return Z(lhs) += rhs;
  }
  friend Z operator-(const Z& lhs,
                     const Z& rhs) {
    return Z(lhs) -= rhs;
  }
  friend Z operator*(const Z& lhs,
                     const Z& rhs) {
    return Z(lhs) *= rhs;
  }
  friend Z operator/(const Z& lhs,
                     const Z& rhs) {
    return Z(lhs) /= rhs;
  }
  friend bool operator==(const Z& lhs,
                         const Z& rhs) {
    return mont_norm(lhs.v_, Md) == mont_norm(rhs.v_, Md);
  }
  friend bool operator!=(const Z& lhs,
                         const Z& rhs) {
    return mont_norm(lhs.v_, Md) != mont_norm(rhs.v_, Md);
  }
  inline Z& operator++() {
    (*this) += 1;
    return *this;
  }
  inline Z& operator--() {
    (*this) -= 1;
    return *this;
  }
  inline Z operator++(int) {
    Z result(*this);
    *this += 1;
    return result;
  }
  inline Z operator--(int) {
    Z result(*this);
    *this -= 1;
    return result;
  }
  template <typename T>
  friend inline const T& operator>>(const T& is, dZ& m) {
    long long x;
    is >> x;
    m = x;
    return is;
  }
  bool operator<(const Z& z) const {
    return get() < z.get();
  }
};
}  // namespace dynamic
}  // namespace modular
using dZ = modular::dynamic::Z;
using modular::dynamic::getDynamicMod;
using modular::dynamic::setDynamicMod;
#endif