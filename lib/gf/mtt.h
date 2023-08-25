// decl
#ifndef TEMPLATE_CPP_MTT
#define TEMPLATE_CPP_MTT 1
#ifndef NO_TEMPLATE_IMPORT
#include <functional>
using std::function;
#endif
// impl
// only works when n <= 1e5, haha
#include "lib/gf/fft.h"
namespace gf {
template <typename T>
inline constexpr long long tint2(T val, unsigned int Mod) {
  long long v = val;
  return ((v < 0) ? (Mod + (v - 1) / 2 % Mod) : (v + 1) / 2) % Mod;
}
template <typename T = double>
inline void mtt_mul(vector<int> &f, vector<int> &g, unsigned int Mod) {
  static const int CONQUER_BIT = 16;
  static const int CONQUER_MASK = (1 << CONQUER_BIT) - 1;
  using Ele = Complex<T>;
  static const Ele I(0, 1);
  
  int u = f.size() + g.size() - 1;
  int n = f.size(), m = g.size();
  int L = get_len(u), K = get_log(L);
  vector<Ele> P(L), Q(L), _Q(L);
  for (int i = 0; i < n; i ++)
    P[i].rel() = (f[i] & CONQUER_MASK),
    P[i].img() = (f[i] >> CONQUER_BIT);
  fft(P, K);
  for (int i = 0; i < L; i ++)
    _Q[i] = P[i];
  for (int i = 0; i < m; i ++)
    Q[i].rel() = (g[i] & CONQUER_MASK),
    Q[i].img() = (g[i] >> CONQUER_BIT);
  fft(Q, K);

  P[0] *= Q[0].rel() * 2; _Q[0] *= Q[0].img() * 2;
  for (int d = 0; d < K; d ++) {
    int l = 1 << d, msk = l - 1;
    for (int i = l; i < (l << 1); i ++) {
      Ele &p = Q[i], q = Q[i ^ msk].conj();
      Ele a = (p + q), b = (q - p) * I;
      P[i] *= a; _Q[i] *= b;
    }
  }
  fft(P, K, false); fft(_Q, K, false);
  f.resize(u);
  for (int i = 0; i < u; i ++) {
    long long cur = (tint2(_Q[i].img(), Mod) << (CONQUER_BIT << 1))
      + (tint2(_Q[i].rel() + P[i].img(), Mod) << CONQUER_BIT)
      + (tint2(P[i].rel(), Mod));
    f[i] = cur % Mod;
  }
}
}
using gf::mtt_mul;
#endif