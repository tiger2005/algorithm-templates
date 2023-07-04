// decl
#ifndef TEMPLATE_CPP_CHIRP_Z
#define TEMPLATE_CPP_CHIRP_Z 1
#ifndef NO_TEMPLATE_IMPORT
#endif
// impl
#include "lib/gf/poly.h"
namespace gf {
template <const unsigned int Mod>
vector<modular::Z<Mod>> chirp_z(Poly<Mod> p, modular::Z<Mod> c, int m) {
  using Z = modular::Z<Mod>;
  int n = p.size(), l = n + m - 1, l2 = max(n, m) - 1;
  int L = get_len(l), D = get_log(L);
  p.resize(L);
  Poly<Mod> pwc(L);
  vector<Z> ipwc(l2);
  pwc[l - 2] = pwc[l - 1] = 1;
  for (int i = l - 3; i >= 0; i --)
    pwc[i] = pwc[i + 1] * c;
  for (int i = l - 3; i >= 0; i --)
    pwc[i] *= pwc[i + 1];
  Z ic = c.inv();
  ipwc[0] = 1;
  for (int i = 1; i < l2; i ++)
    ipwc[i] = ipwc[i - 1] * ic;
  for (int i = 1; i < l2; i ++)
    ipwc[i] *= ipwc[i - 1];
  for (int i = 1; i < n; i ++)
    p[i] *= ipwc[i - 1];
  p.dft(D); pwc.dft(D);
  for (int i = 0; i < L; i ++)
    p[i] *= pwc[i];
  p.idft(D);
  vector<Z> ans(m); ans[0] = p[l - 1];
  for (int i = 1; i < m; i ++)
    ans[i] = p[l - 1 - i] * ipwc[i - 1];
  return ans;
}
}
using gf::chirp_z;
#endif