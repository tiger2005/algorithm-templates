// decl
#ifndef TEMPLATE_CPP_INTERPOLATE
#define TEMPLATE_CPP_INTERPOLATE 1
#ifndef NO_TEMPLATE_IMPORT
#include <functional>
using std::function;
using std::vector;
#endif
// impl
#include "lib/gf/multipoint.h"
namespace gf {
template <const unsigned int Mod>
Poly<Mod> interpolate(vector<modular::Z<Mod>> X, vector<modular::Z<Mod>> Y) {
  using Ele = modular::Z<Mod>;
  using Poly = Poly<Mod>;
  int n = X.size();
  vector<Poly> Q(4 * n + 1);
  auto dfs_Q = [&X, &Q](auto self, int x, int l, int r) {
    if (l == r)
      return Q[x] = Poly(vector<Ele>{-X[l], 1});
    int m = (l + r) >> 1;
    return Q[x] = self(self, x << 1, l, m) * self(self, x << 1 | 1, m + 1, r);
  };
  vector<Ele> q = multipoint(dfs_Q(dfs_Q, 1, 0, n - 1).derivative(), X);
  vector<Ele> T;
  for (int i = 0; i < n; i++)
    T.push_back(Y[i] / q[i]);
  auto dfs = [&Q, &T](auto self, int x, int l, int r) {
    if (l == r)
      return Poly(vector<Ele>{T[l]});
    int m = (l + r) >> 1;
    return self(self, x << 1, l, m) * Q[x << 1 | 1] + self(self, x << 1 | 1, m + 1, r) * Q[x << 1];
  };
  return dfs(dfs, 1, 0, n - 1);
}
}  // namespace gf
using gf::interpolate;
#endif