// decl
#ifndef TEMPLATE_CPP_FFT
#define TEMPLATE_CPP_FFT 1
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
#include <algorithm>
using std::vector;
#endif
// impl
#include "lib/math/log2.h"
#include "lib/math/complex.h"
namespace gf {
template <typename T = double>
inline void extend_roots_fft(vector<Complex<T>> &t, int &plen, int len) {
  static const double PI2 = PI / 2;
  if (plen >= len)
    return;
  t.resize(1 << len);
  for (int l = plen; l < len; l ++) {
    int L = 1 << l;
    t[L] = get_comp<T>(PI2 / L);
    for (int j = L + 1; j < (L << 1); j ++)
      t[j] = t[j - L] * t[L];
  }
  plen = len;
}
// copy from lib/gf/poly.h
template <typename T = double>
inline void fft(vector<Complex<T>> &arr, int base, bool forw = true) {
  using Ele = Complex<T>;
  static vector<Ele> roots({T()});
  static int len = 0;
  extend_roots_fft(roots, len, base - 1);
  int n = arr.size();
  if (forw) {
    for (int i = n, l; i >= 2; i >>= 1) {
      l = i >> 1;
      for (int j = 0; j != l; j++) {
        Ele u = arr[j], v = arr[j + l];
        arr[j] = u + v; arr[j + l] = u - v;
      }
      for (int j = i, m = 1; j != n; j += i, ++m) {
        Ele r = roots[m];
        for (int k = 0; k != l; k++) {
          Ele u = arr[j + k], v = arr[j + k + l] * r;
          arr[j + k] = u + v; arr[j + k + l] = u - v;
        }
      }
    }
  }
  else {
    for (int i = 2, l; i <= n; i <<= 1) {
      l = i >> 1;
      for (int j = 0; j != l; j++) {
        Ele u = arr[j], v = arr[j + l];
        arr[j] = u + v; arr[j + l] = u - v;
      }
      for (int j = i, m = 1; j != n; j += i, ++m) {
        Ele r = roots[m];
        for (int k = 0; k != l; k++) {
          Ele u = arr[j + k], v = arr[j + k + l];
          arr[j + k] = u + v; arr[j + k + l] = (u - v) * r;
        }
      }
    }
    T inv_n = T(1) / n;
    for (auto &e: arr) e *= inv_n;
    std::reverse(arr.begin() + 1, arr.end());
  }
}
template <typename T = double>
inline void fft_mul_inplace(vector<Complex<T>> &f, vector<Complex<T>> &g) {
  int u = f.size() + g.size() - 1;
  if (std::min(f.size(), g.size()) <= 8) {
    vector<Complex<T>> tmp(u);
    int p = f.size(); f.resize(u);
    for (int i = p - 1; i >= 0; i --)
      for (int j = g.size() - 1; j >= 0; j --)
        tmp[i + j] += f[i] * g[j];
    return f = tmp, void();
  }
  int L = get_len(u), K = get_log(L);
  f.resize(L), g.resize(L);
  fft(f, K); fft(g, K);
  for (int i = 0; i < L; i ++)
    f[i] *= g[i];
  fft(f, K, false); f.resize(u);
}
template <typename T = double>
inline void fft_mul(vector<Complex<T>> &f, const vector<Complex<T>> &g) {
  return fft_mul_inplace<T>(f, vector<Complex<T>>(g));
}
}
using gf::fft_mul_inplace;
using gf::fft_mul;
#endif