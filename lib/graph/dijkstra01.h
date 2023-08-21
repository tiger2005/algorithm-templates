// decl
#ifndef TEMPLATE_CPP_DIJKSTRA01
#define TEMPLATE_CPP_DIJKSTRA01 1
#ifndef NO_TEMPLATE_IMPORT
#include <deque>
#include <vector>
#include <functional>
using std::function;
using std::vector;
using std::pair;
using std::deque;
#endif
// impl
#include "lib/graph/graph.h"
namespace graph {
template <typename T1, typename T = int>
vector<T> dijkstra01(GraphLength<T1> &g, vector<int> s, T maxv = std::numeric_limits<T>::max()
  , function<T(const T&, const T1&)> acceletare = [](const T& a, const T1& b) -> T {
    return a + b;
  }) {
  vector<T> res(g.n + 1, maxv);
  vector<bool> vis(g.n + 1);
  deque<int> que;
  for (auto p: s) {
    res[p] = T();
    que.push_back(p);
  }
  while (!que.empty()) {
    int pos = que.front();
    que.pop_front();
    if (vis[pos])
      continue;
    vis[pos] = true;
    for (auto ele: g[pos]) {
      int v = ele.first; T1 w = ele.second;
      T nlen = acceletare(res[pos], w);
      if (nlen < res[v]) {
        res[v] = nlen;
        if (w == T1())
          que.push_front(v);
        else
          que.push_back(v);
      }
    }
  }
  return res;
}
// single point alias
template <typename T1, typename T = int>
vector<T> dijkstra01(GraphLength<T1> &g, int s, T maxv = std::numeric_limits<T>::max()
  , function<T(const T&, const T1&)> acceletare = [](const T& a, const T1& b) {
    return a + b;
  }) {
  return dijkstra01(g, vector<int>{s}, maxv, acceletare);
}
}
using graph::dijkstra01;
#endif