// decl
#ifndef TEMPLATE_CPP_GRAPH_LINK
#define TEMPLATE_CPP_GRAPH_LINK 1
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
using std::vector;
#endif
// impl
#include "lib/misc/io.h"
namespace graph {
template <typename T = bool>
struct GraphLink {
  int n, m; bool hasVal, dirt;
  vector<int> hd, tow, nxt;
  vector<T> vals;
  GraphLink(int n, int m, bool hasVal = false, bool dirt = false)
    :n(n), m(m), hasVal(hasVal), dirt(dirt) {
    clear(m);
  }
  void clear(int mm) {
    tow.clear(); vals.clear(); nxt.clear();
    hd.assign(n + 1, -1);
    if (mm)
      reserve(mm);
  }
  void reserve(int mm) {
    nxt.reserve(mm * (dirt ? 1 : 2));
    tow.reserve(mm * (dirt ? 1 : 2));
    if (hasVal)
      vals.reserve(mm * (dirt ? 1 : 2));
  }
  void add_raw(int a, int b, T c = T()) {
    tow.push_back(b);
    if (hasVal)
      vals.push_back(c);
    nxt.push_back(hd[a]);
    hd[a] = (m ++);
  }
  void add(int a, int b, T c = T()) {
    add_raw(a, b, c);
    if (!dirt)
      add_raw(b, a, c);
  }
  void init() {
    int u = m; clear(m); m = 0; T c;
    for (int a, b; u; u --) {
      io >> a >> b >> c;
      add(a, b, c);
    }
  }
  int &head(int x) { return hd[x]; }
  int &next(int x) { return nxt[x]; }
  int &to(int x) { return tow[x]; }
  T &val(int x) { return vals[x]; }
};
}
using graph::GraphLink;
#endif