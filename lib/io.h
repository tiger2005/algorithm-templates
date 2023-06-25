// decl
#ifndef TEMPLATE_CPP_IO
#define TEMPLATE_CPP_IO 1
#ifndef NO_TEMPLATE_IMPORT
#include <iostream>
#include <string>
#include <vector>
using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
#endif
// impl
#ifdef FREAD
#define MAXBUFFERSIZE 1000000
inline char fgetc() {
  static char buf[MAXBUFFERSIZE + 5], *p1 = buf, *p2 = buf;
  return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, MAXBUFFERSIZE, stdin), p1 == p2) ? EOF : *p1++;
}
#undef MAXBUFFERSIZE
#define getchar fgetc
#endif
#define gc getchar
struct IOReader {
  template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
  inline const IOReader& operator>>(T& a) const {
    a = 0;
    bool flg = false;
    char ch = gc();
    while (ch < '0' || ch > '9') {
      if (ch == '-') flg ^= 1;
      ch = gc();
    }
    while (ch >= '0' && ch <= '9') {
      a = (a << 3) + (a << 1) + (ch ^ '0');
      ch = gc();
    }
    if (flg) a = -a;
    return *this;
  }
  inline const IOReader& operator>>(string& a) const {
    a.clear();
    char ch = gc();
    while (isspace(ch) && ch != EOF) ch = gc();
    while (!isspace(ch) && ch != EOF) a += ch, ch = gc();
    return *this;
  }
  inline const IOReader& operator>>(char* a) const {
    char ch = gc();
    while (isspace(ch) && ch != EOF) ch = gc();
    while (!isspace(ch) && ch != EOF) *(a++) = ch, ch = gc();
    *a = '\0';
    return *this;
  }
  inline const IOReader& operator>>(char& a) const {
    a = gc();
    while (isspace(a)) a = gc();
    return *this;
  }
  template <typename T, std::enable_if_t<std::is_floating_point<T>::value, int> = 0>
  inline const IOReader& operator>>(T& a) const {
    a = 0;
    bool flg = false;
    char ch = gc();
    while ((ch < '0' || ch > '9') && ch != '.') {
      if (ch == '-') flg ^= 1;
      ch = gc();
    }
    while (ch >= '0' && ch <= '9') {
      a = a * 10 + (ch ^ '0');
      ch = gc();
    }
    if (ch == '.') {
      ch = gc();
      T p = 0.1;
      while (ch >= '0' && ch <= '9') {
        a += p * (ch ^ '0');
        ch = gc();
        p *= 0.1;
      }
    }
    if (flg) a = -a;
    return *this;
  }
#undef importRealReader
};
const IOReader io;
#define importReader(type, name) \
  type name() {                  \
    type a(0);                   \
    io >> a;                     \
    return a;                    \
  }
importReader(int, readInt) importReader(unsigned int, readUInt)
importReader(long long, readLL) importReader(unsigned long long, readULL)
importReader(short, readShort) importReader(unsigned short, readUShort)
importReader(float, readFL) importReader(double, readDB) importReader(long double, readLDB)
importReader(string, readToken)
#undef importReader
#undef gc
template <typename T = int>
vector<T> getv(int n, int start = 0) {
  vector<T> res(start + n);
  for (int i = start; i < start + n; i++)
    io >> res[i];
  return res;
}
#endif