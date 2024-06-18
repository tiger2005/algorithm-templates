// decl
#ifndef TEMPLATE_CPP_DEFS
#define TEMPLATE_CPP_DEFS 1
#ifndef NO_TEMPLATE_IMPORT
#include <string>
#include <vector>
using std::pair;
using std::string;
using std::vector;
#endif
// impl
#include "lib/misc/debug.h"
#include "lib/misc/io.h"
namespace defs {
inline void yOn(bool x, string str = "") { printf("%s%s", (x) ? "Yes" : "No", str.c_str()); }
inline void YON(bool x, string str = "") { printf("%s%s", (x) ? "YES" : "NO", str.c_str()); }
inline void yOn(bool x, char ch) { printf("%s%c", (x) ? "Yes" : "No", ch); }
inline void YON(bool x, char ch) { printf("%s%c", (x) ? "YES" : "NO", ch); }
#define openFile(a) freopen(a ".in", "r", stdin), freopen(a ".out", "w", stdout)
#define closeFile() fclose(stdin), fclose(stdout)
#define ALL(x) std::begin(x), std::end(x)
#define multiCase()         \
  int totCases = readInt(); \
  for (int currCase = 1; currCase <= totCases; currCase++)
typedef long long LL;
template <typename T>
void sort(T& v) { sort(ALL(v)); }
template <typename T>
T sorted(T v) { return sort(v), v; }
template <typename T>
void sort(T& v, auto compare) { sort(ALL(v), compare); }
template <typename T>
T sorted(T v, auto compare) { return sort(v, compare), v; }
template <typename T>
void reverse(T& v) { reverse(ALL(v)); }
template <typename T>
T reversed(T v) { return reverse(v), v; }
template <typename T>
void unique(vector<T>& v) { v.erase(unique(ALL(v)), v.end()); }
template <typename T>
vector<T> uniqued(vector<T> v) { return unique(v), v; }
bool in(char ch, string str) {
  for (auto x : str)
    if (ch == x)
      return true;
  return false;
}
template <typename T>
bool in(T v, vector<T> V) {
  for (auto x : V)
    if (v == x)
      return true;
  return false;
}
template <typename T>
vector<pair<int, T>> index(vector<T> v, int n = 0) {
  vector<pair<int, T>> res;
  for (int i = 0; i < (int) v.size(); i++)
    res.emplace_back(n++, v[i]);
  return res;
}
template <typename T>
bool ckmax(T& a, const T& b) {
  return a < b ? (a = b, true) : false;
}
template <typename T>
bool ckmin(T& a, const T& b) {
  return b < a ? (a = b, true) : false;
}
}  // namespace defs
using namespace defs;
#endif