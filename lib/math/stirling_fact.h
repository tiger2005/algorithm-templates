// decl
#ifndef TEMPLATE_CPP_STIRLING_FACT
#define TEMPLATE_CPP_STIRLING_FACT 1
#ifndef NO_TEMPLATE_IMPORT
#endif
// impl
#include "lib/math/binom.h"
namespace math {
template <const unsigned int Mod = Md>
struct stirling2_fact {
  using T = modular::Z<Mod>;
  Inv_Factors<Mod> invf;
  stirling2_fact(int n = 0) {
    invf.assign(n);
  };
  T get(int n, int m) {
    if (n < m || m < 0)
      return T();
    if (n == 0)
      return T(1);
    T res(0); invf.assign(n);
    for (int i = 1; i <= m; i ++) {
      if ((m - i) & 1)
        res -= T(i).pow(n) * invf[i] * invf[m - i];
      else
        res += T(i).pow(n) * invf[i] * invf[m - i];
    }
    return res;
  }
};
}
using math::stirling2_fact;
#endif