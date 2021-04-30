#include<bits/stdc++.h>
using namespace std;

using ll = long long;
const ll inf = 1e18;

struct LDE { // sum(a[i] * x[i]) = k, x[i] >= 0
  int n, x;
  vector<ll> d;
  LDE() {}
  LDE(vector<int> a) { // O(min(a[i]) * n * log(min(a[i])))
    n = a.size();
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> q;
    x = *min_element(a.begin(), a.end());
    assert(x > 0);
    d.resize(x);
    fill(d.begin(), d.end(), inf);
    d[0] = 0; q.push(pair<ll, int>(0, 0));
    while (!q.empty()) {
      pair<ll, int> nw = q.top(); q.pop();
      int u = nw.second;
      ll val = nw.first;
      if (d[u] != val) continue;
      for (int i = 0; i < n; ++ i) {
        ll tmp = val + a[i];
        int v = tmp % x;
        if (d[v] > tmp) {
          d[v] = tmp;
          q.push(pair<ll, int>(tmp, v));
        }
      }
    }
  }
  bool can(ll k) { // if a solution exists
    return d[k % x] <= k;
  }
  ll count(ll l, ll r) { // count of l <= k <= r s.t. solution for k exists
    ll ans = 0;
    for (int i = 0; i < x; i++) { // d[i], d[i] + x, d[i] + 2 * x, ... are achievable
      ans += d[i] <= r ? (r - d[i]) / x + 1 : 0;
      ans -= d[i] <= l - 1 ? (l - 1 - d[i]) / x + 1: 0;
    }
    return ans;
  }
};
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n; ll l, r; cin >> n >> l >> r;
  vector<int> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  LDE t(a);
  cout << t.count(l, r) << '\n';
  return 0;
}
// https://www.lydsy.com/JudgeOnline/problem.php?id=2118
// https://codeforces.com/blog/entry/71230?#comment-556761
