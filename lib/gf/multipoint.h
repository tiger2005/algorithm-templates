// decl
#ifndef TEMPLATE_CPP_MULTIPOINT
#define TEMPLATE_CPP_MULTIPOINT 1
#ifndef NO_TEMPLATE_IMPORT
#include <functional>
using std::function;
using std::max;
using std::vector;
#endif
// impl
#include "lib/gf/poly.h"
#include "lib/math/z.h"
namespace gf {
template <const unsigned int Mod>
Poly<Mod> mul_T(Poly<Mod> a, Poly<Mod> b) {
  b.reverse();
  int n = a.size(), m = b.size();
  a *= b;
  return a.abst(m - 1, n + m - 1);
}
template <const unsigned int Mod>
vector<modular::Z<Mod>> multipoint(Poly<Mod> f, vector<modular::Z<Mod>> v) {
  using Ele = modular::Z<Mod>;
  int N = v.size();
  int n = max(f.size(), (int) v.size());
  f.resize(n + 1);
  v.resize(n);
  vector<Ele> res(n);
  vector<Poly<Mod>> Q(4 * n + 1);
  auto dfs_Q = [&](auto self, int x, int l, int r) {
    if (l == r) {
      Q[x] = Poly<Mod>(vector<Ele>{1, -v[l]});
      return;
    }
    int m = (l + r) >> 1;
    self(self, x << 1, l, m);
    self(self, x << 1 | 1, m + 1, r);
    Q[x] = Q[x << 1] * Q[x << 1 | 1];
  };
  dfs_Q(dfs_Q, 1, 0, n - 1);
  auto dfs = [&](auto self, int x, int l, int r, auto F) {
    F.resize(r - l + 1);
    if (l == r) {
      res[l] = F[0];
      return;
    }
    int m = (l + r) >> 1;
    self(self, x << 1, l, m, mul_T(F, Q[x << 1 | 1]));
    self(self, x << 1 | 1, m + 1, r, mul_T(F, Q[x << 1]));
  };
  dfs(dfs, 1, 0, n - 1, mul_T(f, Q[1].inv()));
  res.resize(N);
  return res;
}
}  // namespace gf
using gf::multipoint;
#endif