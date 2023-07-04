// decl
#ifndef TEMPLATE_CPP_DEC_POWER
#define TEMPLATE_CPP_DEC_POWER 1
#ifndef NO_TEMPLATE_IMPORT
#endif
// impl
#include "lib/gf/poly_shift.h"
namespace gf {
template <const unsigned int Mod = Md>
Poly<Mod> dec_power(int n) {
  using Z = modular::Z<Mod>;
  if (n == 0)
    return Poly<Mod>({Z(1)});
  int t = n >> 1;
  Poly<Mod> u = dec_power<Mod>(t);
  u *= poly_shift<Mod>(u, -t);
  if (n & 1)
    u *= Poly<Mod>({Z(1 - n), Z(1)});
  return u;
}
template <const unsigned int Mod = Md>
Poly<Mod> dec_field_mul(Poly<Mod> f, Poly<Mod> g) {
  using Z = modular::Z<Mod>;
  int n = f.size() + g.size() - 1;
  int L = get_len(2 * n - 1), K = get_log(L);
  Inv_Factors<Mod> invf(n - 1);
  Factors<Z> fact(n - 1);
  f.resize(L), g.resize(L);
  Poly<Mod> ex(L), _ex(L);
  for (int i = 0; i < n; i ++)
    ex[i] = invf[i], _ex[i] = (i & 1) ? -invf[i] : invf[i];
  f.dft(K); g.dft(K); ex.dft(K);
  for (int i = 0; i < L; i ++)
    f[i] *= ex[i], g[i] *= ex[i];
  f.idft(K); g.idft(K); f.resize(n);
  for (int i = 0; i < n; i ++)
    f[i] *= g[i] * fact[i];
  return f *= _ex, f.resize(n), f;
}
}
using gf::dec_power;
using gf::dec_field_mul;
#endif