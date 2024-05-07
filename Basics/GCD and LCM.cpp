#include<bits/stdc++.h>
using namespace std;

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t; cin >> t;
  while (t--) {
    int a, b; cin >> a >> b;
    int GCD = __gcd(a, b);
    long long LCM = 1LL * a * b / GCD;
    cout << LCM << ' ' << GCD << '\n';
  }
  return 0;
}
// https://vjudge.net/problem/HackerRank-si-lcm-and-hcf
