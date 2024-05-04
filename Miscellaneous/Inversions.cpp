#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;

template <typename T> using o_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t; cin >> t;
  while (t--) {
    int n; cin >> n;
    int a[n + 1];
    for (int i = 1; i <= n; i++) {
      cin >> a[i];
    }
    o_set<int> se;
    long long ans = 0;
    for (int i = n; i >= 1; i--) {
      ans += se.order_of_key(a[i]);
      se.insert(a[i]);
    }
    cout << ans << '\n';
  }
  return 0;
}
// https://vjudge.net/problem/SPOJ-INVCNT
