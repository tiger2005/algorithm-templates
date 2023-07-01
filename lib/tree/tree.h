// decl
#ifndef TEMPLATE_CPP_TREE
#define TEMPLATE_CPP_TREE 1
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
using std::vector;
#endif
// impl
#include "lib/graph/graph.h"
#define OBTAIN_TREE_VALUE 1
#define OBTAIN_TREE_DEPTH 2
#define OBTAIN_TREE_SIZE 4
namespace tree {
template <typename T, const unsigned short OBT = 0>
struct Tree {
  int n, root;
  vector<T> val;
  vector<int> fa;
  vector<vector<int>> son;
  vector<int> deps;
  vector<int> sizes;
  Tree (int n, int root = 1, T p = T()) :n(n), root(root) {
    son.assign(n + 1, vector<int>{});
    fa.assign(n + 1, 0);
    if constexpr (OBT & OBTAIN_TREE_VALUE) val.assign(n + 1, p);
    else val.clear();
    if constexpr (OBT & OBTAIN_TREE_DEPTH) deps.assign(n + 1, 0);
    else deps.clear();
    if constexpr (OBT & OBTAIN_TREE_SIZE) sizes.assign(n + 1, 0);
    else sizes.clear();
  }
  void init(bool value = false) {
    if (value) {
      graph::GraphLength<T> g(n, n - 1);
      g.init(); init(g);
    }
    else {
      Graph g(n, n - 1);
      g.init(); init(g);
    }
  }
  void init(Graph g, int nr = -1) {
    if (nr == -1)
      nr = root;
    fa[nr] = nr;
    if constexpr (OBT & OBTAIN_TREE_DEPTH) deps[nr] = 1;
    auto dfs = [&] (auto self, int x) -> void {
      if constexpr (OBT & OBTAIN_TREE_SIZE) sizes[x] = 1;
      for (auto v: g[x]) {
        if (fa[v])
          continue;
        son[x].push_back(v);
        if constexpr (OBT & OBTAIN_TREE_DEPTH) deps[v] = deps[x] + 1;
        fa[v] = x; self(self, v);
        if constexpr (OBT & OBTAIN_TREE_SIZE) sizes[x] += sizes[v];
      }
    };
    dfs(dfs, nr);
  }
  void init(graph::GraphLength<T> g, int nr = -1) {
    if (nr == -1)
      nr = root;
    fa[nr] = nr;
    if constexpr (OBT & OBTAIN_TREE_DEPTH) deps[nr] = 1;
    auto dfs = [&] (auto self, int x) -> void {
      if constexpr (OBT & OBTAIN_TREE_SIZE) sizes[x] = 1;
      for (auto [v, w]: g[x]) {
        if (fa[v])
          continue;
        son[x].push_back(v);
        if constexpr (OBT & OBTAIN_TREE_DEPTH) deps[v] = deps[x] + 1;
        if constexpr (OBT & OBTAIN_TREE_VALUE) val[v] = w;
        fa[v] = x;
        self(self, v);
        if constexpr (OBT & OBTAIN_TREE_SIZE) sizes[x] += sizes[v];
      }
    };
    dfs(dfs, nr);
  }
  inline const vector<int>& sons(int x) { return son[x]; }
  inline const vector<int>& operator[](int x) { return son[x]; }
  inline bool visited(int x) { return fa[x] != 0; }
  inline int father(int x) { return fa[x]; }
  #if OBT & OBTAIN_TREE_VALUE
  inline T value(int x) { return val[x]; }
  #endif
  #if OBT & OBTAIN_TREE_DEPTH
  inline int depth(int x) { return deps[x]; }
  #endif
  #if OBT & OBTAIN_TREE_SIZE
  inline int size(int x) { return sizes[x]; }
  #endif
};
}
using Tree = tree::Tree<int>;
#endif