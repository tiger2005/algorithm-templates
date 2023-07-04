// decl
#ifndef TEMPLATE_CPP_POLY_SHIFT
#define TEMPLATE_CPP_POLY_SHIFT 1
#ifndef NO_TEMPLATE_IMPORT
#endif
// impl
#include "lib/gf/poly.h"
#include "lib/math/binom.h"
namespace gf {
template <const unsigned int Mod = Md>
Poly<Mod> poly_shift(Poly<Mod> p, modular::Z<Mod> c) {
  using Z = modular::Z<Mod>;
  using Poly = Poly<Mod>;
  int n = p.size() - 1;
  Poly a(n + 1), b(n + 1);
  Inv_Factors<Mod> invf(n);
  Factors<Z> fact(n);
  Z cur = 1;
  for (int i = 0; i <= n; i ++)
    a[i] = cur * invf[i], cur *= c,
    b[i] = p[n - i] * fact[n - i];
  a *= b;
  Poly res(n + 1);
  for (int i = 0; i <= n; i ++)
    res[i] = a[n - i] * invf[i];
  return res;
}
}
#endif