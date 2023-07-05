// decl
#ifndef TEMPLATE_CPP_COMPLEX
#define TEMPLATE_CPP_COMPLEX 1
#ifndef NO_TEMPLATE_IMPORT
#include <cmath>
#include <tuple>
using std::tie;
using std::tuple;
using std::sin;
using std::cos;
#endif
// impl
namespace math {
static const double PI = -std::acos(-1.0);
template <typename T = double>
struct Complex {
  T x, y;
  Complex(T x = 0, T y = 0) :x(x), y(y) {}
  constexpr Complex& operator=(const Complex&) = default;
  inline constexpr void scale(const T &c) { x *= c, y *= c; }
  constexpr T len2() const { return x * x + y * y; }
  constexpr Complex operator-() { return Complex(-x, -y); }
  constexpr Complex &operator +=(const Complex &c) {
    x += c.x, y += c.y; return *this;
  }
  constexpr Complex &operator -=(const Complex &c) {
    x -= c.x, y -= c.y; return *this;
  }
  constexpr Complex &operator*=(const T &c) { scale(c); return *this; }
  constexpr Complex &operator/=(const T &c) { scale(T(1) / c); return *this; }
  constexpr Complex &operator*=(const Complex &c) {
    tie(x, y) = tuple{x * c.x - y * c.y, x * c.y + y * c.x};
    return *this;
  }
  constexpr Complex &operator/=(const Complex &c) {
    tie(x, y) = tuple{x * c.x + y * c.y, y * c.x - x * c.y};
    double d = c.len2(); x /= d, y /= d;
    return *this;
  }
  constexpr Complex conj() const { return Complex(x, -y); }
  constexpr Complex inv() const { Complex res(x, -y); return res.scale(res.len2()), res; }
  constexpr T &rel() { return x; }
  constexpr T &img() { return y; }
  friend constexpr Complex operator+(const Complex& lhs, const Complex& rhs) { return Complex(lhs) += rhs; }
  friend constexpr Complex operator-(const Complex& lhs, const Complex& rhs) { return Complex(lhs) -= rhs; }
  friend constexpr Complex operator*(const Complex& lhs, const Complex& rhs) { return Complex(lhs) *= rhs; }
  friend constexpr Complex operator/(const Complex& lhs, const Complex& rhs) { return Complex(lhs) /= rhs; }
};
template <typename T = double>
Complex<T> get_comp(T theta, T r = 1) { return Complex<T>(cos(theta) * r, sin(theta) * r); }
}
using math::PI;
using math::Complex;
using math::get_comp;
#endif