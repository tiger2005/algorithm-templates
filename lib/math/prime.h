// decl
#ifndef TEMPLATE_CPP_PRIME
#define TEMPLATE_CPP_PRIME 1
#ifndef NO_TEMPLATE_IMPORT
#include <vector>
#include <cmath>
using std::vector;
#endif
// impl
namespace math {
namespace prime {
vector<bool> excludes({false, false});
vector<int> primes;
int n_field = 1;
void expand(int n) {
  excludes.resize(n + 1);
  // extra sieve
  for (auto p: primes) {
    int start = (n_field / p + 1) * p;
    for (; start <= n; start += p)
      excludes[start] = true;
  }
  // main
  for (int i = n_field + 1; i <= n; i ++) {
    if (!excludes[i])
      primes.push_back(i);
    for (auto p: primes) {
      if (i * p > n)
        break;
      excludes[i * p] = true;
      if (i % p == 0)
        break;
    }
  }
  n_field = n;
}
vector<int> getPrimes(int n) {
  return expand(n), primes;
}
vector<bool> getPrimeMap(int n) {
  return expand(n), excludes;
}
// sqrt(r), r - l should be small
vector<long long> rangePrimes(long long l, long long r) {
  int len = r - l;
  vector<bool> temp(r - l + 1);
  if (l == 1)
    temp[0] = true;
  int u = sqrt(r);
  while (1ll * (u + 1) * (u + 1) <= r)
    ++ u;
  expand(u);
  for (auto p: primes) {
    if (p <= u) {
      int start = l % p;
      start = (p & -(start != 0)) - start;
      if (start + l == p)
        start += p;
      for(; start <= len; start += p)
        temp[start] = true;
    }
    else
      break;
  }
  vector<long long> res;
  for (int i = 0; i <= len; i ++)
    if (!temp[i])
      res.push_back(l + i);
  return res;
}
}
};
using math::prime::getPrimes;
using math::prime::getPrimeMap;
using math::prime::rangePrimes;
#endif