// decl
#ifndef TEMPLATE_CPP_LOG2
#define TEMPLATE_CPP_LOG2 1
#ifndef NO_TEMPLATE_IMPORT
#include <cstdlib>

#endif

// impl
// https://en.wikipedia.org/wiki/De_Bruijn_sequence
namespace math {
unsigned int get_log(unsigned long long n) {  // assume n is power of 2
  static constexpr unsigned long long deBruijn = 0x022fdd63cc95386d;
  static constexpr unsigned int convert[64] = {
      0, 1, 2, 53, 3, 7, 54, 27, 4, 38, 41, 8, 34, 55, 48, 28, 62, 5, 39, 46, 44, 42,
      22, 9, 24, 35, 59, 56, 49, 18, 29, 11, 63, 52, 6, 26, 37, 40, 33, 47, 61, 45, 43, 21,
      23, 58, 17, 10, 51, 25, 36, 32, 60, 20, 57, 16, 50, 31, 19, 15, 30, 14, 13, 12};
  return convert[n * deBruijn >> 58];
}
inline constexpr int get_len(int n) { return --n, n |= n >> 1, n |= n >> 2, n |= n >> 4, n |= n >> 8, n |= n >> 16, ++n; }
}  // namespace math
using math::get_log;
using math::get_len;
#endif