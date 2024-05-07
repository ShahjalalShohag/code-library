#include<bits/stdc++.h>
using namespace std;
 
using ll = long long;
const int N = 1e5 + 9;
int a[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n; ll s; cin >> n >> s;
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }
  ll ans = 0;
  int r = 1;
  ll sum = 0;
  for (int l = 1; l <= n; l++) {
    while (r <= n and sum + a[r] <= s) {
      sum += a[r];
      r++;
    }
    // r - 1 is the maximum index i such that sum of a[l...i] is <= s
    ans += r - l;
    sum -= a[l];
  }
  cout << ans << '\n';
  return 0;
}
// https://codeforces.com/edu/course/2/lesson/9/2/practice/contest/307093/problem/C
