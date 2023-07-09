// decl
#ifndef TEMPLATE_CPP_GRAPH
#define TEMPLATE_CPP_GRAPH 1
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
using std::pair;
using std::vector;
#endif
// impl
#include "lib/misc/io.h"
namespace graph {
struct Graph {
  int n, m;
  vector<vector<int> > ft;
  bool dirt;
  Graph(int n = 0, int m = 0, bool dirt = false)
      : n(n), m(m), dirt(dirt) {
    ft.assign(n + 1, vector<int>{});
  }
  inline void init() {
    for (int i = 1, a, b; i <= m; i++) {
      io >> a >> b;
      ft[a].emplace_back(b);
      if (!dirt)
        ft[b].emplace_back(a);
    }
  }
  inline void extend(int _n) { ft.resize((n = _n) + 1); }
  inline void add(int a, int b) {
    ++ m;
    ft[a].emplace_back(b);
    if (!dirt)
      ft[b].emplace_back(a);
  }
  vector<int>& operator[](const int& x) {
    return ft[x];
  }
};
template <typename T = int>
struct GraphLength {
  typedef T value_type;
  int n, m;
  vector<vector<pair<int, T> > > ft;
  bool dirt;
  GraphLength(int n = 0, int m = 0, bool dirt = false)
      : n(n), m(m), dirt(dirt) {
    ft.assign(n + 1, vector<pair<int, T> >{});
  }
  inline void init() {
    T c;
    for (int i = 1, a, b; i <= m; i++) {
      io >> a >> b >> c;
      ft[a].emplace_back(b, c);
      if (!dirt)
        ft[b].emplace_back(a, c);
    }
  }
  inline void extend(int _n) { ft.resize((n = _n) + 1); }
  inline void add(int a, int b, T c) {
    ++ m;
    ft[a].emplace_back(b, c);
    if (!dirt)
      ft[b].emplace_back(a, c);
  }
  vector<pair<int, T> >& operator[](const int& x) {
    return ft[x];
  }
};
}  // namespace graph
using graph::Graph;
using graph::GraphLength;
#endif