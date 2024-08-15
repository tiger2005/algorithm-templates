// decl
#ifndef TEMPLATE_CPP_MCMF
#define TEMPLATE_CPP_MCMF 1
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
template <typename T1 = long long, typename T2 = T1>
struct MCMF {
  int n, m, S, E;
  GraphLink<pair<T1, T2>> g;
  vector<T2> dep;
  vector<int> cur;
  vector<bool> vis;
  T2 inf;
  MCMF(int n, int m, int S = 0, int E = 0, T2 inf = std::numeric_limits<T2>::max())
    :n(n), m(m), inf(inf), S(S), E(E), g(GraphLink<pair<T1, T2>>(n, 0, true, true)) {
    dep.assign(n + 1, 0);
    cur.assign(n + 1, 0);
    vis.assign(n + 1, 0);
    g.reserve(m << 1);
  }
  void add(int a, int b, T1 c, T2 d) {
    g.add(a, b, {c, d});
    g.add(b, a, {0, -d});
  }
  void setS(int x) { S = x; }
  void setT(int x) { E = x; }
  bool bfs() {
    queue<int> q;
    std::fill(dep.begin(), dep.end(), inf);
    for (int i = 1; i <= n; i ++)
      cur[i] = g.head(i);
    dep[S] = 0; q.push(S);
    while (!q.empty()) {
      int u = q.front(); q.pop(); vis[u] = false;
      for (int i = g.head(u); ~i; i = g.next(i)) {
        int v = g.to(i); pair<T1, T2> w = g.val(i);
        if (dep[v] > dep[u] + w.second && w.first) {
          dep[v] = dep[u] + w.second;
          if (!vis[v])
            q.push(v), vis[v] = true;
        }
      }
    }
    return dep[E] != dep[0];
  }
  T2 cost;
  T1 dfs(int x, T1 rem) {
    if (x == E || !rem)
      return rem;
    T1 tmp = 0, ret = 0;
    vis[x] = true;
    for (int &i = cur[x]; ~i && ret != rem; i = g.next(i)) {
      int v = g.to(i); pair<T1, T2> w = g.val(i);
      if (!vis[v] && dep[v] == dep[x] + w.second && w.first) {
        tmp = dfs(v, min(rem - ret, w.first));
        if (tmp) {
          cost += tmp * w.second;
          g.val(i).first -= tmp;
          g.val(i ^ 1).first += tmp;
          ret += tmp;
          if (ret == rem) return vis[x] = false, rem;
        }
      }
    }
    vis[x] = false;
    return ret;
  }
  pair<T1, T2> solve() {
    T1 ans = 0; cost = 0;
    while (bfs())
      ans += dfs(S, std::numeric_limits<T1>::max());
    return {ans, cost};
  }
};
}
using graph::MCMF;
#endif