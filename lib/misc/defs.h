// decl
#ifndef TEMPLATE_CPP_DEFS
#define TEMPLATE_CPP_DEFS 1
#ifndef NO_TEMPLATE_IMPORT
#include <string>
#include <vector>
using std::string;
using std::vector;
using std::pair;
#endif
// impl
#include "lib/misc/io.h"
namespace defs {
template <typename T>
void readV(int l, int r, T &A) { for (int i = l; i <= r; i ++) io >> A[i]; }
template <typename T>
void outR(int l, int r, T &A, const char* sp) { for (int i = l; i <= r; i ++) printf(sp, A[i]); }
template <typename T>
void outV(auto &A, const T* sp) { for (auto e: A) printf(sp, e); }
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
template <typename T> void sort(vector<T>& v) { sort(ALL(v)); }
template <typename T> vector<T> sorted(vector<T> v) { return sort(v), v; }
template <typename T> void reverse(vector<T>& v) { reverse(ALL(v)); }
template <typename T> vector<T> reversed(vector<T> v) { return reverse(v), v; }
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
  for (int i = 0; i < (int)v.size(); i ++)
    res.emplace_back(n ++, v[i]);
  return res;
}
}
using namespace defs;
#endif