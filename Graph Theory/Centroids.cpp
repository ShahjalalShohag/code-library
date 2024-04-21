#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;
vector<int> g[N];
int sz[N];
void dfs(int u, int p = 0) {
	sz[u] = 1;
	for (auto v: g[u]) {
		if (v ^ p) {
			dfs(v, u);
			sz[u] += sz[v];
		}
	}
}
vector<int> get_centroids(int n) {
	int u = 1;
	dfs(u);
	while (1) {
		int tmp = -1;
		for (auto v: g[u]) {
			if (sz[v] > sz[u]) continue;
			if (2 * sz[v] >= n) tmp = v;
		}
		if (tmp == -1) break;
		u = tmp;
	}
	dfs(u);
	for (auto v: g[u]) {
		if (2 * sz[v] == n) {
			return {u, v};
		}
	}
	return {u};
}
int32_t main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	int t; cin >> t;
	while (t--) {
		int n; cin >> n;
		for (int i = 1; i < n; i++) {
		    int u, v; cin >> u >> v;
		    g[u].push_back(v);
		    g[v].push_back(u);
		}
		auto v = get_centroids(n);
		for (auto i: v) cout << i << ' '; cout << '\n';
		for (int i = 1; i <= n; i++) g[i].clear();
	}
  return 0;
}
// similar problem: https://cses.fi/problemset/task/2079
