// decl
#ifndef TEMPLATE_CPP_POLY
#define TEMPLATE_CPP_POLY 1
#ifndef NO_TEMPLATE_IMPORT
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
using std::max;
using std::string;
using std::swap;
using std::vector;
#endif

// impl
#include "lib/math/quadratic_mod.h"
#include "lib/z.h"
namespace gf {
vector<int> inverse;
int gf_roots_md = 0;
vector<int> roots{0, 1};
void recalcInverse(const int Mod, const int G, int len) {
  if (inverse.size() != (1ull << len)) {
    inverse.resize(1 << len, 0);
    for (int i = 0; i < (1 << len); i++)
      inverse[i] = (inverse[i >> 1] >> 1) ^ ((i & 1) << (len - 1));
  }
  if ((int) roots.size() < (1 << len) || Mod != gf_roots_md) {
    if (Mod ^ gf_roots_md) {
      roots = vector<int>{0, 1};
      gf_roots_md = Mod;
    }
    int k = 0;
    while ((1 << k) < (int) roots.size())
      ++k;
    roots.resize(1 << len);
    while (k != len) {
      int e = 1, bas = G;
      int pw = (Mod - 1) >> (k + 1);
      while (pw) {
        if (pw & 1)
          e = (long long) e * bas % Mod;
        bas = (long long) bas * bas % Mod;
        pw >>= 1;
      }
      for (int i = 1 << (k - 1); i < 1 << k; i++) {
        roots[i << 1] = roots[i];
        roots[i << 1 | 1] = (long long) roots[i] * e % Mod;
      }
      ++k;
    }
  }
}

template <const int Mod, const int G>
struct Poly {
  using Ele = modular::Z<Mod>;
  int n;
  vector<Ele> arr;
  Poly(int _n = 0) {
    arr.clear();
    resize(_n);
  }
  Poly(vector<Ele> v) {
    arr = v;
    n = arr.size();
  }
  void resize(int _n) {
    n = _n;
    arr.resize(n);
  }
  void reverse() { std::reverse(arr.begin(), arr.end()); }
  constexpr int getMod() { return Mod; }
  constexpr int getG() { return G; }
  int size() { return n; }

  vector<Ele> raw() const { return arr; }

  Poly abst(int start = 0, int end = -1) const {
    if (end == -1)
      end = n;
    vector<Ele> v(end - start);
    for (int i = start; i < n && i < end; i++)
      v[i - start] = arr[i];
    return Poly(v);
  }

  Poly copy() const {
    Poly p = (*this);
    return p;
  }

  Ele& operator[](int idx) {
    return arr[idx];
  }

  Poly operator+=(const Poly& p) {
    n = max(n, p.n);
    arr.resize(n, 0);
    for (int i = 0; i < p.n; i++)
      arr[i] += p.arr[i];
    return *this;
  }

  Poly operator-=(const Poly& p) {
    n = max(n, p.n);
    arr.resize(n, 0);
    for (int i = 0; i < p.n; i++)
      arr[i] -= p.arr[i];
    return *this;
  }

  Poly operator*=(const Ele mult) {
    for (auto& val : arr)
      val *= mult;
    return *this;
  }

  Poly operator/=(const Ele mult) {
    Ele dvd = Z(1) / mult;
    for (auto& val : arr)
      val *= dvd;
    return *this;
  }

  void dft(int base) {
    recalcInverse(Mod, G, base);
    for (int i = 0; i < n; i++)
      if (i > inverse[i])
        swap(arr[i], arr[inverse[i]]);
    for (int m = 1; m < n; m <<= 1) {
      int r = m << 1;
      for (int i = 0; i < n; i += r) {
        for (auto j = 0; j < m; j++) {
          Ele x = arr[i + j], y = roots[m + j] * arr[i + j + m];
          arr[i + j] = x + y;
          arr[i + j + m] = x - y;
        }
      }
    }
  }

  void idft(int base) {
    std::reverse(arr.begin() + 1, arr.end());
    dft(base);
    (*this) *= Z(-((Mod - 1) >> base));
  }

  Poly operator*=(const Poly& P) {
    Poly p = P.copy();
    int L = 1, l = 0, targ = n + p.n - 1;
    while (L <= targ)
      L <<= 1, ++l;
    resize(L);
    dft(l);
    if (this != &p) {
      p.resize(L);
      p.dft(l);
    }
    for (int i = 0; i < L; i++)
      arr[i] *= p[i];
    idft(l);
    resize(targ);
    return *this;
  }

  Poly inv() {
    Poly res({arr[0].inv()});
    for (int i = 2, k = 2; (i >> 1) < n; i <<= 1, ++k) {
      Poly a = abst(0, i);
      a.resize(i << 1);
      res.resize(i << 1);
      a.dft(k);
      res.dft(k);
      for (int j = 0; j < (i << 1); j++)
        res[j] *= (2 - a[j] * res[j]);
      res.idft(k);
      res.resize(i);
    }
    res.resize(n);
    return res;
  }

  Poly derivative() {
    Poly res(n - 1);
    for (int i = 1; i < n; i++)
      res[i - 1] = arr[i] * i;
    return res;
  }

  Poly integral() {
    Poly res(n + 1);
    for (int i = 0; i < n; i++)
      res[i + 1] = arr[i] / (i + 1);
    return res;
  }

  Poly ln() {
    Poly res = derivative() * inv();
    res.resize(n - 1);
    return res.integral();
  }

  Poly exp() {
    Poly res(vector<Ele>{1});
    for (int i = 2; (i >> 1) < n; i <<= 1) {
      Poly a = abst(0, i);
      res.resize(i);
      Poly l = res.ln();
      l = Poly(vector<Ele>{1}) - l + a;
      res *= l;
      res.resize(i);
    }
    res.resize(n);
    return res;
  }

  Poly pow(int k) {
    int id = -1;
    for (int i = 0; i < n; i++)
      if (arr[i] != 0) {
        id = i;
        break;
      }
    if (id == -1 || (long long) id * k >= n)
      return Poly(n);
    Poly a = abst(id, id + n) / arr[id];
    a = (a.ln() * Ele(k)).exp() * arr[id].pow(k);
    Poly res(n);
    for (int i = id * k, j = 0; i < n; i++, j++)
      res[i] = a[j];
    return res;
  }

  Poly pow(string s) {
    int id = -1;
    for (int i = 0; i < n; i++)
      if (arr[i] != 0) {
        id = i;
        break;
      }
    if (id != -1) {
      long long tmp = 0;
      for (auto ch : s) {
        if ((tmp = tmp * 10 + ch - '0') * id >= n)
          return Poly(n);
      }
    }
    Ele r1;
    modular::Z<Mod - 1> r2;
    for (auto ch : s) {
      r1 = r1 * 10 + (ch - '0');
      r2 = r2 * 10 + (ch - '0');
    }
    int k = r1.val;
    Poly a = abst(id, id + n) / arr[id];
    a = (a.ln() * Ele(k)).exp() * arr[id].pow(r2.val);
    Poly res(n);
    for (int i = id * k, j = 0; i < n; i++, j++)
      res[i] = a[j];
    return res;
  }

  Poly sqrt() {
    Poly res(vector<Ele>{quadratic_residue(arr[0])});
    for (int i = 2; (i >> 1) < n; i <<= 1) {
      Poly a = abst(0, i);
      res.resize(i);
      Poly u = res.inv();
      res = res + a * u;
      res /= 2;
      res.resize(i);
    }
    res.resize(n);
    return res;
  }

  pair<Poly, Poly> div(const Poly& P) {
    Poly a = copy();
    a.reverse();
    Poly p = P.copy();
    p.reverse();
    int m = p.size();
    p.resize(n - m + 1);
    a *= p.inv();
    a.resize(n - m + 1);
    a.reverse();
    Poly r = (*this) - a * P;
    r.resize(m - 1);
    return make_pair(a, r);
  }

  friend Poly operator+(const Poly& lhs,
                        const Poly& rhs) {
    return Poly(lhs) += rhs;
  }
  friend Poly operator-(const Poly& lhs,
                        const Poly& rhs) {
    return Poly(lhs) -= rhs;
  }
  friend Poly operator*(const Poly& lhs,
                        const Ele rhs) {
    return Poly(lhs) *= rhs;
  }
  friend Poly operator/(const Poly& lhs,
                        const Ele rhs) {
    return Poly(lhs) /= rhs;
  }
  friend Poly operator*(const Poly& lhs,
                        const Poly& rhs) {
    return Poly(lhs) *= rhs;
  }

  void print() const {
    for (int i = 0; i < n; i++)
      printf("%d%c", arr[i].val, " \n"[i == n - 1]);
  }
};
}  // namespace gf

using Poly = gf::Poly<Md3, 3>;
#endif