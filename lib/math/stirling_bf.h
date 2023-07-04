// decl
#ifndef TEMPLATE_CPP_STIRLING_BF
#define TEMPLATE_CPP_STIRLING_BF 1
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
using std::vector;
#endif
// impl
#include "lib/math/z.h"
#include "lib/math/binom.h"
namespace math {
template <typename T = long long>
struct stirling1_bf {
  vector<vector<T>> v;
  int n;
  stirling1_bf(int x = 0) {
    v.clear(); n = 0;
    v.push_back(vector<T>{1}); assign(x);
  }
  void assign(int x) {
    while (n < x) {
      ++ n;
      vector<T> tmp(n + 1, T());
      vector<T> &las = v.back();
      tmp[n] = las[n - 1];
      for (int i = 1; i < n; i ++)
        tmp[i] = las[i - 1] + las[i] * (n - 1);
      v.push_back(tmp);
    }
  }
  T get(int x, int y) {
    if (x < y || y < 0)
      return T();
    assign(x);
    return v[x][y];
  }
};
template <typename T = long long>
struct stirling2_bf {
  vector<vector<T>> v;
  int n;
  stirling2_bf(int x = 0) {
    v.clear(); n = 0;
    v.push_back(vector<T>{1}); assign(x);
  }
  void assign(int x) {
    while (n < x) {
      ++ n;
      vector<T> tmp(n + 1, T());
      vector<T> &las = v.back();
      tmp[n] = las[n - 1];
      for (int i = 1; i < n; i ++)
        tmp[i] = las[i - 1] + las[i] * i;
      v.push_back(tmp);
    }
  }
  T get(int x, int y) {
    if (x < y || y < 0)
      return T();
    assign(x);
    return v[x][y];
  }
};
}
using math::stirling1_bf;
using math::stirling2_bf;
#endif