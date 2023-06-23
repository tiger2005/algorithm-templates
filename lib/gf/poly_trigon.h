// decl
#ifndef TEMPLATE_CPP_POLY_TRIGON
#define TEMPLATE_CPP_POLY_TRIGON 1
#ifndef NO_TEMPLATE_IMPORT
#endif
// impl
#include "lib/gf/poly.h"
namespace gf {
template <const unsigned int Mod>
Poly<Mod> fsin(const Poly<Mod> &f) {
  using Ele = modular::Z<Mod>;
  Ele i = Ele(f.G).pow((Mod - 1) / 4);
  Poly<Mod> res = ((f * i).exp() - (f * (-i)).exp()) / (2 * i);
  res.resize(f.size());
  return res;
}
template <const unsigned int Mod>
Poly<Mod> fcos(const Poly<Mod> &f) {
  using Ele = modular::Z<Mod>;
  Ele i = Ele(f.G).pow((Mod - 1) / 4);
  Poly<Mod> res = ((f * i).exp() + (f * (-i)).exp()) / Ele(2);
  res.resize(f.size());
  return res;
}
template <const unsigned int Mod>
Poly<Mod> ftan(const Poly<Mod> &f) {
  Poly<Mod> res = fsin(f) / fcos(f);
  res.resize(f.size());
  return res;
}
template <const unsigned int Mod>
Poly<Mod> fasin(const Poly<Mod> &f) {
  Poly<Mod> p = f;
  p *= p;
  p.resize(f.size());
  p.nega();
  ++p[0];
  Poly<Mod> res = f.derivative() / p.sqrt();
  res.resize(p.size() - 1);
  return res.integral();
}
template <const unsigned int Mod>
Poly<Mod> facos(const Poly<Mod> &f) {
  return -fasin(f);
}
template <const unsigned int Mod>
Poly<Mod> fatan(const Poly<Mod> &f) {
  Poly<Mod> p = f;
  p *= p;
  p.resize(f.size());
  ++p[0];
  Poly<Mod> res = f.derivative() / p;
  res.resize(p.size() - 1);
  return res.integral();
}
}  // namespace gf
using gf::facos;
using gf::fasin;
using gf::fatan;
using gf::fcos;
using gf::fsin;
using gf::ftan;
#endif