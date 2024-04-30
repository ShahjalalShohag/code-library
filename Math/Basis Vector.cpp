#include<bits/stdc++.h>
using namespace std;

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
  bool can(int x) {
    for(int i = B - 1; i >= 0; i--) {
      x = min(x, x ^ a[i]);
    }
    return x == 0;
  }
  int max_xor(int ans = 0) {
    for(int i = B - 1; i >= 0; i--) {
      ans = max(ans, ans ^ a[i]);
    }
    return ans;
  }
};

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t, cs = 0; cin >> t;
  while (t--) {
    int n; cin >> n;
    Basis B;
    for (int i = 1; i <= n; i++) {
      int x; cin >> x;
      B.insert(x);
    }
    cout << "Case " << ++cs << ": " << B.max_xor() << '\n';
  }
  return 0;
}
// https://lightoj.com/problem/maximum-subset-sum
