// decl
#ifndef TEMPLATE_CPP_KM
#define TEMPLATE_CPP_KM 1
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
#include <limits>
#include <queue>
using std::vector;
using std::min;
using std::queue;
using std::swap;
#endif
// impl
namespace graph {
template <typename T = long long>
struct KM {
  int n;
  vector<T> lx, ly;
  vector<vector<T>> w;
  vector<T> slack;
  vector<int> matchx, matchy, pre;
  vector<bool> visx, visy;
  T inf, res;
  queue<int> q;
  KM (int n, T _inf = std::numeric_limits<T>::max() / 2) :n(n) {
    inf = _inf; res = 0;
    w = vector<vector<T> >(n, vector<T>(n, -inf));
    matchx = vector<int>(n, -1);
    matchy = vector<int>(n, -1);
    pre = vector<int>(n);
    visx = vector<bool>(n);
    visy = vector<bool>(n);
    lx = vector<T>(n, -inf);
    ly = vector<T>(n);
    slack = vector<T>(n);
  }
  void add(int a, int b, T c) { w[a][b] = c; }
  void getw(int x, int y) { return w[x][y]; }
  bool check(int v) {
    visy[v] = true;
    if (matchy[v] != -1) {
      int u = matchy[v];
      q.push(u); visx[u] = true;
      return false;
    }
    while (v != -1) {
      matchy[v] = pre[v];
      swap(v, matchx[pre[v]]);
    }
    return true;
  }
  void bfs(int x) {
    while (!q.empty()) q.pop();
    visx[x] = true; q.push(x);
    while (1) {
      while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v = 0; v < n; v ++) if (!visy[v]) {
          T delta = lx[u] + ly[v] - w[u][v];
          if (slack[v] > delta) {
            pre[v] = u;
            slack[v] = delta;
            if (!delta && check(v))
              return;
          }
        }
      }
      T a = inf;
      for (int i = 0; i < n; i ++)
        if (!visy[i]) a = min(a, slack[i]);
      for (int i = 0; i < n; i ++) {
        if (visx[i]) lx[i] -= a;
        if (visy[i]) ly[i] += a;
        else slack[i] -= a;
      }
      for (int i = 0; i < n; i ++)
        if (!visy[i] && slack[i] == 0 && check(i))
          return;
    }
  }
  T solve() {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        lx[i] = max(lx[i], w[i][j]);
      }
    }
    for (int i = 0; i < n; i++) {
      fill(slack.begin(), slack.end(), inf);
      fill(visx.begin(), visx.end(), false);
      fill(visy.begin(), visy.end(), false);
      bfs(i);
    }
    res = 0;
    for (int i = 0; i < n; i ++)
      if (~matchx[i]) res += w[i][matchx[i]];
    return res;
  }
  int matchL(int p) { return matchx[p]; }
  int matchR(int p) { return matchy[p]; }
};
}
using graph::KM;
#endif