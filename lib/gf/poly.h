// decl
#ifndef TEMPLATE_CPP_POLY
#define TEMPLATE_CPP_POLY 1
#ifndef NO_TEMPLATE_IMPORT
#include <algorithm>
#include <cmath>
#include <functional>
#include <string>
#include <vector>
using std::function;
using std::max;
using std::string;
using std::swap;
using std::vector;
#endif
// impl
#include "lib/math/log2.h"
#include "lib/math/quadratic_mod.h"
#include "lib/math/z.h"
namespace gf {
// https://loj.ac/d/3165
template <const unsigned int Mod>
inline vector<modular::Z<Mod>>& recalcInvs(int len) {
  static vector<modular::Z<Mod>> invs(2, 1);
  static int gf_inv_lim = 1;
  if (len > gf_inv_lim) {
    invs.resize(len + 1);
    for (int i = gf_inv_lim + 1; i <= len; i++)
      invs[i] = invs[Mod % i] * (Mod - Mod / i);
    gf_inv_lim = len;
  }
  return invs;
}
template <const unsigned int Mod>
inline vector<modular::Z<Mod>>& recalcRoots(const int G, int len) {
  static vector<modular::Z<Mod>> roots{0};
  static int gf_roots_lim = 0;
  if (gf_roots_lim < len) {
    int k = gf_roots_lim;
    roots.resize(1 << len);
    roots[1 << (len - 1)] = modular::Z<Mod>(G).pow((Mod - 1) >> (len + 1));
    for (int i = len - 1; i > k; i--)
      roots[1 << (i - 1)] = roots[1 << i] * roots[1 << i];
    for (int lim = 1 << k; k != len; k++, lim <<= 1)
      for (int i = lim + 1; i < (lim << 1); i++)
        roots[i] = roots[i - lim] * roots[lim];
    gf_roots_lim = len;
  }
  return roots;
}

template <const unsigned int Mod>
struct Poly {
  using Ele = modular::Z<Mod>;
  static constexpr unsigned int G = Ele::get_primitive_root_prime();
  vector<Ele> arr;
  Poly(int _n = 0) { arr.assign(_n, 0); }
  Poly(vector<Ele> v) { arr = v; }
  Poly(typename vector<Ele>::const_iterator b, typename vector<Ele>::const_iterator e) {
    arr.clear();
    for (auto it = b; it != e; it++) arr.push_back(*it);
  }
  inline Poly& resize(int _n) { return arr.resize(_n), *this; }
  inline void push_back(Ele v) { arr.push_back(v); }
  inline void reverse() { std::reverse(arr.begin(), arr.end()); }
  constexpr int getMod() { return Mod; }
  constexpr int getG() { return G; }
  inline int size() const { return static_cast<int>(arr.size()); }
  void nega() {
    for (auto& ele : arr) ele = -ele;
  }
  vector<Ele> raw() const { return arr; }
  inline Poly abst(int start = 0, int end = -1) const {
    if (end == -1) end = size();
    vector<Ele> v(end - start);
    for (int i = start; i < size() && i < end; i++)
      v[i - start] = arr[i];
    return Poly(v);
  }
  inline constexpr Ele& operator[](int idx) { return arr[idx]; }
  inline Poly& operator+=(const Poly& p) {
    resize(max(size(), p.size()));
    for (int i = 0; i < p.size(); i++)
      arr[i] += p.arr[i];
    return *this;
  }
  inline Poly& operator-=(const Poly& p) {
    resize(max(size(), p.size()));
    for (int i = 0; i < p.size(); i++)
      arr[i] -= p.arr[i];
    return *this;
  }
  inline Poly& operator*=(const Ele mult) {
    for (auto& val : arr) val *= mult;
    return *this;
  }
  inline Poly& operator/=(const Ele mult) {
    Ele dvd = Ele(1) / mult;
    for (auto& val : arr) val *= dvd;
    return *this;
  }
  inline Poly& operator<<=(const unsigned int DIG) {
    const int n = arr.size();
    arr.resize(n + DIG);
    for (unsigned int i = n + DIG - 1; !(i >> 31); i--)
      arr[i] = (i >= DIG) ? arr[i - DIG] : 0;
    return *this;
  }
  inline Poly& operator>>=(const unsigned int DIG) {
    const int n = arr.size();
    if (n <= DIG)
      arr.clear();
    else {
      for (int i = 0; i < n - DIG; i++)
        arr[i] = arr[i + DIG];
      arr.resize(n - DIG);
    }
  }
  inline void dft(int base) {
    vector<Ele> &roots = recalcRoots<Mod>(G, base);
    for (int i = size(), l; i >= 2; i >>= 1) {
      l = i >> 1;
      for (int j = 0; j != l; j++) {
        Ele u = arr[j], v = arr[j + l];
        arr[j] = u + v;
        arr[j + l] = u - v;
      }
      for (int j = i, m = 1; j != size(); j += i, ++m) {
        Ele r = roots[m];
        for (int k = 0; k != l; k++) {
          Ele u = arr[j + k], v = arr[j + k + l] * r;
          arr[j + k] = u + v;
          arr[j + k + l] = u - v;
        }
      }
    }
  }
  inline void idft(int base) {
    vector<Ele> &roots = recalcRoots<Mod>(G, base);
    for (int i = 2, l; i <= size(); i <<= 1) {
      l = i >> 1;
      for (int j = 0; j != l; j++) {
        Ele u = arr[j], v = arr[j + l];
        arr[j] = u + v;
        arr[j + l] = u - v;
      }
      for (int j = i, m = 1; j != size(); j += i, ++m) {
        Ele r = roots[m];
        for (int k = 0; k != l; k++) {
          Ele u = arr[j + k], v = arr[j + k + l];
          arr[j + k] = u + v;
          arr[j + k + l] = (u - v) * r;
        }
      }
    }
    (*this) *= Ele(Mod - ((Mod - 1) >> base));
    std::reverse(arr.begin() + 1, arr.end());
  }
  inline Poly operator*=(const Poly& P) {
    if (min(size(), P.size()) <= 8) {
      vector<Ele> nw(size() + P.size() - 1);
      for (int i = 0; i < size(); i++)
        for (int j = 0; j < P.size(); j++)
          nw[i + j] += arr[i] * P.arr[j];
      arr = nw;
      return *this;
    }
    Poly p = P;
    int targ = size() + p.size() - 1;
    int L = get_len(targ), l = get_log(L);
    resize(L), dft(l);
    if (this != &p)
      p.resize(L), p.dft(l);
    for (int i = 0; i < L; i++) arr[i] *= p[i];
    idft(l), resize(targ);
    return *this;
  }
  // 10E(n), can 9E(n)
  inline Poly inv() const {
    Poly res({arr[0].inv()});
    int len = get_len(size());
    res.resize(min(32, size()));
    for (int i = 1; i != 32 && i != size(); i++) {
      Ele sm = 0;
      for (int j = 1; j <= i; j++)
        sm -= res[i - j] * arr[j];
      res[i] = sm * res[0];
    }
    for (int i = 64, k = 6; i <= len; i <<= 1, ++k) {
      Poly f = abst(0, i);
      res.resize(i);
      Poly g = res;
      f.dft(k);
      g.dft(k);
      for (int j = 0; j < i; j++) f[j] *= g[j];
      f.idft(k);
      for (int j = 0; j < (i >> 1); j++) f[j] = 0;
      f.dft(k);
      for (int j = 0; j < i; j++) f[j] *= g[j];
      f.idft(k);
      for (int j = (i >> 1); j != i; j++) res[j] = -f[j];
    }
    return res.resize(size()), res;
  }
  // ??E(n)
  inline Poly quick_div(const Poly& p) const {
    Poly F = p.abst(0, size());
    int len = get_len(size());
    Poly g0({F[0].inv()}), q0({g0[0] * arr[0]}), _g0;
    for (int i = 2, k = 1; i <= len; i <<= 1, k++) {
      Poly f = F.abst(0, i);
      g0.resize(i);
      _g0 = g0;
      _g0.dft(k);
      q0.resize(i);
      Poly _q0 = q0;
      f.dft(k);
      _q0.dft(k);
      for (int j = 0; j < i; j++) f[j] *= _q0[j];
      f.idft(k);
      for (int j = 0; j < (i >> 1); j++) f[j] = 0;
      for (int j = (i >> 1); j < i; j++) f[j] -= (j >= size() ? 0 : arr[j]);
      f.dft(k);
      for (int j = 0; j < i; j++) f[j] *= _g0[j];
      f.idft(k);
      for (int j = (i >> 1); j < i; j++) q0[j] -= f[j];
      if (i != len) {
        f = F.abst(0, i);
        Poly g = g0;
        f.dft(k);
        for (int j = 0; j < i; j++) f[j] *= _g0[j];
        f.idft(k);
        for (int j = 0; j < (i >> 1); j++) f[j] = 0;
        f.dft(k);
        for (int j = 0; j < i; j++) f[j] *= _g0[j];
        f.idft(k);
        for (int j = (i >> 1); j != i; j++) g0[j] = -f[j];
      }
    }
    return q0.resize(size()), q0;
  }
  inline Poly derivative() const {
    Poly res(size() - 1);
    for (int i = 1; i < size(); i++)
      res[i - 1] = arr[i] * i;
    return res;
  }
  inline Poly integral() const {
    Poly res(size() + 1);
    vector<Ele> &invs = recalcInvs<Mod>(size());
    for (int i = 0; i < size(); i++)
      res[i + 1] = arr[i] * invs[i + 1];
    return res;
  }
  inline Poly ln() const {
    Poly res = derivative() / *this;
    return res.integral();
  }
  // O(n log^2 n)
  inline Poly exp() const;
  inline Poly pow(long long k) const {
    int id = -1;
    for (int i = 0; i < size(); i++)
      if (arr[i] != 0) {
        id = i;
        break;
      }
    if (id == -1 || id >= (size() + k - 1) / k)
      return Poly(size());
    Poly a = abst(id, id + size() - (id * k)) / arr[id];
    if (a.size() == 1)
      a *= arr[id].pow(k);
    else
      a = (a.ln() * Ele(k)).exp() * arr[id].pow(k);
    return a << (id * k);
  }
  inline Poly pow(string s) const {
    int id = -1;
    for (int i = 0; i < size(); i++)
      if (arr[i] != 0) {
        id = i;
        break;
      }
    if (id != -1) {
      long long tmp = 0;
      for (auto ch : s) {
        if ((tmp = tmp * 10 + ch - '0') * id >= size())
          return Poly(size());
      }
    }
    Ele r1;
    unsigned long long r2 = 0;
    for (auto ch : s) {
      r1 = r1 * 10 + (ch - '0');
      r2 = (r2 * 10 + (ch - '0')) % (Mod - 1);
    }
    unsigned int k = r1.get();
    Poly a = abst(id, id + size() - (id * k)) / arr[id];
    if (a.size() == 1)
      a *= arr[id].pow(r2);
    else
      a = (a.ln() * Ele(k)).exp() * arr[id].pow(r2);
    return a << (id * k);
  }
  // 11E(n), 10.5E(n) if inv => 9E(n)
  // 15E(n), 14E(n) if inv => 9E(n)
  inline Poly sqrt(bool inv = false) const {
    static Ele iv2 = -Ele(1) / 2;
    int len = get_len(size());
    Poly g0({quadratic_residue(arr[0])}), h0({1 / g0[0]}), tmp = g0;
    for (int i = 2, k = 1; i <= len; i <<= 1, k++) {
      Poly a = abst(0, i);
      for (int j = 0; j < (i >> 1); j++) tmp[j] *= tmp[j];
      tmp.idft(k - 1);
      tmp.resize(i);
      for (int j = 0; j != (i >> 1) - 1; j++) {
        tmp[j + (i >> 1)] = tmp[j] - a[j] - a[j + (i >> 1)];
        tmp[j] = 0;
      }
      tmp[(i >> 1) - 1] = 0;
      tmp[i - 1] -= a[i - 1];
      h0.resize(i);
      Poly _h = h0;
      _h.dft(k);
      tmp.dft(k);
      for (int j = 0; j < i; j++) tmp[j] *= _h[j];
      tmp.idft(k);
      g0.resize(i);
      for (int j = (i >> 1); j < i; j++) g0[j] = tmp[j] * iv2;
      if (i != len || inv) {
        tmp = g0;
        tmp.dft(k);
        Poly nw(i);
        for (int j = 0; j < i; j++) nw[j] = _h[j] * tmp[j];
        nw.idft(k);
        for (int j = 0; j < (i >> 1); j++) nw[j] = 0;
        nw.dft(k);
        for (int j = 0; j < i; j++) nw[j] *= _h[j];
        nw.idft(k);
        for (int j = (i >> 1); j < i; j++) h0[j] -= nw[j];
      }
    }
    if (inv)
      return h0.resize(size()), h0;
    return g0.resize(size()), g0;
  }
  inline pair<Poly, Poly> div(const Poly& P) const {
    Poly a = (*this);
    a.reverse();
    Poly p = P;
    p.reverse();
    int m = p.size();
    p.resize(size() - m + 1);
    a = a / p;
    a.resize(size() - m + 1);
    a.reverse();
    Poly r = (*this) - a * P;
    r.resize(m - 1);
    return make_pair(a, r);
  }
  friend constexpr Poly operator+(const Poly& lhs, const Poly& rhs) { return Poly(lhs) += rhs; }
  friend constexpr Poly operator-(const Poly& lhs) {
    Poly res(lhs);
    return res.nega(), res;
  }
  friend constexpr Poly operator-(const Poly& lhs, const Poly& rhs) { return Poly(lhs) -= rhs; }
  friend constexpr Poly operator*(const Poly& lhs, const Ele rhs) { return Poly(lhs) *= rhs; }
  friend constexpr Poly operator<<(const Poly& lhs, const unsigned int rhs) { return Poly(lhs) <<= rhs; }
  friend constexpr Poly operator>>(const Poly& lhs, const unsigned int rhs) { return Poly(lhs) >>= rhs; }
  friend constexpr Poly operator/(const Poly& lhs, const Ele rhs) { return Poly(lhs) /= rhs; }
  friend constexpr Poly operator*(const Poly& lhs, const Poly& rhs) { return Poly(lhs) *= rhs; }
  friend constexpr Poly operator/(const Poly& lhs, const Poly& rhs) { return Poly(lhs).quick_div(rhs); }
  void print() const {
    for (int i = 0; i < size(); i++)
      printf("%d%c", arr[i].get(), " \n"[i == size() - 1]);
  }
};

template <const unsigned int Mod, bool dynamic = false>
struct HalfOnlineConv {
  using Ele = modular::Z<Mod>;
  using Func = gf::Poly<Mod>;
  static const int BASE_BITS = 5;
  static const int CONQUER_BITS = 4;
  static const int BASE_MASK = (1 << BASE_BITS) - 1;
  static const int CONQUER_MASK = (1 << CONQUER_BITS) - 1;

  int N, n, i;
  Func f;
  vector<Ele> diagonal_sum, g_;
  vector<Ele> a;
  vector<vector<Func>> tmp_f, tmp_g;

  void init(int size, Func func) {
    f = func;
    a = f.abst(0, 1 << BASE_BITS).raw();
    if (a.size() < (1 << BASE_BITS))
      a.resize(1 << BASE_BITS);
    if constexpr (dynamic) {
      N = i = 0;
      n = 2147483647;
      diagonal_sum = vector<Ele>(2 << BASE_BITS);
      tmp_f.clear();
      tmp_g.clear();
      g_.clear();
      g_.reserve(1 << BASE_BITS);
      return;
    }
    N = n = size;
    i = 0;
    N = (N >> BASE_BITS) + (bool) (N & BASE_MASK);
    N = get_log(get_len(N));
    N = (N + CONQUER_BITS - 1) / CONQUER_BITS;
    // fixed length
    diagonal_sum = vector<Ele>(2 * n);
    g_.reserve(n);
    tmp_f = tmp_g = vector<vector<Func>>(N, vector<Func>(CONQUER_MASK));
  }
  HalfOnlineConv() = default;
  HalfOnlineConv(int _n, Func _f) { init(_n, _f); }
  HalfOnlineConv(Func _f) { init(_f.size(), _f); }
  void calc() {
    int gi = i & BASE_MASK;
    if (gi == 0) {
      // full
      int u = i >> BASE_BITS, ci = 0, csize = 1 << BASE_BITS;
      while ((u & CONQUER_MASK) == 0)
        csize <<= CONQUER_BITS, ++ci, u >>= CONQUER_BITS;
      // mid-way
      int csize2 = csize << 1, K = get_log(csize2), uid = u & CONQUER_MASK;

      // capacity
      if constexpr (dynamic) {
        if (ci >= N) {
          ++ N;
          tmp_f.push_back(vector<Func>(CONQUER_MASK));
          tmp_g.push_back(vector<Func>(CONQUER_MASK));
        }
        if (i == (i & (-i)))
          diagonal_sum.resize(i << 1);
      }

      if (uid == u) {
        Func& p = tmp_f[ci][uid - 1];
        p = f.abst((uid - 1) * csize, (uid + 1) * csize);
        p.dft(K);
      }
      vector<Func>& _g = tmp_g[ci];
      _g[uid - 1] = Func(g_.cbegin() + (i - csize), g_.cbegin() + i);
      _g[uid - 1].resize(csize2);
      _g[uid - 1].dft(K);
      Func tmp(csize2);
      for (int j = 0; j != uid; j++)
        for (int k = 0; k != csize2; k++)
          tmp[k] += tmp_f[ci][uid - 1 - j][k] * _g[j][k];
      tmp.idft(K);
      for (int j = 0; j != csize; j++)
        diagonal_sum[j + i] += tmp[csize + j];
    }
    for (int j = 1; j <= gi; j++)
      diagonal_sum[i] += a[j] * g_[i - j];
  }
  Ele get(int x) {
    if (x < 0 || x >= (int)diagonal_sum.size())
      return 0;
    return diagonal_sum[x];
  }
  Ele current() { return get(i); }
  Ele set(Ele v) {
    g_.push_back(v);
    diagonal_sum[i] += a[0] * v;
    if (++i < n) return calc(), diagonal_sum[i];
    return 0;
  }
  vector<Ele> g() { return g_; }
  vector<Ele> fg() { return diagonal_sum; }
};

template <const unsigned int Mod>
Poly<Mod> Poly<Mod>::exp() const {
  using Func = Poly<Mod>;
  if (size() == 1)
    return Func({1});
  Func d = derivative();
  d.resize(size());
  vector<Ele> &invs = recalcInvs<Mod>(size());
  HalfOnlineConv worker(d);
  Func::Ele e = worker.set(1);
  for (int i = 1; i < size(); i++)
    e = worker.set(worker.get(i - 1) * invs[i]);
  return Func(worker.g());
}

}  // namespace gf
using Poly = gf::Poly<Md3>;
using gf::HalfOnlineConv;
#endif