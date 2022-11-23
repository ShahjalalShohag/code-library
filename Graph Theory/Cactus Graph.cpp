#include<bits/stdc++.h>
using namespace std;

const int N = 2e5 + 9, LG = 18, mod = 998244353;

// no multi-edge or self-loops
// edge cactus: each edge is in at most one cycle
// this also works for vertex cactus
vector<array<int, 2>> g[N]; // (node, edge_id)
vector<int> ct[N]; // cactus tree
int P[N];
int vis[N];
int cyc, id, par_edge_id[N];
bool cycle_edge[N * 2]; // if the edge is in a cycle
void dfs(int u, int p = 0) {
  vis[u] = 1;
  P[u] = p;
  for (auto x: g[u]) {
    int v = x[0], e_id = x[1];
    if (v == p) continue;
    else if (!vis[v]) {
      par_edge_id[v] = e_id;
      dfs(v, u);
    }
    else if (vis[v] == 1) {
      ++cyc;
      // add an edge from the cycle to the nodes of the cycle
      int cur = u, edge = par_edge_id[u];
      while (cur != v) {
        ct[cur].push_back(cyc);
        ct[cyc].push_back(cur);
        cycle_edge[edge] = true;
        cur = P[cur];
        edge = par_edge_id[cur];
      }
      ct[v].push_back(cyc);
      ct[cyc].push_back(v);
      cycle_edge[e_id] = true;
    }
  }
  vis[u] = 2;
}
int par[N][LG + 1], dep[N], cnt[N], c, comp[N];
void dfs2(int u, int p = 0) {
  par[u][0] = p;
  dep[u] = dep[p] + 1;
  cnt[u] = cnt[p] + (u > id);
  comp[u] = c;
  for (int i = 1; i <= LG; i++) par[u][i] = par[par[u][i - 1]][i - 1];
  for (auto v: ct[u]) if (v != p) {
    dfs2(v, u);
  }
}
int lca(int u, int v) {
  if (dep[u] < dep[v]) swap(u, v);
  for (int k = LG; k >= 0; k--) if (dep[par[u][k]] >= dep[v]) u = par[u][k];
  if (u == v) return u;
  for (int k = LG; k >= 0; k--) if (par[u][k] != par[v][k]) u = par[u][k], v = par[v][k];
  return par[u][0];
}


int dp[N], up[N], done[N];
void dfs3(int u, int p = 0) {
  done[u] = 1;
  dp[u] = u <= id;
  for (auto v: ct[u]) {
    if (v ^ p) {
      dfs3(v, u);
      if (u > id) {
        dp[u] += dp[v] * 2 % mod;
      }
      else {
        dp[u] += dp[v];
      }
      dp[u] %= mod;
    }
  }
}

void dfs4(int u, int p = 0) {
  int sum = 0;
  for (auto v: ct[u]) {
    if (v ^ p) {
      sum += dp[v];
      sum %= mod;
    }
  }
  for (auto v: ct[u]) {
    if (v ^ p) {
      int w = (up[u] + (sum - dp[v] + mod) % mod) % mod;
      up[v] = u <= id;
      up[v] += u > id ? w * 2 % mod : w;
      up[v] %= mod;
      dfs4(v, u);
    }
  }
}
int pw[N], f[N];
vector<array<int, 2>> e[N];
int mp[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  pw[0] = 1;
  for (int i = 1; i < N; i++) {
    pw[i] = pw[i - 1] * 2 % mod;
  }
  int t; cin >> t;
  while (t--) {
    int n, m, x; cin >> n >> m >> x;
    while (m--) {
      int u, v, w; cin >> u >> v >> w;
      e[w].push_back({u, v});
    }
    int ans = 0;
    for (int d = x; d >= 1; d--) {
      id = 0;
      for (int mul = d; mul <= x; mul += d) {
        for (auto x: e[mul]) {
          int u = x[0], v = x[1];
          if (!mp[u]) mp[u] = ++id;
          if (!mp[v]) mp[v] = ++id;
        }
      }
      // build the graph
      int e_id = 0;
      for (int mul = d; mul <= x; mul += d) {
        for (auto x: e[mul]) {
          int u = x[0], v = x[1];
          g[mp[u]].push_back({mp[v], ++e_id});
          g[mp[v]].push_back({mp[u], e_id});
        }
      }
      // build cactus tree
      cyc = id;
      c = 0;
      for (int u = 1; u <= id; u++) {
        if (!vis[u]) {
          ++c;
          dfs(u);
        }
      }
      for (int u = 1; u <= id; u++) {
        for (auto x: g[u]) {
          int v = x[0], e_id = x[1];
          if (!cycle_edge[e_id]) {
            ct[u].push_back(v);
          }
        }
      }
      // cactus tree built!

      // for (int u = 1; u <= cyc; u++) {
      //   if (!dep[u]) {
      //     dfs2(u);
      //   }
      // }
      // int brute_res = 0;
      // for (int u = 1; u <= id; u++) {
      //   for (int v = u + 1; v <= id; v++) {
      //     if (comp[u] != comp[v]) continue;
      //     int l = lca(u, v);
      //     int d = cnt[u] + cnt[v] - cnt[l] - cnt[par[l][0]];
      //     brute_res += pw[d];
      //     brute_res %= mod;
      //   }
      // }

      // find the sum of the number of simple paths from u to v over all 1 <= u < v <= nodes
      // do up-down dp
      for (int u = 1; u <= id; u++) {
        if (done[u]) continue;
        dfs3(u);
        dfs4(u);
      }
      int fast_res = 0;
      for (int u = 1; u <= id; u++) {
        fast_res += (dp[u] + up[u]) % mod;
        fast_res %= mod;
      }
      fast_res -= id;
      fast_res = (fast_res + mod) % mod;
      fast_res = 1LL * fast_res * ((mod + 1) / 2) % mod;
      f[d] = fast_res;
      for (int mul = d + d; mul <= x; mul += d) {
        f[d] = (f[d] - f[mul] + mod) % mod;
      }
      for (int u = 1; u <= cyc; u++) {
        g[u].clear();
        ct[u].clear();
        vis[u] = cnt[u] = dep[u] = comp[u] = done[u] = dp[u] = up[u] = 0;
      }
      for (int i = 1; i <= e_id; i++) {
        cycle_edge[i] = false;
      }
      for (int mul = d; mul <= x; mul += d) {
        for (auto x: e[mul]) {
          int u = x[0], v = x[1];
          mp[u] = mp[v] = 0;
        }
      }
    }
    for (int d = 1; d <= x; d++) {
      ans ^= f[d];
      e[d].clear();
    }
    cout << ans << '\n';
  }
  return 0;
}
// https://vjudge.net/problem/HDU-7143
