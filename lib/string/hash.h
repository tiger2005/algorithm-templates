// decl
#ifndef TEMPLATE_CPP_HASH
#define TEMPLATE_CPP_HASH 1
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
#include <cassert>
using std::vector;
using std::pair;
#endif
// impl
#include "lib/math/z.h"

namespace custom_hash {
template <int len = 0, int D = 131, int mod1 = 998244353, int mod2 = 1000000007, int offset = 0>
struct Hasher {
  using Z1 = modular::Z<mod1>;
  using Z2 = modular::Z<mod2>;
  vector<Z1> v1;
  vector<Z2> v2;
  void init(int l) {
    v1.resize(l + 1);
    v2.resize(l + 1);
    v1[0] = 1, v2[0] = 1;
    for (int i = 1; i <= l; i ++)
      v1[i] = v1[i - 1] * D,
      v2[i] = v2[i - 1] * D;
  }
  Hasher() { init(len); }

  template <typename T>
  pair<int, int> get(const T& s) {
    Z1 x = 0; Z2 y = 0;
    for (auto e: s) {
      x = x * D + (e + offset);
      y = y * D + (e + offset);
    }
    return {(int)x, (int)y};
  }

  struct HashInfo {
    vector<Z1> q1;
    vector<Z2> q2;
    Hasher *parent;
    int __size = 0;

    template <typename T>
    HashInfo(Hasher *parent, const T& v) :parent(parent) {
      __size = v.size();
      assert(__size <= len);
      q1.reserve(__size), q2.reserve(__size);
      Z1 x = 0; Z2 y = 0;
      for (auto e: v) {
        x = x * D + (e + offset);
        y = y * D + (e + offset);
        q1.push_back(x);
        q2.push_back(y);
      }
    }

    int size() {
      return __size;
    }

    pair<int, int> get() {
      return {
        (int)q1.back(), (int)q2.back()
      };
    }

    pair<int, int> get(int l, int r = -1) {
      if (r == -1)
        r = __size - 1;
      if (l == 0)
        return { (int)q1[r], (int)q2[r] };
      return { (int)(q1[r] - q1[l - 1] * (parent -> v1[r - l + 1])), (int)(q2[r] - q2[l - 1] * (parent -> v2[r - l + 1])) };
    }
  };

  template <typename T>
  HashInfo info(const T& v) {
    return HashInfo(this, v);
  }
};

}

using custom_hash::Hasher;

#endif