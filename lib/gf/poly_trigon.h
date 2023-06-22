// decl
#ifndef TEMPLATE_CPP_POLY_TRIGON
#define TEMPLATE_CPP_POLY_TRIGON 1
#ifndef NO_TEMPLATE_IMPORT

#endif

// impl
#include "lib/gf/poly.h"
namespace gf {

template <const int Mod, const int G>
Poly<Mod, G> fsin(const Poly<Mod, G> &f) {
  using Ele = modular::Z<Mod>;
  Ele i = Z(G).pow((Mod - 1) / 4);
  Poly<Mod, G> res = ((f * i).exp() - (f * (-i)).exp()) / (2 * i);
  res.resize(f.size());
  return res;
}

template <const int Mod, const int G>
Poly<Mod, G> fcos(const Poly<Mod, G> &f) {
  using Ele = modular::Z<Mod>;
  Ele i = Z(G).pow((Mod - 1) / 4);
  Poly<Mod, G> res = ((f * i).exp() + (f * (-i)).exp()) / 2;
  res.resize(f.size());
  return res;
}

template <const int Mod, const int G>
Poly<Mod, G> ftan(const Poly<Mod, G> &f) {
  Poly<Mod, G> res = fsin(f) / fcos(f);
  res.resize(f.size());
  return res;
}

template <const int Mod, const int G>
Poly<Mod, G> fasin(const Poly<Mod, G> &f) {
  Poly<Mod, G> p = f.copy();
  p *= p; p.resize(f.size());
  p.nega(); ++ p[0];
  Poly<Mod, G> res = f.derivative() / p.sqrt();
  res.resize(p.size() - 1);
  return res.integral();
}

template <const int Mod, const int G>
Poly<Mod, G> facos(const Poly<Mod, G> &f) {
  return -fasin(f);
}

template <const int Mod, const int G>
Poly<Mod, G> fatan(const Poly<Mod, G> &f) {
  Poly<Mod, G> p = f.copy();
  p *= p; p.resize(f.size());
  ++ p[0];
  Poly<Mod, G> res = f.derivative() / p;
  res.resize(p.size() - 1);
  return res.integral();
}

}  // namespace gf

using gf::fcos;
using gf::fsin;
using gf::ftan;
using gf::fasin;
using gf::facos;
using gf::fatan;
#endif
