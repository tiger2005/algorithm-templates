// decl
#ifndef TEMPLATE_CPP_MONTGOMERT
#define TEMPLATE_CPP_MONTGOMERT 1
#ifndef NO_TEMPLATE_IMPORT
#endif
// impl
namespace math {
using i32 = int;
using u32 = unsigned int;
using u64 = unsigned long long;
static constexpr u32 mont_get_r(u32 mod) {
  u32 two = 2, iv = mod * (two - mod * mod);
  iv *= (two - mod * iv);
  iv *= (two - mod * iv);
  return iv * (mod * iv - two);
}
static constexpr u32 mont_reduce(u64 x, u32 r, u32 mod) {
  return (x + u64(u32(x) * r) * mod) >> 32;
}
static constexpr u32 mont_norm(u32 x, u32 mod) {
  return x - (mod & -((mod - 1 - x) >> 31));
}
}  // namespace math
using math::mont_get_r;
using math::mont_norm;
using math::mont_reduce;
#endif