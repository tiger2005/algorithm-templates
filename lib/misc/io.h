// decl
#ifndef TEMPLATE_CPP_IO
#define TEMPLATE_CPP_IO 1
#ifndef NO_TEMPLATE_IMPORT
#include <string>
#include <vector>
using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
#endif
// impl
namespace io_lib {
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
  template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
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
  template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
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
  template <typename T1, typename T2>
  inline const IOReader& operator>>(std::pair<T1, T2> &p) const {
    return operator>>(p.first), operator>>(p.second), *this;
  }
#undef importRealReader
};
const IOReader io;
#define importReader(type, name) \
  type name() {                  \
    type a;                     \
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
}
using namespace io_lib;
#endif