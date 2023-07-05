// decl
#ifndef TEMPLATE_CPP_POLY_TRANS
#define TEMPLATE_CPP_POLY_TRANS 1
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
using std::vector;
using std::pair;
#endif
// impl
#include "lib/gf/poly.h"
#include "lib/math/binom.h"
#include "lib/gf/multipoint.h"
#include "lib/gf/interpolate.h"
namespace gf {
template <const unsigned int Mod = Md>
Poly<Mod> norm_to_dec(Poly<Mod> f) {
  using Z = modular::Z<Mod>;
  // use inverse method of multipoint here
  // better performance than divide-and-conquer
  int n = f.size();
  vector<Z> p(n);
  for (int i = 0; i < n; i ++) p[i] = i;
  Poly<Mod> pval(multipoint(f, p));
  Inv_Factors<Mod> invf(n);
  Poly<Mod> _ex(n);
  for (int i = 0; i < n; i ++)
    _ex[i] = (i & 1) ? -invf[i] : invf[i],
    pval[i] *= invf[i];
  return pval *= _ex, pval.resize(n), pval;
}
template <const unsigned int Mod = Md>
Poly<Mod> norm_to_inc(Poly<Mod> f) {
  for (int i = 1, l = f.size(); i < l; i += 2) f[i] = -f[i];
  f = norm_to_dec(f);
  for (int i = 1, l = f.size(); i < l; i += 2) f[i] = -f[i];
  return f;
}
template <const unsigned int Mod = Md>
Poly<Mod> dec_to_norm(Poly<Mod> f) {
  using Z = modular::Z<Mod>;
  using Poly = Poly<Mod>;
  // use divide-and-conquer method here
  auto dfs = [&] (auto self, int l, int r, bool first = true) -> pair<Poly, Poly> {
    if (l == r)
      return {Poly({Z(-l), Z(1)}), Poly({Z(f[l])})};
    int m = (l + r) >> 1;
    auto L = self(self, l, m, false), R = self(self, m + 1, r, false);
    return {first ? Poly() : L.first * R.first, L.first * R.second + L.second};
  };
  return dfs(dfs, 0, f.size() - 1).second;
}
template <const unsigned int Mod = Md>
Poly<Mod> inc_to_norm(Poly<Mod> f) {
  for (int i = 1, l = f.size(); i < l; i += 2) f[i] = -f[i];
  f = dec_to_norm(f);
  for (int i = 1, l = f.size(); i < l; i += 2) f[i] = -f[i];
  return f;
}
}
using gf::norm_to_dec;
using gf::norm_to_inc;
using gf::dec_to_norm;
using gf::inc_to_norm;
#endif