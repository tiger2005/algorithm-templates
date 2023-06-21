// decl
#ifndef TEMPLATE_CPP_UNION
#define TEMPLATE_CPP_UNION 1
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
using std::vector;
#endif

// impl
namespace ds {

struct UnionSet {
  int n;
  vector<int> fa;
  UnionSet(int n = 0) : n(n) {
    fa.assign(n + 1, 0);
    for (int i = 1; i <= n; i++)
      fa[i] = i;
  }
  int getF(int x) {
    return x == fa[x] ? x : fa[x] = getF(fa[x]);
  }
  void link(int x, int y) {
    x = getF(x);
    y = getF(y);
    if (x != y)
      fa[x] = y;
  }
  int operator[](const int& x) {
    return getF(x);
  }
};

}  // namespace ds

using Union = ds::UnionSet;
#endif