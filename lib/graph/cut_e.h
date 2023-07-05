// decl
#ifndef TEMPLATE_CPP_CUT_E
#define TEMPLATE_CPP_CUT_E 1
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
using std::vector;
using std::pair;
using std::min;
#endif
// impl
#include "lib/graph/graph.h"
namespace graph {
vector<pair<int, int>> cut_e(Graph &g) {
  int idx = 0;
  vector<int> dfn(g.n + 1), low(g.n + 1);
  vector<pair<int, int>> cur;
  auto dfs = [&] (auto self, int u, int fa) -> void {
    low[u] = dfn[u] = ++ idx;
    bool mul = false;
    for (auto v: g[u]) {
      if (!dfn[v]) {
        self(self, v, u), low[u] = min(low[u], low[v]);
        if (low[v] > dfn[u])
          cur.emplace_back(u, v);
      }
      else if (v != fa || mul)
        low[u] = min(low[u], dfn[v]);
      else mul = true;
    }
  };
  for (int i = 1; i <= g.n; i ++)
    if (!dfn[i]) dfs(dfs, i, 0);
  return cur;
}
}
using graph::cut_e;
#endif