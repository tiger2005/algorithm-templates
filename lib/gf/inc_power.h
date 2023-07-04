// decl
#ifndef TEMPLATE_CPP_INC_POWER
#define TEMPLATE_CPP_INC_POWER 1
#ifndef NO_TEMPLATE_IMPORT
#endif
// impl
#include "lib/gf/dec_power.h"
namespace gf {
template <const unsigned int Mod = Md>
Poly<Mod> inc_power(int n) {
  using Z = modular::Z<Mod>;
  if (n == 0)
    return Poly<Mod>({Z(1)});
  int t = n >> 1;
  Poly<Mod> u = inc_power<Mod>(t);
  u *= poly_shift<Mod>(u, t);
  if (n & 1)
    u *= Poly<Mod>({Z(n - 1), Z(1)});
  return u;
}
template <const unsigned int Mod = Md>
Poly<Mod> inc_field_mul(Poly<Mod> f, Poly<Mod> g) {
  for (int i = 1, l = f.size(); i < l; i += 2) f[i] = -f[i];
  for (int i = 1, l = g.size(); i < l; i += 2) g[i] = -g[i];
  Poly<Mod> res = dec_field_mul(f, g);
  for (int i = 1, l = res.size(); i < l; i += 2) res[i] = -res[i];
  return res;
}
}
using gf::inc_power;
using gf::inc_field_mul;
#endif