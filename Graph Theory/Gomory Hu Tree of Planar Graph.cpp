#include <bits/stdc++.h>
using namespace std;
const int N = 200005;
const int mod = 998244353;
struct EDGE {
  int from, to, rev;
  long long w;
} E[N * 4];
int n, m, bel[N * 4];
bool vis[N * 4];
vector<int> G[N];
vector<vector<int> > plr;
set<pair<long long, int> > st;
vector<pair<pair<int, int>, long long> > tr;
int fa[N], sz[N];
int Find(int x) {
  return x == fa[x] ? x : fa[x] = Find(fa[x]);
}
// credit: Misaka-Mikoto-
int main() {
  scanf("%d%d", &n, &m);
  for (int i = 0; i < m; i++) {
    scanf("%d%d%lld", &E[i * 2].from, &E[i * 2].to, &E[i * 2].w);
    E[i * 2].from--, E[i * 2].to--;
    E[i * 2 + 1] = E[i * 2];
    swap(E[i * 2 + 1].from, E[i * 2 + 1].to);
  }
  for (int i = 0; i < m * 2; i++) {
    G[E[i].from].push_back(i);
    if ((E[i].from + 1) % n == E[i].to) vis[i] = true;
    else vis[i] = false;
  }
  for (int i = 0; i < n; i++) {
    sort(G[i].begin(), G[i].end(), [&](int e1, int e2) {
      return (E[e1].to - i + n) % n < (E[e2].to - i + n) % n;
    });
    for (int j = 0; j < (int)G[i].size(); j++) {
      int e = G[i][j];
      E[e ^ 1].rev = j;
    }
  }
  for (int i = 0; i < m * 2; i++) if (!vis[i]) {
    int e = i;
    vector<int> ve;
    while (!vis[e]) {
      ve.push_back(e);
      vis[e] = true;
      int v = E[e].to;
      int ne = G[v][(E[e].rev + 1) % G[v].size()];
      e = ne;
    }
    plr.push_back(ve);
    for (int x : ve) bel[x] = plr.size() - 1;
  }
  for (int i = 0; i < m * 2; i++) {
    if ((E[i].from + 1) % n == E[i].to) vis[i] = true;
    else vis[i] = false;
  }
  for (int i = 0; i < m; i++) {
    int x = E[i * 2].from, y = E[i * 2].to;
    if ((x + 1) % n == y || (y + 1) % n == x) {
      st.insert(make_pair(E[i * 2].w, i));
    }
  }
  while (!st.empty()) {
    int e = st.begin() -> second;
    st.erase(st.begin());
    if (vis[e * 2] && vis[e * 2 + 1]) {
      tr.push_back(make_pair(make_pair(E[e * 2].from, E[e * 2].to), E[e * 2].w));
      continue;
    }
    if (!vis[e * 2]) e *= 2;
    else e = e * 2 + 1;
    int b = bel[e];
    for (int x : plr[b]) {
      vis[x] = true;
      if (x == e) continue;
      st.erase(make_pair(E[x].w, x >> 1));
      E[x].w += E[e].w;
      E[x ^ 1].w += E[e].w;
      st.insert(make_pair(E[x].w, x >> 1));
    }
  }
  reverse(tr.begin(), tr.end()); // edges of GHT
  int ans = 0;
  for (int i = 0; i < n; i++) fa[i] = i, sz[i] = 1;
  for (int i = 0; i < n - 1; i++) {
    int x = Find(tr[i].first.first), y = Find(tr[i].first.second), w = tr[i].second % mod;
    ans = (ans + 1ll * sz[x] * sz[y] % mod * w) % mod;
    if (sz[x] < sz[y]) swap(x, y);
    fa[y] = x;
    sz[x] += sz[y];
  }
  printf("%d\n", ans);
  return 0;
}
// https://codeforces.com/gym/102471/submission/71744464