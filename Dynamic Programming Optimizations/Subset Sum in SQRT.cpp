#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;

// if sum of the array values = x, then complexity is O(x sqrt(x))
// returns the subset ids
vector<int> subset_sum(vector<int> a, int s) {
  vector<int> cnt(s + 1, 0), dp(s + 1, 0), last(s + 1, 0);
  map<int, vector<int>> id;
  for (int i = 0; i < a.size(); i++) {
    if (a[i] <= s) {
      id[a[i]].push_back(i);
      cnt[a[i]]++;
    }
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
          last[k] = id[i][c];
        }
      }
    }
  }
  if (dp[s]) {
    vector<int> ans;
    while (s) {
      ans.push_back(last[s]);
      s -= a[last[s]];
    }
    return ans;
  }
  else {
    return {-1};
  }
}
int ans[N], dep[N], sz[N];
vector<int> g[N], id[N];
void dfs(int u, int p = 0) {
  dep[u] = dep[p] + 1;
  id[dep[u]].push_back(u);
  sz[u] = 1;
  for (auto v: g[u]) {
    if (v ^ p) {
      dfs(v, u);
      sz[u] += sz[v];
    }
  }
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, k; cin >> n >> k;
  for (int i = 2; i <= n; i++) {
    int k; cin >> k;
    g[k].push_back(i);
  }
  dfs(1);
  vector<int> a;
  int mx = 0;
  for (int i = 1; i <= n; i++) {
    mx = max(mx, dep[i]);
  }
  for (int i = 1; i <= mx; i++) {
    a.push_back(id[i].size());
  }
  auto v = subset_sum(a, k);
  if (!v.empty() && v.front() == -1) {
    cout << mx + 1 << '\n';
    pair<int, int> x = {k, 1}, y = {n - k, 0};
    for (int i = 1; i <= mx; i++) {
      if (x < y) {
        swap(x, y);
      }
      sort(id[i].begin(), id[i].end(), [&](int u, int v) {
        return sz[u] < sz[v];
      });
      while (!id[i].empty() && x.first) {
        x.first--;
        ans[id[i].back()] = x.second;
        id[i].pop_back();
      }
      while (!id[i].empty() && y.first) {
        y.first--;
        ans[id[i].back()] = y.second;
        id[i].pop_back();
      }
    }
  }
  else {
    cout << mx << '\n';
    for (auto x: v) {
      for (auto i: id[x + 1]) {
        ans[i] = 1;
      }
    }
  }
  for (int i = 1; i <= n; i++) {
    cout << (ans[i] ? 'a' : 'b');
  }
  return 0;
}
// https://codeforces.com/problemset/problem/1481/F