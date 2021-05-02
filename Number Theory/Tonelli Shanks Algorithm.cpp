#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

int power(long long n, long long k, const int mod) {
  int ans = 1 % mod; n %= mod; if (n < 0) n += mod;
  while (k) {
    if (k & 1) ans = (long long) ans * n % mod;
    n = (long long) n * n % mod;
    k >>= 1;
  }
  return ans;
}
// find sqrt(a) % p, i.e. find any x such that x^2 = a (mod p)
// if a solution exist, then if a == 0 or p == 2, there are 1 solution, otherwise, there are exactly 2 solutions (x and p - x)
// p is prime
// complexity: O(log^2 p) worst case, O(log p) on average
int SQRT(int a, int p) {
  a %= p; if (a < 0) a += p;
  if (a == 0) return 0;
  if (power(a, (p - 1) / 2, p) != 1) return -1; // solution does not exist
  if (p % 4 == 3) return power(a, (p + 1) / 4, p);
  int s = p - 1, n = 2;
  int r = 0, m;
  while (s % 2 == 0) ++r, s /= 2;
  // find a non-square mod p
  while (power(n, (p - 1) / 2, p) != p - 1) ++n;
  int x = power(a, (s + 1) / 2, p);
  int b = power(a, s, p), g = power(n, s, p);
  for (;; r = m) {
    int t = b;
    for (m = 0; m < r && t != 1; ++m) t = 1LL * t * t % p;
    if (m == 0) return x;
    int gs = power(g, 1LL << (r - m - 1), p);
    g = 1LL * gs * gs % p;
    x = 1LL * x * gs % p;
    b = 1LL * b * g % p;
  }
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t; cin >> t;
  while (t--) {
    int x, p; cin >> x >> p;
    cout << SQRT(x, p) << '\n';
  }
  return 0;
}