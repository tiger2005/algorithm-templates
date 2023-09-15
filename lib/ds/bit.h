// decl
#ifndef TEMPLATE_CPP_BIT
#define TEMPLATE_CPP_BIT 1
#ifndef NO_TEMPLATE_IMPORT
#include <functional>
#include <vector>
using std::function;
using std::vector;
#endif
// impl
namespace ds {
template<typename T>
struct BIT {
  int n;
  T e;
  vector<T> vt;
  static T default_func(const T &t1, const T &t2) { return t1 + t2; }
  function<T(const T &, const T &)> op = default_func;
  BIT() {
    n = 0; e = T();
    vt = vector<T>(1, e);
  }
  BIT(int n, T e, function<T(const T &, const T &)> func) {
    init(n, e, func);
  }
  void init(int _n, T _e, function<T(const T &, const T &)> func) {
    n = _n; e = _e; op = func;
    vt.assign(n + 1, e);
  }
  void apply(int a, T x) {
    if (a == 0)
      return;
    while (a <= n) {
      vt[a] = op(vt[a], x);
      a += a & -a;
    }
  }
  T query(int a) {
    T res = e;
    while (a) {
      res = op(res, vt[a]);
      a -= a & -a;
    }
    return res;
  }
};
}
using ds::BIT;
#endif