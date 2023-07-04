// decl
#ifndef TEMPLATE_CPP_BINOM
#define TEMPLATE_CPP_BINOM 1
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
using std::vector;
#endif
// impl
#include "lib/math/z.h"
namespace math {
template <typename T = Z>
struct Factors {
  int n;
  vector<T> facts;
  Factors(int x = 0) {
    n = 0;
    facts.assign(1, 1);
    assign(x);
  }
  void assign(int x) {
    while (n < x) {
      ++n;
      facts.push_back(facts.back() * n);
    }
  }
  T operator[](int x) {
    return facts[x];
  }
};
template <const unsigned int Mod = Md>
struct Inv {
  using Z = modular::Z<Mod>;
  vector<Z> invs;
  int n = 0;
  Inv(int x = 1) {
    n = 1;
    invs.assign(2, 1);
    assign(x);
  }
  void assign(int x) {
    while (n < x) {
      ++n;
      invs.push_back(Z(invs[Mod % n]) * (Mod - Mod / n));
    }
  }
  Z operator[](int x) {
    return invs[x];
  }
};
template <const unsigned int Mod = Md>
struct Inv_Factors {
  using Z = modular::Z<Mod>;
  Inv<Mod> invs;
  vector<Z> invfs;
  int n = 0;
  Inv_Factors(int x = 1) {
    n = 1;
    invs.assign(1);
    invfs.assign(2, 1);
    assign(x);
  }
  void assign(int x) {
    invs.assign(x);
    while (n < x) {
      ++n;
      invfs.push_back(invs[n] * invfs.back());
    }
  }
  Z operator[](int x) {
    return invfs[x];
  }
};
template <const unsigned int Mod = Md>
struct Binom_facts {
  using Z = modular::Z<Mod>;
  Factors<Z> f;
  Inv_Factors<Mod> invf;
  int n = 0;
  Binom_facts(int x = 1) {
    n = 1;
    assign(x);
  }
  void assign(int x) {
    f.assign(x);
    invf.assign(x);
  }
  Z get(int x, int y) {
    if (x < y || y < 0)
      return 0;
    assign(x);
    return f[x] * invf[y] * invf[x - y];
  }
};
template <typename T = Z>
struct Binom_bf {
  int n = 0;
  Binom_bf() {
    n = 1;
  }
  T get(int x, int y) {
    if (x < y || y < 0)
      return T();
    T ret(1);
    for (int i = x; i >= (x - y + 1); i--)
      ret = ret * i / (x - i + 1);
    return ret;
  }
};
}  // namespace math
using math::Factors;
using math::Inv;
using math::Inv_Factors;
using math::Binom_facts;
using math::Binom_bf;
#endif