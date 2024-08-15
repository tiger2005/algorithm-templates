// decl
#ifndef TEMPLATE_CPP_Z
#define TEMPLATE_CPP_Z 1
#ifndef NO_TEMPLATE_IMPORT
const unsigned int Md3 = 998244353, Md7 = 1e9 + 7, Md9 = 1e9 + 9;
const unsigned int Md = Md3;
#endif
// impl
#include "lib/misc/io.h"
namespace modular {
template <const unsigned int mod>
struct Z {
  static_assert((mod & 1) == 1, "Mod should be odd");
  // implementation from https://loj.ac/s/1232785 by hly1204
  using i32 = int;
  using u32 = unsigned int;
  using u64 = unsigned long long;
  static constexpr u32 get_r() {
    u32 two = 2, iv = mod * (two - mod * mod);
    iv *= (two - mod * iv);
    iv *= (two - mod * iv);
    return iv * (mod * iv - two);
  }
  static constexpr u32 reduce(u64 x) { return (x + u64(u32(x) * r) * mod) >> 32; }
  static constexpr u32 norm(u32 x) { return x - (mod & -((mod - 1 - x) >> 31)); }
  static constexpr u32 r = get_r();
  static constexpr u32 r2 = -u64(mod) % mod;
  static constexpr u32 mod2 = mod << 1;
  u32 v_;
  constexpr Z() : v_(0) {}
  template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
  constexpr Z(T v) : v_(reduce(u64(v % i32(mod) + i32(mod)) * r2)) {}
  constexpr Z(const Z&) = default;

  static constexpr u32 getMod() { return mod; }
  constexpr u32 get() const { return norm(reduce(v_)); }
  template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
  explicit constexpr operator T() const { return T(get()); }
  
  static constexpr u32 get_primitive_root_prime() {
    u32 tmp[32] = {}; int cnt = 0;
    const u32 phi = mod - 1; u32 m = phi;
    for (u32 i = 2; i * i <= m; ++i) {
      if (m % i == 0) {
        tmp[cnt++] = i;
        do { m /= i; } while (m % i == 0);
      }
    }
    if (m != 1) tmp[cnt++] = m;
    for (Z res = 2;; res += 1) {
      bool f = true;
      for (int i = 0; i < cnt && f; ++i) f &= res.pow(phi / tmp[i]) != 1;
      if (f) return u32(res);
    }
  }
  template <typename T, std::enable_if_t<std::is_signed<T>::value, int> = 0>
  constexpr Z pow(T y) const {
    if (y < 0) y = y % (int)(mod - 1) + (mod - 1);
    Z bas(*this), ret = (y & 1) ? bas : 1;
    while (y >>= 1) {
      bas *= bas;
      if (y & 1) ret *= bas;
    }
    return ret;
  }
  template <typename T, std::enable_if_t<std::is_unsigned<T>::value, int> = 0>
  constexpr Z pow(T y) const {
    Z bas(*this), ret = (y & 1) ? bas : 1;
    while (y >>= 1) {
      bas *= bas;
      if (y & 1) ret *= bas;
    }
    return ret;
  }
  template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
  constexpr i32 powVal(T y) const { return pow(y).get(); }
  constexpr Z operator+() const { Z ret(*this); return ret; }
  constexpr Z operator-() const { Z ret; return ret.v_ = (mod2 & -(v_ != 0)) - v_, ret; }
  constexpr Z inv() const {
    i32 x1 = 1, x3 = 0, a = get(), b = mod;
    while (b) {
      i32 q = a / b, _x1 = x1, _a = a;
      x1 = x3, x3 = _x1 - x3 * q, a = b, b = _a - b * q;
    }
    return Z(x1);
  }
  constexpr i32 invVal() const { return inv().get(); }
  constexpr Z operator+=(const Z& z) { return v_ += z.v_ - mod2, v_ += mod2 & -(v_ >> 31), *this; }
  constexpr Z operator-=(const Z& z) { return v_ -= z.v_, v_ += mod2 & -(v_ >> 31), *this; }
  constexpr Z operator*=(const Z& z) { return v_ = reduce(u64(v_) * z.v_), *this; }
  constexpr Z operator/=(const Z& z) { return operator*=(z.inv()); }
  constexpr Z& operator=(const Z&) = default;
  friend constexpr Z operator+(const Z& lhs, const Z& rhs) { return Z(lhs) += rhs; }
  friend constexpr Z operator-(const Z& lhs, const Z& rhs) { return Z(lhs) -= rhs; }
  friend constexpr Z operator*(const Z& lhs, const Z& rhs) { return Z(lhs) *= rhs; }
  friend constexpr Z operator/(const Z& lhs, const Z& rhs) { return Z(lhs) /= rhs; }
  friend constexpr bool operator==(const Z& lhs, const Z& rhs) { return norm(lhs.v_) == norm(rhs.v_); }
  friend constexpr bool operator!=(const Z& lhs, const Z& rhs) { return norm(lhs.v_) != norm(rhs.v_); }
  inline Z& operator++() { return (*this) += 1, *this; }
  inline Z& operator--() { return (*this) -= 1, *this; }
  inline Z operator++(int) { Z result(*this); *this += 1; return result; }
  inline Z operator--(int) { Z result(*this); *this -= 1; return result; }
  template <typename T>
  friend inline const T& operator>>(const T& is, Z& m) { long long x; return is >> x, m = x, is; }
  bool operator<(const Z& z) const { return get() < z.get(); }
};
}  // namespace modular
using Z = modular::Z<Md>;
#endif