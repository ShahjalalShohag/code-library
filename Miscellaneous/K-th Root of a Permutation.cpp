#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

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
    int len = v.size(), g = __gcd(k, len); //g cycles of len / g length
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
//cycle decomposition of the k-th root of p with minimum disjoint cycles
//if toggle = 1, then the parity of number of cycles will be different from the other(toggle = 0) if possible
//returns empty vector if no solution exists
vector<vector<int>> root(vector<int> &p, int k, int toggle = 0) {
  int n = p.size();
  vector<vector<int>> cycles[n + 1];
  auto d = decompose(p);
  for (auto v : d) cycles[(int)v.size()].push_back(v);
  vector<vector<int>> ans;
  for (int len = 1; len <= n; len++) {
    if (cycles[len].empty()) continue;
    int tmp = k, t = 1, x = __gcd(len, tmp);
    while (x != 1) {
      t *= x;
      tmp /= x;
      x = __gcd(len, tmp);
    }
    if ((int)cycles[len].size() % t != 0) {
      ans.clear();
      return ans; //no solution exists
    }
    int id = 0;
    //we can merge t * z cycles iff tmp % z === 0
    if (toggle && tmp % 2 == 0 && (int)cycles[len].size() >= 2 * t) {
      int m = 2 * t * len;
      vector<int> cycle(m);
      for (int x = 0; x < 2 * t; x++) { //merging 2t cycles to perform the toggle
        for (int y = 0; y < len; y++) {
          cycle[(x + 1LL * y * k) % m] = cycles[len][x][y];
        }
      }
      ans.push_back(cycle);
      id = 2 * t;
      toggle = 0;
    }
    for (int i = id; i < (int)cycles[len].size(); i += t) {
      int m = t * len;
      vector<int> cycle(m);
      for (int x = 0; x < t; x++) { //merging t cycles
        for (int y = 0; y < len; y++) {
          cycle[(x + 1LL * y * k) % m] = cycles[len][i + x][y];
        }
      }
      ans.push_back(cycle);
    }
  }
  return ans;
}
//minimum swaps to obtain this perm from unit perm
vector<pair<int, int>> transpositions(vector<vector<int>> &cycles) {
  vector<pair<int, int>> ans;
  for (auto v : cycles) {
    int m = v.size();
    for (int i = m - 1; i > 0; i--) ans.push_back({v[0], v[i]});
  }
  return ans;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, l, k;
  cin >> n >> l >> k;
  vector<int> p(n);
  for (auto &x : p) cin >> x, --x;
  auto a = root(p, k);
  if (a.empty()) return cout << "no solution\n", 0;
  auto t = transpositions(a);
  if (t.size() % 2 != l % 2) {
    a = root(p, k, 1);
    t = transpositions(a);
  }
  if (t.size() % 2 != l % 2 || t.size() > l) return cout << "no solution\n", 0;
  auto z = restore(n, a);
  auto w = power(z, k);
  auto x = restore(n, w);
  assert(p == x);
  for (auto x : t) cout << x.first + 1 << ' ' << x.second + 1 << '\n';
  for (int i = t.size(); i < l; i++) cout << 1 << ' ' << 2 << '\n';
  return 0;
}
//https://www.hackerrank.com/contests/infinitum14/challenges/sasha-and-swaps/problem
