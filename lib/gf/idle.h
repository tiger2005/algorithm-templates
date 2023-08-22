// decl
#ifndef TEMPLATE_CPP_POLY_OP
#define TEMPLATE_CPP_POLY_OP 1
#ifndef NO_TEMPLATE_IMPORT
#include <functional>
#include <string>
using std::function;
using std::to_string;
#endif
// impl

#include "lib/gf/online_conv.h"
#include "lib/gf/poly.h"
#include "lib/math/binom.h"
namespace gf {

namespace idle_core {

template <const unsigned int Mod>
struct PolyCache {
  using Ele = modular::Z<Mod>;
  function<Ele(int)> handle;
  vector<Ele> vals;
  vector<bool> used, request;
  int n;
  bool store;
  PolyCache(int n = 0) : n(n) {
    if (n == -1)
      store = false;
    else {
      store = true;
      vals.clear();
      vals.assign(n, 0);
      used.assign(n, 0);
      request.assign(n, 0);
    }
  }
  PolyCache(vector<Ele> v) {
    store = false;
    handle = [=](int x) {
      return (x < (int) v.size()) ? v[x] : Ele(0);
    };
  }
  Ele get(int x) {
    fflush(stdout);
    if (x < 0)
      throw "PolyCache::get() out of bound";
    if (!store)
      return handle(x);
    if (x >= n)
      vals.resize(x + 1), used.resize(x + 1), request.resize(x + 1), n = x + 1;
    if (!used[x]) {
      if (request[x])
        throw "PolyCache::get() invalid request";
      request[x] = true;
      vals[x] = handle(x);
      request[x] = false;
      used[x] = true;
    }
    return vals[x];
  }
  void bind(PolyCache *pc) {
    handle = [=](int x) { return pc->get(x); };
  }
  int size() const { return n; }
  PolyCache(PolyCache const &) = delete;
  PolyCache &operator=(PolyCache const &) = delete;
};

template <const unsigned int Mod>
struct Corner : PolyCache<Mod> {
  using typename PolyCache<Mod>::Ele;
  Corner(PolyCache<Mod> *l, vector<Ele> v) :PolyCache<Mod>(-1) {
    this->handle = [=](int x) {
      return x < (int) v.size() ? v[x] : l->get(x);
    };
  }
};

template <const unsigned int Mod>
struct Add : PolyCache<Mod> {
  Add(PolyCache<Mod> *l, PolyCache<Mod> *r) :PolyCache<Mod>(-1) {
    this->handle = [=](int x) {
      return l->get(x) + r->get(x);
    };
  }
};

template <const unsigned int Mod>
struct Del : PolyCache<Mod> {
  Del(PolyCache<Mod> *l, PolyCache<Mod> *r) :PolyCache<Mod>(-1) {
    this->handle = [=](int x) {
      return l->get(x) - r->get(x);
    };
  }
};

template <const unsigned int Mod>
struct Mul : PolyCache<Mod> {
  bool lblk, rblk;
  int lzero, rzero;
  OnlineConv<Mod> oc;
  Mul(PolyCache<Mod> *l, PolyCache<Mod> *r) {
    lblk = rblk = false;
    lzero = rzero = 0;
    oc = OnlineConv<Mod>();
    this->handle = [l, r, this](int x) -> typename PolyCache<Mod>::Ele {
      if (x)
        this->get(x - 1);
      // find 0 perfex in some order
      // this part is dangerous
      while (x >= lzero + rzero && !(lblk && rblk)) {
        if ((lzero <= rzero && !lblk) || rblk)
          l->get(lzero) == 0 ? lzero++ : lblk = true;
        else
          r->get(rzero) == 0 ? rzero++ : rblk = true;
      }
      if (x < lzero + rzero)
        return 0;
      oc.set(l->get(x - rzero), r->get(x - lzero));
      return oc.get(x - lzero - rzero);
    };
  }
};

template <const unsigned int Mod>
struct Trans : PolyCache<Mod> {
  using typename PolyCache<Mod>::Ele;
  Trans(PolyCache<Mod> *l, function<Ele(Ele, int, function<Ele(int)>)> f) {
    function<Ele(int)> getter = [this] (int x) { return this -> get(x); };
    this->handle = [=](int x) {
      return f(l->get(x), x, getter);
    };
  }
};

template <const unsigned int Mod>
struct Build : PolyCache<Mod> {
  using typename PolyCache<Mod>::Ele;
  Build(function<Ele(int, function<Ele(int)>)> f) {
    function<Ele(int)> getter = [this] (int x) { return this -> get(x); };
    this->handle = [=](int x) {
      return f(x, getter);
    };
  }
};

template <const unsigned int Mod>
struct PreSum : PolyCache<Mod> {
  PreSum(PolyCache<Mod> *l) {
    this->handle = [=, this](int x) {
      if (x == 0)
        return l -> get(0);
      return this -> get(x - 1) + l -> get(x);
    };
  }
};

template <const unsigned int Mod>
struct MulInt : PolyCache<Mod> {
  MulInt(PolyCache<Mod> *l, int v) :PolyCache<Mod>(-1) {
    this->handle = [=](int x) {
      return l->get(x) * v;
    };
  }
};

template <const unsigned int Mod>
struct Shift : PolyCache<Mod> {
  Shift(PolyCache<Mod> *l, int delta) :PolyCache<Mod>(-1) {
    this->handle = [=](int x) -> typename PolyCache<Mod>::Ele {
      x -= delta;
      return x < 0 ? 0 : l->get(x);
    };
  }
};

template <const unsigned int Mod>
struct Dot : PolyCache<Mod> {
  Dot(PolyCache<Mod> *l, PolyCache<Mod> *r) :PolyCache<Mod>(-1) {
    this->handle = [=](int x) {
      return l->get(x) * r->get(x);
    };
  }
};

template <const unsigned int Mod>
struct Amp : PolyCache<Mod> {
  Amp(PolyCache<Mod> *l, int num) :PolyCache<Mod>(-1) {
    this->handle = [=](int x) -> typename PolyCache<Mod>::Ele {
      return x % num == 0 ? l->get(x / num) : 0;
    };
  }
};

template <const unsigned int Mod>
struct Deri : PolyCache<Mod> {
  Deri(PolyCache<Mod> *l) :PolyCache<Mod>(-1) {
    this->handle = [=](int x) {
      return l->get(x + 1) * (x + 1);
    };
  }
};

template <const unsigned int Mod>
struct Inte : PolyCache<Mod> {
  using typename PolyCache<Mod>::Ele;
  Inv<Mod> iv;
  Inte(PolyCache<Mod> *l, Ele c = 0) :PolyCache<Mod>(-1) {
    this->handle = [=, this](int x) {
      if (x == 0)
        return Ele(c);
      iv.assign(x);
      return l->get(x - 1) * iv[x];
    };
  }
};

template <const unsigned int Mod>
struct Exp : PolyCache<Mod> {
  Exp(PolyCache<Mod> *l) {
    this->bind(new Inte(new Mul(new Deri(l), this), 1));
  }
};

template <const unsigned int Mod>
struct Inv : PolyCache<Mod> {
  using typename PolyCache<Mod>::Ele;
  Mul<Mod> *mul;
  Ele inv0;
  Inv(PolyCache<Mod> *l) {
    inv0 = Ele();
    // prevent dependency loop
    mul = new Mul(new Corner(l, {0}), this);
    this->handle = [l, this](int x) {
      if (!x)
        return inv0 = l->get(0).inv();
      this->get(0);
      return -mul->get(x) * inv0;
    };
  }
};

template <const unsigned int Mod>
struct Div : PolyCache<Mod> {
  using typename PolyCache<Mod>::Ele;
  Ele inv0;
  Del<Mod> *d;
  Div(PolyCache<Mod> *l, PolyCache<Mod> *r) {
    d = new Del(
        l,
        new Mul(new Corner(r, {0}), this));
    inv0 = Ele();
    this->handle = [this, l, r](int x) {
      if (!x)
        return d->get(0) * (inv0 = r->get(0).inv());
      this->get(0);
      return d->get(x) * inv0;
    };
  }
};

template <const unsigned int Mod>
struct Ln : PolyCache<Mod> {
  Ln(PolyCache<Mod> *l) {
    this->bind(new Inte(new Div(new Deri(l), l)));
  }
};

template <const unsigned int Mod>
struct Sqrt : PolyCache<Mod> {
  using typename PolyCache<Mod>::Ele;
  Corner<Mod> *cor;
  Del<Mod> *m;
  Sqrt(PolyCache<Mod> *l, Ele v) {
    cor = new Corner(this, {0});
    Ele coef = (v * 2).inv();
    m = new Del(l, new Mul(cor, cor));
    this->handle = [this, v, coef](int x) {
      return !x ? v : m->get(x) * coef;
    };
  }
  Sqrt(PolyCache<Mod> *l) : Sqrt(l, quadratic_residue(l->get(0))){};
};

template <const unsigned int Mod>
struct Pow : PolyCache<Mod> {
  using typename PolyCache<Mod>::Ele;
  PolyCache<Mod> *s;
  Shift<Mod> *r;
  int pad;
  Pow(PolyCache<Mod> *l, string str) {
    Ele r1;
    unsigned long long r2 = 0, r3 = 0;
    for (auto ch : str) {
      r1 = r1 * 10 + (ch - '0');
      r2 = (r2 * 10 + (ch - '0')) % (Mod - 1);
      r3 = min(r3 * 10 + (ch - '0'), (unsigned long long) 2e9);
    }
    unsigned int k = r1.get();
    pad = 0;
    this->handle = [=, this](int x) {
      if (r3 == 0)
        return Ele(x == 0);
      if (x)
        this->get(x - 1);
      if (pad == x && l->get(x) == 0)
        ++pad;
      if ((x -= pad * r3) < 0)
        return Ele(0);
      if (!x) {
        // g' = k (g / f) f'
        r = new Shift(l, -pad);
        s = new Inte(
            new Div(
                new MulInt(
                    new Mul(new Deri(r), this), k),
                r));
        return r->get(0).pow(r2);
      }
      return s->get(x);
    };
  }
  Pow(PolyCache<Mod> *l, unsigned long long num) : Pow(l, to_string(num)) {}
};

template <const unsigned int Mod>
struct Idle {
  PolyCache<Mod> *pc;
  using Ele = modular::Z<Mod>;
  Idle() { pc = new PolyCache<Mod>(); }
  Idle(PolyCache<Mod> *pc) : pc(pc) {
    if (!pc) throw "Pointer is empty";
  }
  Ele get(int x) { return pc->get(x); }
  // do not store here
  Idle(vector<Ele> p) { pc = new PolyCache<Mod>(p); }
  vector<Ele> await(int w) {
    vector<Ele> res;
    for (int i = 0; i < w; i++)
      res.push_back(pc->get(i));
    return res;
  }
  Idle(function<Ele(int, function<Ele(int)>)> f) { pc = new Build(f); }
  Idle operator+(const Idle &i) const { return Idle(new Add(pc, i.pc)); }
  Idle operator-(const Idle &i) const { return Idle(new Del(pc, i.pc)); }
  Idle operator*(const Idle &i) const { return Idle(new Mul(pc, i.pc)); }
  Idle operator*(const int x) const { return Idle(new MulInt(pc, x)); }
  Idle operator/(const Idle &i) const { return Idle(new Div(pc, i.pc)); }
  Idle operator/(const int x) const { return Idle(new MulInt(pc, Ele(x).invVal())); }
  Idle dot(const Idle &i) const { return Idle(new Dot(pc, i.pc)); }
  Idle corner(vector<Ele> v) { return Idle(new Corner(pc, v)); }
  Idle trans(function<Ele(Ele, int, function<Ele(int)>)> f) { return Idle(new Trans(pc, f)); }
  Idle operator<<(const int x) { return Idle(new Shift(pc, x)); }
  Idle operator>>(const int x) { return Idle(new Shift(pc, -x)); }
  Idle amp(const int x) { return Idle(new Amp(pc, x)); }
  Idle deri() { return Idle(new Deri(pc)); }
  Idle inte(int c = 0) { return Idle(new Inte(pc, c)); }
  Idle inv() { return Idle(new Inv(pc)); }
  Idle exp() { return Idle(new Exp(pc)); }
  Idle ln() { return Idle(new Ln(pc)); }
  Idle pow(unsigned long long k) { return Idle(new Pow(pc, k)); }
  Idle pow(string k) { return Idle(new Pow(pc, k)); }
  Idle sqrt(Ele v) { return Idle(new Sqrt(pc, v)); }
  Idle sqrt() { return Idle(new Sqrt(pc)); }
  Idle pre() { return Idle(new PreSum(pc)); }
  void equal(const Idle &i) { if (pc && pc != i.pc) pc->bind(i.pc); }
};

}  // namespace idle_core

};  // namespace gf
using Idle = gf::idle_core::Idle<Md>;
#endif