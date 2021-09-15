#include<bits/stdc++.h>
using namespace std;

#define ll long long
#define eb emplace_back
#define nl '\n'
#define deb(x) cerr << #x" = " << x << nl
#define in() ( { int a ; scanf("%d", &a); a; } )

const int N = 3e5 + 9;
const int mod = 1e9 + 7;

int pw[20][20], b[1 << 10][3000], cnt[1 << 10], dp[2][20][1 << 10][2520];
vector<int> v;
int yo(int i, int mask, int rem, int f) {
  if(i == -1) return pw[b[mask][rem]][cnt[mask]];
  int &ret = dp[f][i][mask][rem];
  if(ret != -1 && !f) return ret;
  int m = f ? v[i] : 9;
  int ans = 0;
  for(int k = 0; k <= m; k++) {
    ans += yo(i - 1, mask | (1 << k), (rem * 10 + k) % 2520, k == m ? f : 0);
    if(ans >= mod) ans -= mod;
  }
  if(!f) ret = ans;
  return ans;
}
int solve(ll n) {
  if(n == 0) return 0;
  v.clear();
  while(n) {
    v.eb(n % 10);
    n /= 10;
  }
  return yo(v.size() - 1, 0, 0, 1);
}
int32_t main() {
  memset(dp, -1, sizeof dp);
  for(int i = 1; i < 20; i++) {
    pw[i][0] = 1;
    for(int k = 1; k < 20; k++) pw[i][k] = pw[i][k - 1] * 1LL * i % mod;
  }
  for(int mask = 1; mask < (1 << 10); mask++) cnt[mask] = __builtin_popcount(mask) - (mask & 1);
  for(int mask = 1; mask < (1 << 10); mask++) {
    for(int rem = 0; rem < 2530; rem++) {
      for(int i = 1; i < 10; i++) if(mask >> i & 1) b[mask][rem] += rem % i == 0;
    }
  }
  int t;
  cin >> t;
  while(t--) {
    ll l, r;
    cin >> l >> r;
    cout << (solve(r) - solve(l - 1) + mod) % mod << nl;
  }
  return 0;
}
