// decl
#ifndef TEMPLATE_CPP_ST
#define TEMPLATE_CPP_ST 1
#ifndef NO_TEMPLATE_IMPORT
#include <cmath>
#include <functional>
#include <vector>
using std::vector;
#endif
// impl
namespace ds {
template <typename Info, typename Tag>
struct LazySegTree {
  std::vector<Info> tr;
  std::vector<Tag> tg;
  int n;
  LazySegTree(): n(0) {}
  LazySegTree(int n, Info v = Info()) {
    init(n, v);
  }
  template <typename T>
  LazySegTree(vector<T> vec) {
    init(vec);
  }
  void init(int n, Info v = Info()) {
    init(vector<Info>(n, v));
  }
  template <typename T>
  void init(vector<T> vec) {
    n = vec.size();
    tr.assign(4 << std::__lg(n), Info());
    tg.assign(4 << std::__lg(n), Tag());
    auto build = [&] (auto self, int x, int l, int r) -> void {
      if (r == l + 1) {
        tr[x] = vec[l];
        return;
      }
      int m = (l + r) >> 1;
      self(self, x << 1, l, m);
      self(self, x << 1 | 1, m, r);
      tr[x] = tr[x << 1] + tr[x << 1 | 1];
    };
    build(build, 1, 0, n);
  }
  void _apply(int x, Tag v) {
    tr[x].apply(v);
    tg[x] = tg[x] + v;
  }
  void pushdown(int x) {
    _apply(x << 1, tg[x]);
    _apply(x << 1 | 1, tg[x]);
    tg[x] = Tag();
  }
  void modify(int x, int l, int r, int k, Info v) {
    if (r == l + 1) {
      tr[x] = v;
      return;
    }
    int m = (l + r) >> 1;
    pushdown(x);
    if (k < m)
      modify(x << 1, l, m, k, v);
    else
      modify(x << 1 | 1, m + 1, r, k, v);
    tr[x] = tr[x << 1] + tr[x << 1 | 1];
  }
  void modify(int k, Info v) {
    modify(1, 0, n, k, v);
  }
  void apply(int x, int l, int r, int ll, int rr, Tag v) {
    if (ll <= l && r <= rr) {
      _apply(x, v);
      return;
    }
    int m = (l + r) >> 1;
    pushdown(x);
    if (ll < m)
      apply(x << 1, l, m, ll, rr, v);
    if (rr > m)
      apply(x << 1 | 1, m, r, ll, rr, v);
    tr[x] = tr[x << 1] + tr[x << 1 | 1];
  }
  void apply(int l, int r, Tag v) {
    apply(1, 0, n, l, r + 1, v);
  }
  Info query(int x, int l, int r, int ll, int rr) {
    if (ll <= l && r <= rr)
      return tr[x];
    Info res = Info();
    int m = (l + r) >> 1;
    pushdown(x);
    if (ll < m)
      res = res + query(x << 1, l, m, ll, rr);
    if (rr > m)
      res = res + query(x << 1 | 1, m, r, ll, rr);
    return res;
  }
  Info query(int l, int r) {
    // [l, r] => [l, r + 1)
    return query(1, 0, n, l, r + 1);
  }
  template <typename F>
  int first(int x, int l, int r, int ll, int rr, F &&pred) {
    if (rr <= l || r <= ll)
      return -1;
    if (ll <= l && r <= rr && !F(tr[x]))
      return -1;
    if (r == l + 1)
      return l;
    int m = (l + r) >> 1;
    pushdown(x);
    int res = first(x << 1, l, m, ll, rr, pred);
    if (res == -1)
      res = first(x << 1 | 1, m, r, ll, rr, pred);
    return res;
  }
  template <typename F>
  int first(int l, int r, F &&pred) {
    return first(1, 0, n, l, r + 1, pred);
  }
  template <typename F>
  int last(int x, int l, int r, int ll, int rr, F &&pred) {
    if (rr <= l || r <= ll)
      return -1;
    if (ll <= l && r <= rr && !F(tr[x]))
      return -1;
    if (r == l + 1)
      return l;
    int m = (l + r) >> 1;
    pushdown(x);
    int res = last(x << 1 | 1, m, r, ll, rr, pred);
    if (res == -1)
      res = last(x << 1, l, m, ll, rr, pred);
    return res;
  }
  template <typename F>
  int last(int l, int r, F &&pred) {
    return last(1, 0, n, l, r + 1, pred);
  }
};
}  // namespace ds
using ds::LazySegTree;
#endif