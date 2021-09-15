#include<bits/stdc++.h>
using namespace std;

const int N = 1e4 + 9;

/*Let w(i,j) be the cost function.
The required condition is the quadrangle inequality: for all i<=j, w(i,j)+w(i+1,j+1)<=w(i+1,j)+w(i,j+1)
From a practical point of view, we don't need to prove that w satisfies such an inequality,
it's easier to just list out a few values of w and check.
The normal form of DP is: dp[x]=min(dp[i]+w(i,x)) for all i from 0 to x-1.
Let k(x) be the optimal index for dp(x) i.e. dp(x) is minimum for index k(x) i.e. dp(x)=dp(k(x))+w(k(x),x)
Then for all i<=j, k(i)<=k(j) because of the quadrangle inequality
If we can't use CHT for this kind of DP optimization, GENERALLY they are solvable using this trick.
Complexity: O(nlogn)*/

int n, s, t[N], f[N];
///the cost function, (i, x]
int w(int i, int x) {
  if (i >= x) return 1e9; //inf > w(0, n)
  return s * (f[n] - f[i]) + t[x] * (f[x] - f[i]);
}
int dp[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  cin >> n >> s;
  t[0] = 0;
  f[0] = 0;
  for (int i = 1; i <= n; i++) {
    cin >> t[i] >> f[i];
    t[i] += t[i - 1];
    f[i] += f[i - 1];
  }
  dp[0] = 0;
  vector<pair<int, int> > v; // (start pos, best k)
  v.push_back(make_pair(0, 0));
  for (int x = 1; x <= n; x++) {
    int k = (--lower_bound(v.begin(), v.end(), make_pair(x + 1, 0)))->second;
    dp[x] = dp[k] + w(k, x);
    for (int i = (int)v.size() - 1; i >= 0; i--) {
      int y = v[i].first, oldk = v[i].second;
      if (y > x && dp[x] + w(x, y) < dp[oldk] + w(oldk, y)) v.pop_back();
      else {
        int l = y + 1, r = n + 1;
        while (l < r) {
          int mid = (l + r) / 2;
          if (dp[x] + w(x, mid) < dp[oldk] + w(oldk, mid)) r = mid;
          else l = mid + 1;
        }
        if (r != n + 1) v.push_back(make_pair(r, x));
        break;
      }
    }
    if (v.size() == 0) v.push_back(make_pair(0, x));
  }
  cout << dp[n] << '\n';
  return 0;
}
//https://vjudge.net/problem/OpenJ_Bailian-1180
