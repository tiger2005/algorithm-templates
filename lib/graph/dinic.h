// decl
#ifndef TEMPLATE_CPP_DINIC
#define TEMPLATE_CPP_DINIC 1
#ifndef NO_TEMPLATE_IMPORT
#include <algorithm>
#include <queue>
#include <limits>
using std::queue;
using std::pair;
using std::min;
#endif
// impl
#include "lib/graph/graph_link.h"
namespace graph {

template <typename T = long long>
struct Dinic {
  int n, m, S, E;
  GraphLink<T> g;
  vector<int> dep, cur;
  Dinic(int n = 0, int m = 0, int S = 0, int E = 0)
    :n(n), m(m), S(S), E(E), g(GraphLink<T>(n, 0, true, true)) {
    dep.assign(n + 1, 0);
    cur.assign(n + 1, 0);
    g.reserve(m << 1);
  }
  void add(int a, int b, T c) {
    g.add(a, b, c);
    g.add(b, a, 0);
  }
  void setS(int x) { S = x; }
  void setT(int x) { E = x; }
  bool bfs() {
    queue<int> q;
    std::fill(dep.begin(), dep.end(), 0);
    for (int i = 1; i <= n; i ++)
      cur[i] = g.head(i);
    dep[S] = 1; q.push(S);
    while (!q.empty()) {
      int u = q.front(); q.pop();
      for (int i = g.head(u); ~i; i = g.next(i)) {
        int v = g.to(i); T w = g.val(i);
        if (!dep[v] && w) {
          dep[v] = dep[u] + 1;
          q.push(v);
        }
      }
    }
    return dep[E] != 0;
  }
  T dfs(int x, T rem) {
    if (x == E || !rem)
      return rem;
    T tmp = 0, ret = 0;
    for (int &i = cur[x]; ~i; i = g.next(i)) {
      int v = g.to(i); T w = g.val(i);
      if (dep[v] == dep[x] + 1 && w) {
        tmp = dfs(v, min(rem - ret, w));
        if (!tmp) dep[v] = n + 1; // enough for inf
        else {
          g.val(i) -= tmp;
          g.val(i ^ 1) += tmp;
          if ((ret += tmp) == rem)
            return rem;
        }
      }
    }
    return ret;
  }
  T solve() {
    T ans = 0;
    while (bfs())
      ans += dfs(S, std::numeric_limits<T>::max());
    return ans;
  }
};
}
using graph::Dinic;
#endif