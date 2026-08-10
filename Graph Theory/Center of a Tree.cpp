// Finds the center(s) of a tree (1 or 2 nodes).
// The center(s) are the node(s) that minimize the height of the tree when rooted there.
// Idea: repeatedly peel off leaves (degree 1 nodes) layer by layer until at most 2 nodes remain.
// Those remaining nodes are the center(s).
// 1-indexed.
// Time: O(n), Space: O(n)
#include <bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;
vector<int> g[N];
int deg[N];

vector<int> centers(int n) {
  if (n == 1) return {1};
  queue<int> leaves;
  for (int i = 1; i <= n; i++) {
    if (deg[i] == 1) leaves.push(i);
  }
  int remaining = n;
  while (remaining > 2) {
    int sz = leaves.size();
    remaining -= sz;
    for (int i = 0; i < sz; i++) {
      int u = leaves.front(); leaves.pop();
      for (auto v : g[u]) {
        if (--deg[v] == 1) leaves.push(v);
      }
    }
  }
  vector<int> res;
  while (!leaves.empty()) {
    res.push_back(leaves.front());
    leaves.pop();
  }
  return res;
}

int32_t main() {
  int n; 
  cin >> n;
  for (int i = 1; i < n; i++) {
    int u, v; cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
    deg[u]++;
    deg[v]++;
  }
  auto res = centers(n);
  for (auto x : res) cout << x << ' ';
  cout << '\n';
  return 0;
}
