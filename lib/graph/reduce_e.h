// decl
#ifndef TEMPLATE_CPP_REDUCE_E
#define TEMPLATE_CPP_REDUCE_E 1
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
using std::vector;
using std::swap;
#endif
// impl
#include "lib/graph/graph.h"
namespace graph {
Graph reduce_e(Graph &g, vector<pair<int, int>> exc) {
  vector<int> lookup(g.n + 1);
  vector<vector<int>> e(g.n + 1);
  for (auto [x, y]: exc) {
    if (!g.dirt && x > y) swap(x, y);
    e[x].push_back(y);
  }
  Graph res(g.n, 0, g.dirt);
  for (int i = 1; i <= g.n; i ++) {
    for (int v: e[i])
      lookup[v] = i;
    for (auto v: g[i])
      if ((g.dirt || v >= i) && (lookup[v] ^ i))
        res.add(i, v);
  }
  return res;
}
template <typename T>
GraphLength<T> reduce_e(GraphLength<T> &g, vector<pair<int, int>> exc) {
  vector<int> lookup(g.n + 1);
  vector<vector<int>> e(g.n + 1);
  for (auto [x, y]: exc) {
    if (!g.dirt && x > y) swap(x, y);
    e[x].push_back(y);
  }
  GraphLength<T> res(g.n, 0, g.dirt);
  for (int i = 1; i <= g.n; i ++) {
    for (int v: e[i])
      lookup[v] = i;
    for (auto [v, w]: g[i])
      if ((g.dirt || v >= i) && (lookup[v] ^ i))
        res.add(i, v, w);
  }
  return res;
}
}
using graph::reduce_e;
#endif