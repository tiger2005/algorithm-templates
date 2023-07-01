// decl
#ifndef TEMPLATE_CPP_QUADRATIC_DZ
#define TEMPLATE_CPP_QUADRATIC_DZ 1
#ifndef NO_TEMPLATE_IMPORT
#include <algorithm>
using std::min;
using std::pair;
#endif

// impl
#include "lib/math/dz.h"
namespace math {

dZ quadratic_residue(dZ n) {
  unsigned int Mod = getDynamicMod();
  if (n == 0)
    return dZ(0);
  if (n.pow((Mod - 1) / 2) != 1)
    return dZ(0);
  dZ a = 0;
  for (unsigned int i = 0; i < Mod; i++) {
    if ((dZ(i) * i - n).pow((Mod - 1) / 2) == Mod - 1) {
      a = i;
      break;
    }
  }

  dZ i2 = a * a - n;
  using Complex = pair<dZ, dZ>;
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

  dZ res1 = ans.first, res2 = -res1;
  return min(res1, res2);
}

}  // namespace math

using math::quadratic_residue;
#endif