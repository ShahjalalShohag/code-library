#include<bits/stdc++.h>
using namespace std;

int fib(long long n, int mod) {
  assert (n >= 0);
  if (n <= 1) return n;
  int a = 0, b = 1;
  long long i = 1ll << (63 - __builtin_clzll(n) - 1);
  for (; i; i >>= 1) {
    int na = (a *(long long) a + b *(long long) b) % mod;
    int nb = (2ll * a + b) * b % mod;
    a = na; b = nb;
    if (n & i) {
      int c = a + b; if (c >= mod) c -= mod;
      a = b; b = c;
    }
  }
  return b;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  cout << fib(10, 100) << '\n';
  return 0;
}