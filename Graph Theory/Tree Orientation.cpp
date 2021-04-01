
#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

vector<int> g[N];
int sz[N];
int tot, done[N];
void calc_sz(int u, int p) {
  tot ++; sz[u] = 1;
  for (auto v: g[u]) {
    if(v == p || done[v]) continue;
    calc_sz(v, u);
    sz[u] += sz[v];
  }
}
int find_cen(int u, int p) {
  for (auto v: g[u]) {
    if(v == p || done[v]) continue;
    else if(sz[v] > tot / 2) return find_cen(v, u);
  }
  return u;
}

// if sum of the array values = x, then complexity is O(x sqrt(x))
vector<int> subset_sum(vector<int> a) {
  int s = 0;
  for (auto x: a) {
    s += x;
  }
  vector<int> cnt(s + 1, 0), dp(s + 1, 0);
  for (int i = 0; i < a.size(); i++) {
    cnt[a[i]]++;
  }
  dp[0] = 1;
  for (int i = 1; i <= s; i++) {
    if (!cnt[i]) {
      continue;
    }
    for (int j = 0; j < i; j++) {
      int c = 0;
      for (int k = j; k <= s; k += i) {
        if (dp[k]) {
          c = cnt[i];
        }
        else if (c) {
          dp[k] = 1;
          c--;
        }
      }
    }
  }
  return dp;
}

long long cur;
void dfs(int u, int p = 0) {
  sz[u] = 1;
  for (auto v: g[u]) {
    if (v ^ p) {
      dfs(v, u);
      sz[u] += sz[v];
    }
  }
  cur += sz[u] - 1;
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n; cin >> n;
  for (int i = 1; i < n; i++) {
    int u, v; cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  calc_sz(1, 0);
  int r = find_cen(1, 0);
  dfs(r);
  vector<int> v;
  long long ans = 0;
  for (auto u: g[r]) {
    v.push_back(sz[u]);
  }
  auto dp = subset_sum(v);
  for (int i = 1; i <= n - 1; i++) {
    if (dp[i]) {
      ans = max(ans, cur + 1LL * i * (n - 1 - i));
    }
  }
  cout << n - 1 << ' ' << ans << '\n';
  return 0;
}
// https://szkopul.edu.pl/problemset/problem/3bBT-3VuSu78UsxTQSwaJzVo/site/?key=statement
// https://codeforces.com/blog/entry/89228
