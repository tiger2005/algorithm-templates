// decl
#ifndef TEMPLATE_CPP_STIRLING1_COL
#define TEMPLATE_CPP_STIRLING1_COL 1
#ifndef NO_TEMPLATE_IMPORT
#endif
// impl
#include "lib/gf/poly.h"
#include "lib/math/binom.h"
namespace gf {
template <const unsigned int Mod = Md>
vector<modular::Z<Mod>> stirling1_col(int n, int k) {
  using Z = modular::Z<Mod>;
  if (k > n)
    return vector<Z>(n + 1);
  int l = n - k;
  Inv_Factors<Mod> invf(max(k, l + 1));
  Factors<Z> fact(n);
  // invf contains inv, re-use in template
  Poly<Mod> p(l + 1);
  for (int i = 0; i <= l; i ++)
    p[i] = invf.invs[i + 1];
  p = p.pow(k) << k;
  p *= invf[k];
  for (int i = k; i <= n; i ++)
    p[i] *= fact[i];
  return p.raw();
}
}
using gf::stirling1_col;
#endif