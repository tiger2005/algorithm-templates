// decl
#ifndef TEMPLATE_CPP_DIJKSTRA
#define TEMPLATE_CPP_DIJKSTRA 1
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
#include <functional>
#include <set>
using std::set;
using std::function;
using std::vector;
using std::pair;
#endif
// impl
#include "lib/graph/graph.h"
namespace graph {
template <typename T1, typename T = int>
vector<T> dijkstra(GraphLength<T1> &g, vector<int> s, T maxv = std::numeric_limits<T>::max()
  , function<T(const T&, const T1&)> acceletare = [](const T& a, const T1& b) -> T {
    return a + b;
  }) {
  vector<T> res(g.n + 1, maxv);
  vector<bool> vis(g.n + 1);
  set<pair<T, int>> que;
  for (auto p: s) {
    res[p] = T();
    que.insert({res[p], p});
  }
  while (!que.empty()) {
    auto ptr = que.begin();
    T len = ptr -> first;
    int pos = ptr -> second;
    que.erase(ptr);
    if (vis[pos])
      continue;
    vis[pos] = true;
    for (auto ele: g[pos]) {
      int v = ele.first; T1 w = ele.second;
      T nlen = acceletare(len, w);
      if (nlen < res[v]) {
        res[v] = nlen;
        que.insert({res[v], v});
      }
    }
  }
  return res;
}
// single point alias
template <typename T1, typename T = int>
vector<T> dijkstra(GraphLength<T1> &g, int s, T maxv = std::numeric_limits<T>::max()
  , function<T(const T&, const T1&)> acceletare = [](const T& a, const T1& b) {
    return a + b;
  }) {
  return dijkstra(g, vector<int>{s}, maxv, acceletare);
}
}
using graph::dijkstra;
#endif