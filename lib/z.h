// decl
#ifndef TEMPLATE_CPP_Z
#define TEMPLATE_CPP_Z 1
#ifndef NO_TEMPLATE_IMPORT
const int Md3 = 998244353, Md7 = 1e9 + 7, Md9 = 1e9 + 9;
const int Md = Md3;
#endif

// impl
#include "lib/io.h"
namespace modular {

template <const int Md>
struct Z {
  int val;
  Z() { val = 0; }
  template <typename T>
  Z(T x) : val(x % Md) {
    if (val < 0) val += Md;
  }
  Z(const Z& x) : val(x.val) {}
  int getMod() {
    return Md;
  }
  int powVal(long long y) const {
    if (y >> 63)
      y = y % (Md - 1) + (Md - 1);
    long long ret = 1, bas = val;
    while (y) {
      if (y & 1)
        (ret *= bas) %= Md;
      (bas *= bas) %= Md;
      y >>= 1;
    }
    return (int) ret;
  }
  Z pow(long long y) const {
    if (y >> 63)
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
  Z inv() const {
    return pow(Md - 2);
  }
  int invVal() const {
    return powVal(Md - 2);
  }
  Z operator+() const {
    Z ret(val);
    return ret;
  }
  Z operator-() const {
    Z ret(Md - val);
    if (ret.val >= Md) ret.val -= Md;
    return ret;
  }

  Z operator+=(const Z& z) {
    if ((val += z.val) >= Md)
      val -= Md;
    return *this;
  }
  Z operator-=(const Z& z) {
    if ((val -= z.val) >> 31)
      val += Md;
    return *this;
  }
  Z operator*=(const Z& z) {
    val = (long long) val * z.val % Md;
    return *this;
  }
  Z operator/=(const Z& z) {
    val = (long long) val * z.invVal() % Md;
    return *this;
  }
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
    return lhs.val == rhs.val;
  }
  friend bool operator!=(const Z& lhs,
                         const Z& rhs) {
    return lhs.val != rhs.val;
  }
  Z& operator++() {
    if ((++val) >= Md)
      val -= Md;
    return *this;
  }
  Z& operator--() {
    if ((--val) >> 31)
      val += Md;
    return *this;
  }
  Z operator++(int) {
    Z result(*this);
    *this += 1;
    return result;
  }
  Z operator--(int) {
    Z result(*this);
    *this -= 1;
    return result;
  }
  friend std::ostream& operator<<(std::ostream& os, const Z& m) {
    return os << m.val;
  }
  friend std::istream& operator>>(std::istream& is, Z& m) {
    long long x;
    is >> x;
    m = x;
    return is;
  }
  friend inline const IOReader& operator>>(const IOReader& is, Z& m) {
    long long x;
    is >> x;
    m = x;
    return is;
  }
  bool operator<(const Z& z) const {
    return val < z.val;
  }
};

}  // namespace modular

using Z = modular::Z<Md>;

#endif