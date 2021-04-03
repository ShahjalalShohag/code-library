#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;

struct Basis {
  static const int B = 30;
  int a[B];
  Basis() {
    memset(a, 0, sizeof a);
  }
  void insert(int x){
    for (int i = B - 1; i >= 0; i--) {
      if (x >> i & 1) {
        if (a[i]) x ^= a[i];
        else {
          a[i] = x;
          break;
        }
      }
    }
  }
  int max_xor(int ans = 0) {
    for(int i = B - 1; i >= 0; i--) {
      ans = max(ans, ans ^ a[i]);
    }
    return ans;
  }
  // https://en.wikipedia.org/wiki/Row_echelon_form#Reduced_row_echelon_form
  void reduced_row_echelon_form() {
    for (int i = 0; i < B; i++) {
      if (!a[i]) continue;
      for (int j = i + 1; j < B; j++) {
        if (a[j] >> i & 1) a[j] ^= a[i];
      }
    }
  }
  // max xor after trasforming into reduced row echelon form
  int max_xor2(int x = 0) {
    int ans = 0;
    for (int i = B - 1; i >= 0; i--) {
      if (~x >> i & 1) {
        ans ^= a[i];
      }
    }
    return ans;
  }
}t;
vector<pair<int, int>> g[N];
bool vis[N];
int a[N], p[N];
void dfs(int u) {
  vis[u] = true;
  for (auto [v, w]: g[u]) {
    if (!vis[v]) {
      a[v] = a[u] ^ w;
      dfs(v);
    }
    else {
      t.insert(a[u] ^ a[v] ^ w);
    }
  }
}
int cnt[30][N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m, q; cin >> n >> m >> q;
  for (int i = 1; i <= m; i++) {
    int u, v, w; cin >> u >> v >> w;
    g[u].push_back({v, w});
    g[v].push_back({u, w});
  }
  dfs(1);
  t.reduced_row_echelon_form();
  for (int i = 1; i <= n; i++) {
    p[i] = p[i - 1] ^ a[i];
    for (int k = 0; k < 30; k++) {
      cnt[k][i] = cnt[k][i - 1] + (a[i] >> k & 1);
    }
  }
  while (q--) {
    int l, r; cin >> l >> r;
    int ans = (r - l) % 2 * (p[r] ^ p[l - 1]);
    for (int k = 0; k < 30; k++) {
      if (t.a[k]) {
        int x = cnt[k][r] - cnt[k][l - 1], y = r - l + 1 - x;
        if ((1LL * x * (x - 1) / 2 + 1LL * y * (y - 1) / 2) & 1) {
          ans ^= t.a[k];
        }
      }
    }
    cout << ans << '\n';
  }
  return 0;
}
// https://codeforces.com/gym/102979/problem/F