#include<bits/stdc++.h>
using namespace std;

const int N = 105;

// Complexity: O(n^3) but slower when the graph is dense
// It finds maximum cost matching on a general graph, not necessarily with maximum matching
// for mincost set initial weights to -inf and do the same on the 72nd line too
// and add -weight and return -maxmatch()
// 1-indexed
struct RandomizedMatching {
  long long G[N][N], dis[N];
  int match[N];
  int mat[N], stk[N], id[N], vis[N];
  int n, top;
  const long long inf = 1e18;
  RandomizedMatching() {}
  RandomizedMatching(int _n) {
    n = _n;
    top = 0;
    memset(match, 0, sizeof match);
    for (int i = 1; i <= n + 1; i++) {
      for (int j = 1; j <= n + 1; j++) {
        G[i][j] = -inf;
      }
    }
  }
  void add_edge(int u, int v, long long w) {
    G[u][v] = max(G[u][v], w);
    G[v][u] = max(G[v][u], w);
  }
  bool spfa(int u) {
    stk[top ++] = u;
    if (vis[u]) return true;
    vis[u] = true;
    for (int i = 1; i <= n; ++ i) {
      if (i != u && i != mat[u] && !vis[i]) {
        int v = mat[i];
        if (dis[v] < dis[u] + G[u][i] - G[i][v]) {
          dis[v] = dis[u] + G[u][i] - G[i][v];
          if (spfa(v)) return true;
        }
      }
    }
    top --;
    vis[u] = false;
    return false;
  }
  long long maximum_matching() {
    for (int i = 1; i <= n; ++ i) id[i] = i;
    for (int i = 1; i <= n; i += 2) mat[i] = i + 1, mat[i + 1] = i;
    for (int times = 0, flag; times < 3; ) { //increase the iteration value for higher probability
      memset(dis, 0, sizeof(dis));
      memset(vis, 0, sizeof(vis));
      top = 0;
      flag = 0;
      for (int i = 1; i <= n; ++ i) {
        if (spfa(id[i])) {
          flag = 1;
          int t = mat[stk[top - 1]], j = top - 2;
          while (stk[j] != stk[top - 1]) {
            mat[t] = stk[j];
            swap(t, mat[stk[j]]);
            -- j;
          }
          mat[t] = stk[j];
          mat[stk[j]] = t;
          break;
        }
      }
      if (!flag) times ++;
      if (!flag) random_shuffle(id + 1, id + n + 1);
    }
    long long ans = 0;
    for (int i = 1; i <= n; ++ i) {
      if (mat[i] <= n && i < mat[i]) {
        if (G[i][mat[i]] != -inf) ans += G[i][mat[i]], match[i] = mat[i], match[mat[i]] = i;
      }
    }
    return ans;
  }
};
long long w[N][N];
int deg[N];
const long long inf = 1e18;
//if the graph is not connected then answer is not possible
int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m;
  while (cin >> n && n) {
    cin >> m;
    for (int i = 1; i <= n; i++) for (int j = 1; j <= n; j++) w[i][j] = inf;
    long long ans = 0;
    memset(deg, 0, sizeof deg);
    for (int i = 1; i <= m; i++) {
      int u, v;
      long long we;
      cin >> u >> v >> we;
      w[u][v] = min(w[u][v], we);
      w[v][u] = min(w[v][u], we);
      ans += we;
      deg[u]++;
      deg[v]++;
    }
    vector<int> odd;
    for (int k = 1; k <= n; k++) {
      for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) w[i][j] = min(w[i][j], w[i][k] + w[k][j]);
      }
    }
    for (int i = 1; i <= n; i++) if (deg[i] & 1) odd.push_back(i);
    int k = odd.size();
    assert(k % 2 == 0); //there are even number of vertices having odd degree
    RandomizedMatching M(k);
    for (int i = 1; i <= k; i++) {
      for (int j = 1; j <= k; j++) {
        if (i == j || w[odd[i - 1]][odd[j - 1]] == inf) M.add_edge(i, j, -M.inf);
        else M.add_edge(i, j, -w[odd[i - 1]][odd[j - 1]]);
      }
    }
    ans += -M.maximum_matching();
    cout << ans << '\n';
  }
  return 0;
}
//https://vjudge.net/problem/UVA-10296
