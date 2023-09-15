// decl
#ifndef TEMPLATE_CPP_QUADRATIC_MOD
#define TEMPLATE_CPP_QUADRATIC_MOD
#ifndef NO_TEMPLATE_IMPORT
#include <cmath>
using std::min;
using std::pair;
#endif

// impl
#include "lib/math/z.h"
namespace math {

template <const unsigned int Mod>
modular::Z<Mod> quadratic_residue(modular::Z<Mod> n) {
  using Z = modular::Z<Mod>;
  if (n == 0)
    return Z(0);
  if (n.pow((Mod - 1) / 2) != 1)
    return Z(0);
  Z a = 0;
  for (unsigned int i = 0; i < Mod; i++) {
    if ((Z(i) * i - n).pow((Mod - 1) / 2) == Mod - 1) {
      a = i;
      break;
    }
  }

  Z i2 = a * a - n;
  using Complex = pair<Z, Z>;
  auto mul = [&i2](Complex a, Complex b) {
    return Complex{
        a.first * b.first + a.second * b.second * i2,
        a.first * b.second + a.second * b.first};
  };
  Complex ans{1, 0}, bas{a, 1};
  int y = (Mod + 1) / 2;
  while (y) {
    if (y & 1)
      ans = mul(ans, bas);
    bas = mul(bas, bas);
    y >>= 1;
  }

  Z res1 = ans.first, res2 = -res1;
  return min(res1, res2);
}

}  // namespace math

using math::quadratic_residue;
#endif