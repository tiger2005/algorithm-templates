// decl
#ifndef TEMPLATE_CPP_FWT
#define TEMPLATE_CPP_FWT 1
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
#include <functional>
#include <tuple>
using std::tuple;
using std::function;
using std::vector;
#endif
// impl
namespace math {
namespace fwt_core {
template <typename T, bool forward>
inline void FWT_AND_FUNC (T& a, T& b) {
  return (forward ? (a += b) : (a -= b)), void();
};
template <typename T, bool forward>
inline void FWT_OR_FUNC (T& a, T& b) {
  return (forward ? (b += a) : (b -= a)), void();
};
template <typename T, bool forward>
inline void FWT_XOR_FUNC (T& a, T& b) {
  tie(a, b) = tuple{a + b, a - b};
  if (!forward)
    a /= 2, b /= 2;
  return;
};
template <typename T, bool forward>
inline void FWT_NXOR_FUNC (T& a, T& b) {
  tie(a, b) = tuple{a - b, a + b};
  if (!forward)
    a /= 2, b /= 2;
  return;
};
#define FWT_AND 0
#define FWT_OR 1
#define FWT_XOR 2
#define FWT_NXOR 3
}
template <bool forward, unsigned int type, typename T>
void fwt_step(vector<T> &v) {
  int n = v.size();
  for (int l = 1; l < n; l <<= 1)
    for (int j = 0, b = l << 1; j < n; j += b)
      for (int k = j; k < j + l; k ++) {
        if constexpr (type == FWT_AND)
          fwt_core::FWT_AND_FUNC<T, forward>(v[k], v[k + l]);
        else if constexpr (type == FWT_OR)
          fwt_core::FWT_OR_FUNC<T, forward>(v[k], v[k + l]);
        else if constexpr (type == FWT_XOR)
          fwt_core::FWT_XOR_FUNC<T, forward>(v[k], v[k + l]);
        else if constexpr (type == FWT_NXOR)
          fwt_core::FWT_NXOR_FUNC<T, forward>(v[k], v[k + l]);
      }
}
template <unsigned int type, typename T>
vector<T> fwt(vector<T> a, vector<T> b) {
  if (a.size() != b.size())
    throw "FWT: length error";
  int n = a.size();
  vector<T> res(n);
  fwt_step<true, type, T>(a);
  fwt_step<true, type, T>(b);
  for (int i = 0; i < n; i ++)
    res[i] = a[i] * b[i];
  return fwt_step<false, type, T>(res), res;
}
}
using math::fwt;
using math::fwt_step;
using namespace math::fwt_core;
#endif