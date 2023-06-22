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
#include "lib/math/log2.h"
#include "lib/math/quadratic_mod.h"
#include "lib/z.h"
namespace gf {

// https://loj.ac/d/3165
inline int get_len(int n) { return --n, n |= n >> 1, n |= n >> 2, n |= n >> 4, n |= n >> 8, n |= n >> 16, ++n; }
int gf_roots_md = 0;
vector<int> roots{0};
int gf_roots_lim = 0;
inline void recalcRoots(const int Mod, const int G, int len) {
  --len;
  if (gf_roots_lim < len || Mod != gf_roots_md) {
    if (Mod ^ gf_roots_md) {
      roots = vector<int>{0};
      gf_roots_md = Mod;
      gf_roots_lim = 0;
    }
    int k = gf_roots_lim;
    roots.resize(1 << (len));
    int _e = 1, bas = G;
    int pw = (Mod - 1) >> (len + 1);
    while (pw) {
      if (pw & 1)
        _e = (long long) _e * bas % Mod;
      bas = (long long) bas * bas % Mod;
      pw >>= 1;
    }
    roots[1 << (len - 1)] = _e;
    for (int i = len - 1; i > k; i--)
      roots[1 << (i - 1)] = (long long) roots[1 << i] * roots[1 << i] % Mod;
    for (int lim = 1 << k; k != len; k++, lim <<= 1) {
      for (int i = lim + 1; i < (lim << 1); i++) {
        roots[i] = (long long) roots[i - lim] * roots[lim] % Mod;
      }
    }
    gf_roots_lim = len;
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
  inline void resize(int _n) {
    n = _n;
    arr.resize(n);
  }
  void reverse() { std::reverse(arr.begin(), arr.end()); }
  constexpr int getMod() { return Mod; }
  constexpr int getG() { return G; }
  int size() const { return n; }
  void nega() {
    for (auto& ele : arr) ele = -ele;
  }

  vector<Ele> raw() const { return arr; }

  inline Poly abst(int start = 0, int end = -1) const {
    if (end == -1)
      end = n;
    vector<Ele> v(end - start);
    for (int i = start; i < n && i < end; i++)
      v[i - start] = arr[i];
    return Poly(v);
  }

  inline Poly copy() const {
    Poly p = (*this);
    return p;
  }

  inline Ele& operator[](int idx) {
    return arr[idx];
  }

  inline Poly operator+=(const Poly& p) {
    n = max(n, p.n);
    arr.resize(n, 0);
    for (int i = 0; i < p.n; i++)
      arr[i] += p.arr[i];
    return *this;
  }

  inline Poly operator-=(const Poly& p) {
    n = max(n, p.n);
    arr.resize(n, 0);
    for (int i = 0; i < p.n; i++)
      arr[i] -= p.arr[i];
    return *this;
  }

  inline Poly operator*=(const Ele mult) {
    for (auto& val : arr)
      val *= mult;
    return *this;
  }

  inline Poly operator/=(const Ele mult) {
    Ele dvd = Z(1) / mult;
    for (auto& val : arr)
      val *= dvd;
    return *this;
  }

  inline void dft(int base) {
    recalcRoots(Mod, G, base);
    for (int i = n, l; i >= 2; i >>= 1) {
      l = i >> 1;
      for (int j = 0; j != l; j++) {
        Z u = arr[j], v = arr[j + l];
        arr[j] = u + v;
        arr[j + l] = u - v;
      }
      for (int j = i, m = 1; j != n; j += i, ++m) {
        int r = roots[m];
        for (int k = 0; k != l; k++) {
          Z u = arr[j + k], v = arr[j + k + l] * r;
          arr[j + k] = u + v;
          arr[j + k + l] = u - v;
        }
      }
    }
  }

  inline void idft(int base) {
    for (int i = 2, l; i <= n; i <<= 1) {
      l = i >> 1;
      for (int j = 0; j != l; j++) {
        Z u = arr[j], v = arr[j + l];
        arr[j] = u + v;
        arr[j + l] = u - v;
      }
      for (int j = i, m = 1; j != n; j += i, ++m) {
        Z r = roots[m];
        for (int k = 0; k != l; k++) {
          Z u = arr[j + k], v = arr[j + k + l];
          arr[j + k] = u + v;
          arr[j + k + l] = (u - v) * r;
        }
      }
    }
    (*this) *= Z(-((Mod - 1) >> base));
    std::reverse(arr.begin() + 1, arr.end());
  }

  inline Poly operator*=(const Poly& P) {
    if (min(n, P.n) <= 8) {
      vector<Ele> nw(n + P.n - 1);
      for (int i = 0; i < n; i ++)
        for (int j = 0; j < P.n; j ++)
          nw[i + j] += arr[i] * P.arr[j];
      n = (arr = nw).size();
      return *this;
    }
    Poly p = P.copy();
    int targ = n + p.n - 1;
    int L = get_len(targ), l = get_log(L);
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

  inline Poly inv() const {
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

  inline Poly derivative() const {
    Poly res(n - 1);
    for (int i = 1; i < n; i++)
      res[i - 1] = arr[i] * i;
    return res;
  }

  inline Poly integral() const {
    Poly res(n + 1);
    for (int i = 0; i < n; i++)
      res[i + 1] = arr[i] / (i + 1);
    return res;
  }

  inline Poly ln() const {
    Poly res = derivative() * inv();
    res.resize(n - 1);
    return res.integral();
  }

  inline Poly exp() const {
    Poly res(vector<Ele>{1});
    for (int i = 2; (i >> 1) < n; i <<= 1) {
      Poly a = abst(0, i);
      res.resize(i);
      Poly l = res.ln();
      a -= l; a[0]++;
      res *= a;
      res.resize(i);
    }
    res.resize(n);
    return res;
  }

  inline Poly pow(int k) const {
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

  inline Poly pow(string s) const {
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

  inline Poly sqrt() const {
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

  inline pair<Poly, Poly> div(const Poly& P) const {
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
  friend Poly operator/(const Poly& lhs,
                        const Poly& rhs) {
    return Poly(lhs) *= rhs.inv();
  }

  void print() const {
    for (int i = 0; i < n; i++)
      printf("%d%c", arr[i].val, " \n"[i == n - 1]);
  }
};
}  // namespace gf

using Poly = gf::Poly<Md3, 3>;
#endif