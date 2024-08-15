// decl
#ifndef TEMPLATE_CPP_IO
#define TEMPLATE_CPP_IO 1
#ifndef NO_TEMPLATE_IMPORT
#include <string>
#include <tuple>
#include <vector>
using std::ifstream;
using std::ofstream;
using std::string;
using std::tuple;
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
  inline const IOReader& operator>>(std::pair<T1, T2>& p) const {
    return operator>>(p.first), operator>>(p.second), *this;
  }
  template <typename T, const unsigned long long N>
  inline const IOReader& operator>>(std::array<T, N>& p) const {
    for (unsigned long long i = 0; i < N; i ++)
      operator>>(p[i]);
    return *this;
  }
  template <typename... Ts>
  inline const IOReader& operator>>(std::tuple<Ts...>& p) const;
#undef importRealReader
};
const IOReader io;
#undef gc
template <typename T>
void read(T& val) { io >> val; }
template <typename T>
void read(int l, int r, T& A) { for (int i = l; i <= r; i++) io >> A[i]; }
template <typename T>
void write(const T& A, int l, int r, const char* sp, const char* end = "") { for (int i = l; i <= r; i++) printf(sp, A[i]); printf("%s", end); }
template <typename T>
void write(const auto& A, const T* sp, const char* end = "") {for (auto e : A) printf(sp, e); printf("%s", end); }
template <typename T = int>
T read() { T res; io >> res; return res; }
template <typename T, int N>
std::array<T, N> read() { return read<std::array<T, N>>(); }
template <typename Tuple, typename Func, size_t... N>
void func_call_tuple(Tuple& t, Func&& func, std::index_sequence<N...>) { static_cast<void>(std::initializer_list<int>{(func(std::get<N>(t)), 0)...}); }
template <typename... Args, typename Func>
void travel_tuple(std::tuple<Args...>& t, Func&& func) { func_call_tuple(t, std::forward<Func>(func), std::make_index_sequence<sizeof...(Args)>{}); }
template <typename... Ts>
tuple<Ts...> reads() {
  tuple<Ts...> res;
  travel_tuple(res, [&](auto&& val) { io >> val; });
  return res;
}
template <typename... Ts>
inline const IOReader& IOReader::operator>>(std::tuple<Ts...>& p) const { return p = reads<Ts...>(), *this; }
template <typename T = int>
vector<T> getv(int n, int start = 0) {
  vector<T> res(start + n);
  for (int i = start; i < start + n; i++) io >> res[i];
  return res;
}
template <typename T, typename T1, typename... Ts>
vector<tuple<T, T1, Ts...>> getv(int n, int start = 0) {
  vector<tuple<T, T1, Ts...>> res(start + n);
  for (int i = start; i < start + n; i++) io >> res[i];
  return res;
}
}  // namespace io_lib
using namespace io_lib;
#endif