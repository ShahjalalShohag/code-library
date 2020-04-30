#include<bits/stdc++.h>
using namespace std;

const int N = 405;

// Complexity: O(n^3) but slower when the graph is dense
// It finds maximum cost matching on a general graph, not necessarily with maximum matching
// 1-indexed
int p[N];
struct RandomizedMatching {
	long long G[N][N], dis[N];
	int match[N], stk[N], id[N], vis[N];
	int n, top;
	const long long inf = 1e18;
	RandomizedMatching() {}
	RandomizedMatching(int _n) {
		n = _n; top = 0;
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= n; j++) {
				G[i][j] = 0;
			}
		}
	}
	void add_edge(int u, int v, long long w) {
		assert(w > 0);
		G[u][v] = max(G[u][v], w);
		G[v][u] = max(G[v][u], w);
	}
	bool spfa(int u) {
		stk[top ++] = u;
		if (vis[u]) return true;
		vis[u] = true;
		for (int i = 1; i <= n; ++ i) {
			if (i != u && i != match[u] && !vis[i]) {
				int v = match[i];
				if (dis[v] < dis[u] + G[u][i] - G[i][v]) {
					dis[v] = dis[u] + G[u][i] - G[i][v];
					if (spfa(v)) return true;
				}
			}
		}
		top --; vis[u] = false;
		return false;
	}
	long long maximum_matching() {
		for (int i = 1; i <= n; ++ i) id[i] = i;
		for (int i = 1; i <= n; i += 2) match[i] = i + 1, match[i + 1] = i;
		for (int times = 0, flag; times < 3; ) { //increase the iteration value for higher probability
			memset(dis, 0, sizeof(dis));
			memset(vis, 0, sizeof(vis));
			top = 0; flag = 0;
			for (int i = 1; i <= n; ++ i) {
				if (spfa(id[i])) {
					flag = 1;
					int t = match[stk[top - 1]], j = top - 2;
					while (stk[j] != stk[top - 1]) {
						match[t] = stk[j];
						swap(t, match[stk[j]]);
						-- j;
					}
					match[t] = stk[j]; match[stk[j]] = t;
					break;
				}
			}
			if (!flag) times ++;
			if (!flag) random_shuffle(id + 1, id + n + 1);
		}
		long long ans = 0;
		for (int i = 1; i <= n; ++ i) {
			if (match[i] <= n && i < match[i]) {
				if (G[i][match[i]] != 0) ans += G[i][match[i]], p[i] = match[i], p[match[i]] = i;
			}
		}
		return ans;
	}
};
int32_t main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	int n, m; cin >> n >> m;
	RandomizedMatching M(n);
	for (int i = 1; i <= m; i++) {
		int u, v, w; cin >> u >> v >> w;
		M.add_edge(u, v, w);
	}
	cout << M.maximum_matching() << '\n';
	for (int i = 1; i <= n; i++) cout << p[i] << ' '; cout << '\n';
    return 0;
}