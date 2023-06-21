// decl
#ifndef TEMPLATE_CPP_DEFS
#define TEMPLATE_CPP_DEFS 1
#ifndef NO_TEMPLATE_IMPORT
#include <string>
#include <vector>
using std::string;
using std::vector;
#endif

// impl
#include "lib/io.h"
#define readV(l, r, A) \
  for (int tiger2005 = (l); tiger2005 <= (r); tiger2005++) iocin >> A[tiger2005]
#define outR(l, r, A, sp) \
  for (int tiger2005 = (l); tiger2005 <= (r); tiger2005++) printf(sp, A[tiger2005])
#define outV(A, sp) \
  for (auto tiger2005 : A) printf(sp, tiger2005)
inline void yOn(bool x) {
  printf("%s", (x) ? "Yes" : "No");
}
inline void yOn(bool x, char* str) { printf("%s%s", (x) ? "Yes" : "No", str); }
inline void yOn(bool x, char str) { printf("%s%c", (x) ? "Yes" : "No", str); }
inline void yOn(bool x, const char* str) { printf("%s%s", (x) ? "Yes" : "No", str); }
inline void yOn(bool x, string str) { printf("%s%s", (x) ? "Yes" : "No", str.c_str()); }
inline void YON(bool x) { printf("%s", (x) ? "YES" : "NO"); }
inline void YON(bool x, char* str) { printf("%s%s", (x) ? "YES" : "NO", str); }
inline void YON(bool x, char str) { printf("%s%c", (x) ? "YES" : "NO", str); }
inline void YON(bool x, const char* str) { printf("%s%s", (x) ? "YES" : "NO", str); }
inline void YON(bool x, string str) { printf("%s%s", (x) ? "YES" : "NO", str.c_str()); }
#define rep(i, l, r) for (auto i = (l); i <= (r); i++)
#define per(i, r, l) for (auto i = (r); i >= (l); i--)
#define openFile(a) freopen(a ".in", "r", stdin), freopen(a ".out", "w", stdout)
#define closeFile() fclose(stdin), fclose(stdout)
#define ALL(x) std::begin(x), std::end(x)
#define multiCase()         \
  int totCases = readInt(); \
  for (int currCase = 1; currCase <= totCases; currCase++)
typedef long long LL;

template <typename T>
void sort(vector<T>& v) { sort(ALL(v)); }

template <typename T>
vector<T> sorted(vector<T> v) { return sort(v), v; }

template <typename T>
void reverse(vector<T>& v) { reverse(ALL(v)); }

template <typename T>
vector<T> reversed(vector<T> v) { return reverse(v), v; }

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

#endif