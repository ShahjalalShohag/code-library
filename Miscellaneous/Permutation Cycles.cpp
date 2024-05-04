#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

// 0-indexed, values are also from 0 to n-1

// decompose into cycles
vector<vector<int>> decompose(vector<int> &p) {
  int n = p.size();
  vector<vector<int>> cycles;
  vector<bool> vis(n, 0);
  for (int i = 0; i < n; i++) {
    if (!vis[i]) {
      vector<int> v;
      while (!vis[i]) {
        v.push_back(i);
        vis[i] = 1;
        i = p[i];
      }
      cycles.push_back(v);
    }
  }
  return cycles;
}

// restore the permutation from the cycles
vector<int> restore(int n, vector<vector<int>> &cycles) {
  vector<int> p(n);
  for (auto v : cycles) {
    int m = v.size();
    for (int i = 0; i < m; i++) p[v[i]] = v[(i + 1) % m];
  }
  return p;
}

//cycle decomposition of the k-th power of p
vector<vector<int>> power(vector<int> &p, int k) {
  int n = p.size();
  auto cycles = decompose(p);
  vector<vector<int>> ans;
  for (auto v : cycles) {
    int len = v.size(), g = __gcd(k, len); // g cycles of len / g length
    for (int i = 0; i < g; i++) {
      vector<int> w;
      for (int j = i, cnt = 0; cnt < len / g; cnt++, j = (j + k) % len) {
        w.push_back(v[j]);
      }
      ans.push_back(w);
    }
  }
  return ans;
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n; cin >> n;
  vector<int> p(n);
  for (int i = 0; i < n; i++) {
    cin >> p[i]; --p[i];
  }
  auto cycles = decompose(p);
  int ans = 1;
  for (auto v: cycles) {
    int sz = v.size();
    ans = 1LL * ans * sz / __gcd(ans, sz);
  }
  cout << ans << '\n';
  return 0;
}
// https://acm.timus.ru/problem.aspx?space=1&num=1024
