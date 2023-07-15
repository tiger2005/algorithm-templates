// decl
#ifndef TEMPLATE_CPP_TREE_LCA
#define TEMPLATE_CPP_TREE_LCA 1
#ifndef NO_TEMPLATE_IMPORT
#include <functional>
#include <vector>
using std::function;
using std::vector;
using std::pair;
#endif

// impl
#include "lib/tree/tree.h"
namespace tree {

template <typename T1, const unsigned short OBT>
struct TreeLCA {
  static_assert(OBT & OBTAIN_TREE_DEPTH, "Depth should be provided");
  vector<vector<int>> tableFa;
  vector<int> Lg;
  int n; Tree<T1, OBT> *tr = nullptr;
  TreeLCA() {
    tableFa.clear();
    Lg = vector<int>{0, 0};
    n = 0;
  }
  TreeLCA(Tree<T1, OBT> &t) {
    Lg = vector<int>{0, 0};
    init(t);
  }
  void init(Tree<T1, OBT> &t) {
    tableFa.clear();
    n = t.n + 1; tr = &t;
    if ((int) Lg.size() <= n)
      for (int i = (int) Lg.size(); i <= n; i++)
        Lg.push_back(Lg[i >> 1] + 1);
    int l = Lg[n];
    tableFa.push_back(t.fa);
    for (int i = 1; i <= l; i++) {
      tableFa.push_back(vector<int>(n));
      for (int j = 0; j < n; j++)
        tableFa[i][j] = tableFa[i - 1][tableFa[i - 1][j]];
    }
  }
  int query(int l, int r) {
    int dl = tr -> depth(l), dr = tr -> depth(r);
    if (dl > dr) {
      int delta = dl - dr;
      for (int i = Lg[delta]; i >= 0; i --)
        if (delta & (1 << i))
          l = tableFa[i][l];
      dl = dr;
    }
    else if (dr > dl) {
      int delta = dr - dl;
      for (int i = Lg[delta]; i >= 0; i --)
        if (delta & (1 << i))
          r = tableFa[i][r];
    }
    if (l == r)
      return l;
    for (int i = Lg[dl]; i >= 0; i --) {
      int tmpl = tableFa[i][l], tmpr = tableFa[i][r];
      if (tmpl != tmpr)
        l = tmpl, r = tmpr;
    }
    if (tableFa[0][l] != tableFa[0][r])
      return -1;
    return tableFa[0][l];
  }
};
}  // namespace ds
using tree::TreeLCA;
#endif