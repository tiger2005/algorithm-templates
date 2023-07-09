// decl
#ifndef TEMPLATE_CPP_TWO_SAT
#define TEMPLATE_CPP_TWO_SAT 1
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
using std::vector;
using std::min;
#endif
// impl
#include "lib/graph/graph_link.h"
namespace graph {

struct TwoSat {
  int n;
  GraphLink<bool> g;
  vector<int> col;
  TwoSat(int n = 0, int mm = 0)
    :n(n), g(GraphLink<bool>(2 * n - 1, 0, false, true)) {
    // no value, has direction
    g.reserve(mm * 2);
    col.assign(2 * n, 0);
  }
  void conflict(int a, int b) {
    g.add(a ^ 1, b);
    g.add(b ^ 1, a);
  }
  void must(int a) { g.add(a ^ 1, a); }
  void nmust(int a) { must(a ^ 1); }
  int get(int x) { return (x << 1); }
  int nget(int x) { return (x << 1) + 1; }
  int get(int x, int c) { return (x << 1) + c; }
  bool RUNNED = false;
  void tarjan_scc() {
    if (RUNNED)
      return;
    RUNNED = true;
    int cur = 0, idx = 0, tp = 0;
    col.resize(g.n + 1);
    vector<int> dfn(g.n + 1), low(g.n + 1), stk(g.n + 1);
    vector<bool> in(g.n + 1);
    auto dfs = [&] (auto self, int u) -> void {
      low[u] = dfn[u] = ++ idx;
      in[stk[++ tp] = u] = true;
      for (int id = g.head(u); ~id; id = g.next(id)) {
        int v = g.to(id);
        if (!dfn[v])
          self(self, v), low[u] = min(low[u], low[v]);
        else if (in[v])
          low[u] = min(low[u], dfn[v]);
      }
      if (dfn[u] == low[u] && ++ cur)
        do { col[stk[tp]] = cur; in[stk[tp]] = false; } while (stk[tp --] != u);
    };
    for (int i = 0; i <= g.n; i ++)
      if (!dfn[i]) dfs(dfs, i);
  }
  bool check() {
    tarjan_scc();
    for (int i = 0; i < n; i ++)
      if (col[i << 1] == col[(i << 1) | 1])
        return false;
    return true;
  }
  vector<char> any() {
    tarjan_scc();
    vector<char> res(n);
    for (int i = 0; i < n; i ++)
      if (col[i << 1] == col[(i << 1) | 1])
        return vector<char>{};
      else
        res[i] = (col[i << 1] < col[(i << 1) | 1]);
    return res;
  }
};
}
using graph::TwoSat;
#endif