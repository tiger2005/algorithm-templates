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
#include "lib/misc/io.h"
namespace defs {
inline void Yes(bool x, string str = "") { printf("%s%s", (x) ? "Yes" : "No", str.c_str()); }
inline void YES(bool x, string str = "") { printf("%s%s", (x) ? "YES" : "NO", str.c_str()); }
inline void yes(bool x, string str = "") { printf("%s%s", (x) ? "yes" : "no", str.c_str()); }
inline void Yes(bool x, char ch) { printf("%s%c", (x) ? "Yes" : "No", ch); }
inline void YES(bool x, char ch) { printf("%s%c", (x) ? "YES" : "NO", ch); }
inline void yes(bool x, char ch) { printf("%s%c", (x) ? "yes" : "no", ch); }
#define openFile(a) freopen(a ".in", "r", stdin), freopen(a ".out", "w", stdout)
#define closeFile() fclose(stdin), fclose(stdout)
#define ALL(x) std::begin(x), std::end(x)
#define multiCase()         \
  int totCases; cin >> totCases; \
  for (int currCase = 1; currCase <= totCases; currCase++)
using i32 = int;
using u32 = unsigned int;
using i64 = long long;
using u64 = unsigned long long;
using i128 = __int128;
using u128 = __uint128_t;
using f32 = float;
using f64 = double;
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
auto index(const vector<T>& v, int n = 0) {
  vector<pair<int, T>> res;
  for (int i = 0; i < (int) v.size(); i++)
    res.push_back({n++, v[i]});
  return res;
}
template <typename T>
auto pairwise(const vector<T>& v) {
  vector<pair<T, T>> res;
  for (int i = 1; i < (int)v.size(); i ++)
    res.push_back({v[i - 1], v[i]});
  return res;
}
auto index(const string& v, int n = 0) {
  return index(vector<char>(v.begin(), v.end()), n);
}
auto pairwise(const string& v) {
  return pairwise(vector<char>(v.begin(), v.end()));
}
template <typename T>
bool ckmax(T& a, const T& b) {
  return a < b ? (a = b, true) : false;
}
template <typename T>
bool ckmin(T& a, const T& b) {
  return b < a ? (a = b, true) : false;
}
template <typename T, typename T2>
bool ckmax(T& a, T2 b) {
  return a < b ? (a = b, true) : false;
}
template <typename T, typename T2>
bool ckmin(T& a, T2 b) {
  return b < a ? (a = b, true) : false;
}
}  // namespace defs
using namespace defs;
#endif