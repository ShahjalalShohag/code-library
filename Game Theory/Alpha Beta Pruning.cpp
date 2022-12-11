#include<bits/stdc++.h>
using namespace std;

const int N = 3e7 + 9, mod = 1e9 + 7, inf = mod;

int power(long long n, long long k) {
  int ans = 1 % mod; n %= mod; if (n < 0) n += mod;
  while (k) {
    if (k & 1) ans = (long long) ans * n % mod;
    n = (long long) n * n % mod;
    k >>= 1;
  }
  return ans;
}
int n, target, a[50], fac[N], ifac[N];
int ncr(int n, int k) {
  return 1LL * fac[n] * ifac[k] % mod * ifac[n - k] % mod;
}
int solve(int n, int k) {
  return 1LL * fac[n] * ncr(n - 1, n - k) % mod * ifac[k] % mod;
}
int yo(int i, int which, int total_balls, int count_balls, int alpha, int beta) {
  if (i == n + 1) {
    return abs(solve(total_balls, count_balls) - target);
  }
  if (which) {
    int ans = 0;
    int val = yo(i + 1, which ^ 1, total_balls, count_balls, alpha, beta);
    alpha = max(alpha, val);
    ans = max(ans, val);
    if (beta <= alpha) return ans;
    val = yo(i + 1, which ^ 1, total_balls + a[i], count_balls + 1, alpha, beta);
    alpha = max(alpha, val);
    ans = max(ans, val);
    return ans;
  }
  else {
    int ans = inf;
    int val = yo(i + 1, which ^ 1, total_balls, count_balls, alpha, beta);
    beta = min(beta, val);
    ans = min(ans, val);
    if (beta <= alpha) return ans;
    val = yo(i + 1, which ^ 1, total_balls + a[i], count_balls + 1, alpha, beta);
    beta = min(beta, val);
    ans = min(ans, val);
    return ans;
  }
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  fac[0] = 1;
  for (int i = 1; i < N; i++) {
    fac[i] = 1LL * fac[i - 1] * i % mod;
  }
  ifac[N - 1] = power(fac[N - 1], mod - 2);
  for (int i = N - 2; i >= 0; i--) {
    ifac[i] = 1LL * ifac[i + 1] * (i + 1) % mod;
  }
  int t, cs = 0; cin >> t;
  while (t--) {
    cin >> n >> target;
    for (int i = 1; i <= n; i++) {
      cin >> a[i];
    }
    cout << "Case " << ++cs << ": " << yo(1, 1, 0, 0, -inf, inf) << '\n';
  }
  return 0;
}
// https://algo.codemarshal.org/contests/ruet-2022/problems/C
// https://www.geeksforgeeks.org/minimax-algorithm-in-game-theory-set-4-alpha-beta-pruning/