// decl
#ifndef TEMPLATE_CPP_BCC_V
#define TEMPLATE_CPP_BCC_V 1
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
using std::vector;
using std::min;
#endif
// impl
#include "lib/graph/graph.h"
namespace graph {
vector<vector<int>> bcc_v(Graph &g) {
  int idx = 0, tp = 0, fa = 0;
  vector<int> dfn(g.n + 1), low(g.n + 1), stk(g.n + 1);
  vector<vector<int>> res;
  auto dfs = [&] (auto self, int u) -> void {
    dfn[u] = low[u] = ++ idx;
    int cnt = 0;
    stk[++ tp] = u;
    for (auto v: g[u]) {
      if (! dfn[v]) {
        ++ cnt;
        self(self, v); low[u] = min(low[u], low[v]);
        if (low[v] >= dfn[u]) {
          vector<int> tmp({u});
          do { tmp.push_back(stk[tp]); } while (stk[tp --] != v);
          res.push_back(tmp);
        }
      }
      else
        low[u] = min(low[u], dfn[v]);
    }
    if (u == fa && cnt == 0)
      res.push_back({u});
  };
  for (int i = 1; i <= g.n; i ++)
    if (!dfn[i]) dfs(dfs, fa = i);
  return res;
}
}
using graph::bcc_v;
#endif