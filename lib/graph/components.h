// decl
#ifndef TEMPLATE_CPP_COMPONENTS
#define TEMPLATE_CPP_COMPONENTS 1
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
using std::vector;
using std::pair;
using std::tuple;
#endif
// impl
#include "lib/graph/graph.h"
namespace graph {
template <typename T>
vector<pair<vector<int>, T>> components (T g) {
  vector<bool> vis(g.n + 1);
  vector<pair<vector<int>, T>> res;
  vector<int> nodes;
  if constexpr (std::is_same<T, Graph>::value) {
    vector<pair<int, int>> vp;
    auto dfs = [&] (auto self, int x) -> int {
      nodes.push_back(x); vis[x] = true;
      int idx = nodes.size();
      for (auto v: g[x]) if (!vis[v])
        vp.emplace_back(idx, self(self, v));
      return idx;
    };
    for (int i = 1; i <= g.n; i ++) if (!vis[i]) {
      vp.clear(); nodes.clear();
      dfs(dfs, i);
      T _g(nodes.size(), 0, g.dirt);
      for (auto [x, y]: vp)
        _g.add(x, y);
      res.emplace_back(nodes, _g);
    }
    return res;
  }
  else {
    vector<tuple<int, int, typename T::value_type>> vp;
    auto dfs = [&] (auto self, int x) -> int {
      nodes.push_back(x); vis[x] = true;
      int idx = nodes.size();
      for (auto [v, w]: g[x]) if (!vis[v])
        vp.emplace_back(idx, self(self, v), w);
      return idx;
    };
    for (int i = 1; i <= g.n; i ++) if (!vis[i]) {
      vp.clear(); nodes.clear();
      dfs(dfs, i);
      T _g(nodes.size(), 0, g.dirt);
      for (auto [x, y, w]: vp)
        _g.add(x, y, w);
      res.emplace_back(nodes, _g);
    }
    return res;
  }
}
}
#endif