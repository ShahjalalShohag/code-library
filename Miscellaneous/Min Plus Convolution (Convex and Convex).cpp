#include<bits/stdc++.h>
using namespace std;

// a[i + 1] - a[i] >= a[i] - a[i - 1] -> convex
// b[i + 1] - b[i] >= b[i] - b[i - 1] -> convex
// compute ans(i + j) = min(a(i) + b(j))
vector<int> min_plus_convolution(vector<int> a, vector<int> b) {
  int n = a.size() - 1, m = b.size() - 1;
  vector<int> ans(n + m + 1);
  int sum = a[0] + b[0]; ans[0] = sum;
  int l = 0, r = 0;
  while (l < n && r < m) {
    if (a[l + 1] - a[l] < b[r + 1] - b[r]) {
      sum += a[l + 1] - a[l];
      l++;
    } else {
      sum += b[r + 1] - b[r];
      r++;
    }
    ans[l + r] = sum;
  }
  while (l < n) sum += a[l + 1] - a[l], l++, ans[l + r] = sum;
  while (r < m) sum += b[r + 1] - b[r], r++, ans[l + r] = sum;
  return ans;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m; cin >> n >> m;
  vector<int> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  vector<int> b(m);
  for (int i = 0; i < m; i++) {
    cin >> b[i];
  }
  auto ans = min_plus_convolution(a, b);
  for (int i = 0; i < ans.size(); i++) {
    cout << ans[i] << ' ';
  }
  return 0;
}
// https://judge.yosupo.jp/problem/min_plus_convolution_convex_convex
