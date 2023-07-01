// decl
#ifndef TEMPLATE_CPP_COMPONENTS
#define TEMPLATE_CPP_COMPONENTS 1
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
#include <functional>
using std::vector;
using std::pair;
using std::tuple;
using std::function;
#endif
// impl
#include "lib/graph/graph.h"
namespace graph {
template <typename T>
vector<pair<vector<int>, GraphLength<T>>> components (GraphLength<T> g) {
  vector<bool> vis(g.n + 1);
  vector<pair<vector<int>, GraphLength<T>>> res;
  vector<int> nodes;
  vector<tuple<int, int, T>> vp;
  function<int(int)> dfs = [&] (int x) -> int {
    nodes.push_back(x); vis[x] = true;
    int idx = nodes.size();
    for (auto [v, w]: g[x]) if (!vis[v])
      vp.emplace_back(idx, dfs(v), w);
    return idx;
  };
  for (int i = 1; i <= g.n; i ++) if (!vis[i]) {
    vp.clear(); nodes.clear();
    dfs(i);
    GraphLength<T> _g(nodes.size(), 0, g.dirt);
    for (auto [x, y, w]: vp)
      _g.add(x, y, w);
    res.emplace_back(nodes, _g);
  }
  return res;
}

vector<pair<vector<int>, Graph>> components (Graph g) {
  vector<bool> vis(g.n + 1);
  vector<pair<vector<int>, Graph>> res;
  vector<int> nodes;
  vector<pair<int, int>> vp;
  function<int(int)> dfs = [&] (int x) -> int {
    nodes.push_back(x); vis[x] = true;
    int idx = nodes.size();
    for (auto v: g[x]) if (!vis[v])
      vp.emplace_back(idx, dfs(v));
    return idx;
  };
  for (int i = 1; i <= g.n; i ++) if (!vis[i]) {
    vp.clear(); nodes.clear();
    dfs(i);
    Graph _g(nodes.size(), 0, g.dirt);
    for (auto [x, y]: vp)
      _g.add(x, y);
    res.emplace_back(nodes, _g);
  }
  return res;
}
}
#endif