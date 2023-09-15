// decl
#ifndef TEMPLATE_CPP_ST
#define TEMPLATE_CPP_ST 1
#ifndef NO_TEMPLATE_IMPORT
#include <functional>
#include <vector>
using std::function;
using std::vector;
#endif
// impl
namespace ds {
template <typename T>
struct ST {
  vector<vector<T>> table;
  vector<int> Lg;
  int n;
  static T default_func(const T &t1, const T &t2) { return max(t1, t2); }
  function<T(const T &, const T &)> op = default_func;
  ST() {
    table.clear();
    Lg = vector<int>{0, 0};
    n = 0;
  }
  ST(vector<T> v, function<T(const T &, const T &)> func = default_func) {
    Lg = vector<int>{0, 0};
    init(v, func);
  }
  void init(vector<T> v, function<T(const T &, const T &)> func) {
    op = func;
    table.clear();
    n = v.size();
    if ((int) Lg.size() <= n) {
      int tmp = Lg.size();
      Lg.resize(n + 1);
      for (int i = tmp; i <= n; i++)
        Lg[i] = Lg[i >> 1] + 1;
    }
    int l = Lg[n];
    table.push_back(v);
    for (int i = 1, tmp; i <= l; i++) {
      table.push_back(vector<int>(tmp = n - (1 << i) + 1));
      for (int j = 0; j < tmp; j++)
        table[i][j] = op(table[i - 1][j], table[i - 1][j + (1 << (i - 1))]);
    }
  }
  T query(int l, int r) {
    int u = Lg[r - l + 1];
    return op(table[u][l], table[u][r - (1 << u) + 1]);
  }
};
}  // namespace ds
using ds::ST;
#endif