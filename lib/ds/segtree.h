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
template <typename Info>
struct SegTree {
  std::vector<Info> tr;
  int n;
  SegTree(): n(0) {}
  SegTree(int n, Info v = Info()) {
    init(n, v);
  }
  template <typename T>
  SegTree(vector<T> vec) {
    init(vec);
  }
  void init(int n, Info v = Info()) {
    init(vector<Info>(n, v));
  }
  template <typename T>
  void init(vector<T> vec) {
    n = vec.size();
    tr.assign(4 << std::__lg(n), Info());
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
  void modify(int x, int l, int r, int k, Info v) {
    if (r == l + 1) {
      tr[x] = v;
      return;
    }
    int m = (l + r) >> 1;
    if (k < m)
      modify(x << 1, l, m, k, v);
    else
      modify(x << 1 | 1, m + 1, r, k, v);
    tr[x] = tr[x << 1] + tr[x << 1 | 1];
  }
  void modify(int k, Info v) {
    modify(1, 0, n, k, v);
  }
  Info query(int x, int l, int r, int ll, int rr) {
    if (ll <= l && r <= rr)
      return tr[x];
    Info res = Info();
    int m = (l + r) >> 1;
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
using ds::SegTree;
#endif