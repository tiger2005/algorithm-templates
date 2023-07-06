// decl
#ifndef TEMPLATE_CPP_STRUCTURE
#define TEMPLATE_CPP_STRUCTURE 1
#ifndef NO_TEMPLATE_IMPORT
#endif
// impl
#include "lib/graph/graph.h"
namespace graph {
template <typename T>
inline Graph structure(GraphLength<T> &g) {
  Graph res(g.n, 0, g.dirt);
  for (int i = 1; i <= g.n; i ++)
    for (auto [v, w]: g[i])
      res.add(i, v);
  return res;
}
}
#endif