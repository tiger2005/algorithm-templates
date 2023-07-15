// decl
#ifndef TEMPLATE_CPP_TREE_ST
#define TEMPLATE_CPP_TREE_ST 1
#ifndef NO_TEMPLATE_IMPORT
#include <functional>
#include <vector>
using std::function;
using std::vector;
using std::pair;
#endif
// impl
#include "lib/tree/tree.h"
namespace ds {
template <typename T, typename T1, const unsigned short OBT>
struct TreeST {
  static_assert(OBT & OBTAIN_TREE_DEPTH, "Depth should be provided");
  vector<vector<T>> table;
  vector<vector<int>> tableFa;
  vector<int> Lg;
  int n; Tree<T1, OBT> *tr = nullptr;
  T E;
  static T default_func(const T &t1, const T &t2) { return max(t1, t2); }
  function<T(const T &, const T &)> op = default_func;
  TreeST() {
    table.clear(); tableFa.clear();
    Lg = vector<int>{0, 0};
    n = 0;
  }
  TreeST(Tree<T1, OBT> &t, vector<T> v, function<T(const T &, const T &)> func = default_func, T e = T()) {
    Lg = vector<int>{0, 0};
    init(t, v, func, e);
  }
  void init(Tree<T1, OBT> &t, vector<T> v, function<T(const T &, const T &)> func = default_func, T e = T()) {
    op = func; table.clear(); tableFa.clear();
    n = v.size(); tr = &t; E = e;
    if ((int) Lg.size() <= n)
      for (int i = (int) Lg.size(); i <= n; i++)
        Lg.push_back(Lg[i >> 1] + 1);
    int l = Lg[n];
    table.push_back(v);
    tableFa.push_back(t.fa);
    for (int i = 1; i <= l; i++) {
      table.push_back(vector<int>(n, e));
      tableFa.push_back(vector<int>(n));
      for (int j = 0; j < n; j++) {
        int fa = tableFa[i - 1][j];
        tableFa[i][j] = tableFa[i - 1][fa];
        table[i][j] = op(table[i - 1][j], table[i - 1][fa]);
      }
    }
  }
  pair<int, pair<T, T>> query(int l, int r) {
    T res1(E), res2(E);
    int dl = tr -> depth(l), dr = tr -> depth(r);
    if (dl > dr) {
      int delta = dl - dr;
      for (int i = Lg[delta]; i >= 0; i --)
        if (delta & (1 << i))
          res1 = op(res1, table[i][l]), l = tableFa[i][l];
      dl = dr;
    }
    else if (dr > dl) {
      int delta = dr - dl;
      for (int i = Lg[delta]; i >= 0; i --)
        if (delta & (1 << i))
          res2 = op(res2, table[i][r]), r = tableFa[i][r];
    }
    if (l == r)
      return {l, {res1, res2}};
    for (int i = Lg[dl]; i >= 0; i --) {
      int tmpl = tableFa[i][l], tmpr = tableFa[i][r];
      if (tmpl != tmpr)
        res1 = op(res1, table[i][l]), l = tmpl,
        res2 = op(res2, table[i][r]), r = tmpr;
    }
    if (tableFa[0][l] != tableFa[0][r])
      return {-1, {res1, res2}};
    return {tableFa[0][l], {op(res1, table[0][l]), op(res2, table[0][r])}};
  }
};
template <typename T, const unsigned short OBT = 0>
TreeST<T, T, OBT> build_tree_st(Tree<T, OBT> &t, function<T(const T &, const T &)> func = TreeST<T, T, OBT>::default_func, T e = T()) {
  TreeST<T, T, OBT> res;
  res.init(t, t.val, func, e);
  return res;
}
}  // namespace ds
using ds::TreeST;
using ds::build_tree_st;
#endif