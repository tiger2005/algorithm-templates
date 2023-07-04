// decl
#ifndef TEMPLATE_CPP_STIRLING2_ROW
#define TEMPLATE_CPP_STIRLING2_ROW 1
#ifndef NO_TEMPLATE_IMPORT
#endif
// impl
#include "lib/gf/poly.h"
#include "lib/math/binom.h"
namespace gf {
template <const unsigned int Mod = Md>
vector<modular::Z<Mod>> stirling2_row(int n) {
  using Z = modular::Z<Mod>;
  Inv_Factors<Mod> invf(n);
  Poly<Mod> f(n + 1), g(n + 1);
  for (int i = 0; i <= n; i ++)
    f[i] = Z(i).pow(n) * invf[i],
    g[i] = (i & 1) ? -invf[i] : invf[i];
  return f *= g, f.resize(n + 1), f.raw();
}
}
using gf::stirling2_row;
#endif