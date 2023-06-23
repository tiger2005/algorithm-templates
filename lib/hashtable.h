// decl
#ifndef TEMPLATE_CPP_HASHTABLE
#define TEMPLATE_CPP_HASHTABLE 1
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
using std::vector;
#endif
// impl
template <typename T>
struct HashTable {
  vector<T> val;
  void add(T x) { val.push_back(x); }
  void init() {
    sort(ALL(val));
    val.erase(unique(ALL(val)), val.end());
  }
  int query(T x) { return lower_bound(ALL(val), x) - val.begin() + 1; }
  T operator[](const int t) const { return val[t - 1]; }
  int size() { return val.size(); }
};
#endif