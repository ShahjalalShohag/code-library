#include<bits/stdc++.h>
using namespace std;

const int N = 2e5 + 9;
using ll = long long;
const ll inf = 1e18;
using T = pair<ll, int>;

vector<array<int, 3>> g[N];
ll d[N], rd[N], f[N];
vector<array<int, 2>> S[N], R[N]; // shortest path graph, reverse shortest path graph
bool sp_edge[N]; // if edge is in the main shortest path
int path_id[N], l[N], r[N], u[N], v[N], w[N], edge[N];
vector<T> event[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m, Q; cin >> n >> m >> Q;
  for (int i = 1; i <= m; i++) {
    cin >> u[i] >> v[i] >> w[i];
    g[u[i]].push_back({v[i], w[i], i});
    g[v[i]].push_back({u[i], w[i], i});
  }
  priority_queue<T, vector<T>, greater<T>> q;
  for (int i = 1; i <= n; i++) {
    d[i] = inf;
  }
  d[1] = 0;
  q.push({0, 1});
  while (!q.empty()) {
    auto [x, u] = q.top();
    q.pop();
    for (auto [v, w, i]: g[u]) {
      if (d[u] + w < d[v]) {
        d[v] = d[u] + w;
        edge[v] = i;
        q.push({d[v], v});
      }
    }
  }
  q = priority_queue<T, vector<T>, greater<T>>();
  for (int i = 1; i <= n; i++) {
    rd[i] = inf;
  }
  rd[n] = 0;
  q.push({0, n});
  while (!q.empty()) {
    auto [x, u] = q.top();
    q.pop();
    for (auto [v, w, i]: g[u]) {
      if (rd[u] + w < rd[v]) {
        rd[v] = rd[u] + w;
        q.push({rd[v], v});
      }
    }
  }
  for (int u = 1; u <= n; u++) {
    for (auto [v, w, i]: g[u]) {
      if (d[u] + w == d[v]) {
        S[u].push_back({v, i});
      }
    }
    for (auto [v, w, i]: g[u]) {
      if (rd[u] + w == rd[v]) {
        R[u].push_back({v, i});
      }
    }
  }
  int cur = n;
  vector<int> path;
  while (cur != 1) {
    int i = edge[cur];
    path.push_back(i);
    sp_edge[i] = true;
    int up = cur ^ u[i] ^ v[i];
    cur = up;
  }
  reverse(path.begin(), path.end());
  for (int i = 0; i < path.size(); i++) {
    path_id[path[i]] = i;
  }

  vector<T> vec;
  for (int i = 1; i <= n; i++) {
    vec.push_back({d[i], i});
  }
  sort(vec.begin(), vec.end());
  for (int i = 1; i <= n; i++) {
    l[i] = path.size();
  }
  l[1] = -1;
  for (auto [_, u]: vec) {
    for (auto [v, i]: S[u]) {
      if (sp_edge[i]) {
        l[v] = min(l[v], path_id[i]);
      }
      else {
        l[v] = min(l[v], l[u]);
      }
    }
  }

  vec.clear();
  for (int i = 1; i <= n; i++) {
    vec.push_back({rd[i], i});
  }
  sort(vec.begin(), vec.end());
  for (int i = 1; i <= n; i++) {
    r[i] = -1;
  }
  r[n] = path.size();
  for (auto [_, u]: vec) {
    for (auto [v, i]: R[u]) {
      if (sp_edge[i]) {
        r[v] = max(r[v], path_id[i]);
      }
      else {
        r[v] = max(r[v], r[u]);
      }
    }
  }

  for (int u = 1; u <= n; u++) {
    for (auto [v, w, _]: g[u]) {
      if (sp_edge[_]) continue;
      ll cur = d[u] + rd[v] + w;
      if (l[u] + 1 <= r[v] - 1) {
        event[l[u] + 1].push_back({cur, +1});
        event[r[v] - 1 + 1].push_back({cur, -1});
      }
    }
  }
  multiset<ll> se;
  for (int i = 0; i < path.size(); i++) {
    for (auto [cur, add]: event[i]) {
      if (add == 1) {
        se.insert(cur);
      }
      else {
        se.erase(se.find(cur));
      }
    }
    if (se.empty()) {
      f[i] = inf;
    }
    else {
      f[i] = *se.begin();
    }
  }

  while (Q--) {
    int i, x; cin >> i >> x;

    ll ans = d[u[i]] + rd[v[i]] + x;
    ans = min(ans, d[v[i]] + rd[u[i]] + x);

    // shortest path from 1 to n that doesn't pass through edge i
    ll except_i = inf;
    if (sp_edge[i]) {
      // for (int u = 1; u <= n; u++) {
      //   for (auto [v, w, _]: g[u]) {
      //     if (!sp_edge[_] and l[u] < path_id[i] and path_id[i] < r[v]) {
      //       except_i = min(except_i, d[u] + rd[v] + w);
      //     }
      //   }
      // }
      except_i = f[path_id[i]];
    }
    else {
      except_i = min(except_i, d[n]);
    }
    ans = min(ans, except_i);
    cout << ans << '\n';
  }
  return 0;
}
// https://codeforces.com/contest/1163/problem/F