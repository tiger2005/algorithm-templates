// decl
#ifndef TEMPLATE_CPP_SCC
#define TEMPLATE_CPP_SCC 1
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
using std::vector;
using std::min;
#endif
// impl
#include "lib/graph/graph.h"
namespace graph {
int scc(Graph &g, vector<int> &col, vector<int> &dep) {
  int cur = 0, idx = 0, tp = 0;
  col.resize(g.n + 1);
  dep.resize(g.n + 1);
  vector<int> dfn(g.n + 1), low(g.n + 1), stk(g.n + 1);
  vector<bool> in(g.n + 1);
  auto dfs = [&] (auto self, int u) -> void {
    low[u] = dfn[u] = ++ idx;
    in[stk[++ tp] = u] = true;
    for (auto v: g[u]) {
      if (!dfn[v])
        dep[v] = dep[u] + 1, self(self, v), low[u] = min(low[u], low[v]);
      else if (in[v])
        low[u] = min(low[u], dfn[v]);
    }
    if (dfn[u] == low[u] && ++ cur)
      do { col[stk[tp]] = cur; in[stk[tp]] = false; } while (stk[tp --] != u);
  };
  for (int i = 1; i <= g.n; i ++)
    if (!dfn[i]) dfs(dfs, i);
  return cur;
}
int scc(Graph &g, vector<int> &col) {
  vector<int> tmp; return scc(g, col, tmp);
}
}
using graph::scc;
#endif