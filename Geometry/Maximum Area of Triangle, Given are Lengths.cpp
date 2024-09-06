#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,avx2,fma")
#pragma GCC optimize("unroll-loops")

#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;
inline double area( double a, double b, double c ) {
  double p = (a + b + c) / 2;
  return p * (p - a) * (p - b) * (p - c);
}

bitset<N> f;
// values are distinct
// find the maximum area triangle, given are lengths
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t; cin >> t;
  while (t--) {
    int n, m; cin >> n >> m;
    f.reset();
    for (int i = 0; i < n; i++) {
      int k; cin >> k;
      f[k] = 1;
    }
    while (!f[m]) --m;
    double ans = 1e100;
    int a = 1;
    // triples are (a, b, c) (a < b < c)
    for (int d = 1; d * 2 <= m; d++) { // fix the difference between b and c
      while (a <= d or !f[a]) ++a; // find the first a > d
      auto g = f & (f >> d);
      int b = g._Find_next(a); // find the first b > a s.t. b and b + d is in the array
      if (b <= m) {
        ans = min(ans, area(a, b, b + d));
      }
    }
    if (ans < 1e100) {
      cout << fixed << setprecision(10) << sqrt(ans) << '\n';
    }
    else {
      cout << -1 << '\n';
    }
  }
  return 0;
}
// https://codeforces.com/gym/100956/attachments
// problem L
