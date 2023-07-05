// decl
#ifndef TEMPLATE_CPP_CUT_V
#define TEMPLATE_CPP_CUT_V 1
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
using std::vector;
using std::min;

#endif
// impl
#include "lib/graph/graph.h"
namespace graph {
vector<int> cut_v(Graph &g) {
  int idx = 0;
  vector<int> dfn(g.n + 1), low(g.n + 1);
  vector<int> cur; int fa = 0;
  auto dfs = [&] (auto self, int u) -> void {
    low[u] = dfn[u] = ++ idx;
    int s = 0; bool used = false;
    for (auto v: g[u]) {
      if (!dfn[v]) {
        self(self, v), low[u] = min(low[u], low[v]), ++ s;
        if (low[v] >= dfn[u] && fa != u && !used)
          cur.push_back(u), used = true;
      }
      else low[u] = min(low[u], dfn[v]);
    }
    if (u == fa && s >= 2 && !used)
      cur.push_back(u);
  };
  for (int i = 1; i <= g.n; i ++)
    if (!dfn[i]) dfs(dfs, fa = i);
  return cur;
}
}
using graph::cut_v;
#endif