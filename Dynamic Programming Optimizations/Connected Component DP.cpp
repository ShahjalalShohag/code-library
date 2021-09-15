#include<bits/stdc++.h>
using namespace std;

const int N = 105, mod = 1e9 + 7;

int n, k, a[N], dp[N][N][1005][3];
int yo(int i, int c, int sum, int ends) { //(id, components, sum, borders)
  if (ends > 2 || sum > k)return 0;
  if (c == 0 && i > 1) return 0;
  if (i == n + 1) return ends == 2 && c == 1;
  int &ret = dp[i][c][sum][ends];
  if (ret != -1) return ret;
  int nsum = sum + (a[i] - a[i - 1]) * (2 * c - ends);
  long long ans = 0;
  if (c >= 2) ans += 1LL * (c - 1) * yo(i + 1, c - 1, nsum, ends);  //merge two components
  if (c >= 1) ans += 1LL * (2 * c - ends) * yo(i + 1, c, nsum, ends);  //add to a component's end
  ans += 1LL * (c + 1 - ends) * yo(i + 1, c + 1, nsum, ends);  //create a new component
  if (ends < 2) ans += 1LL * (2 - ends) * yo(i + 1, c + 1, nsum, ends + 1);  //create a new end
  if (ends < 2) ans += 1LL * (2 - ends) * yo(i + 1, c, nsum, ends + 1);  //extend a component to make it a border
  ans %= mod;
  return ret = ans;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  cin >> n >> k;
  for (int i = 1; i <= n; i++) cin >> a[i];
  sort(a + 1, a + n + 1);
  if (n == 1) return cout << 1 << '\n', 0;
  memset(dp, -1, sizeof dp);
  cout << yo(1, 0, 0, 0) << '\n';
  return 0;
}
// https://oj.uz/problem/view/JOI16_skyscraper
