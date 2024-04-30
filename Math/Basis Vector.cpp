#include<bits/stdc++.h>
using namespace std;

template<typename T = int, int B = 31>
struct Basis {
  T a[B];
  Basis() {
    memset(a, 0, sizeof a);
  }
  void insert(T x){
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
  bool can(T x) {
    for(int i = B - 1; i >= 0; i--) {
      x = min(x, x ^ a[i]);
    }
    return x == 0;
  }
  T max_xor(T ans = 0) {
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
    Basis<long long, 63> B;
    for (int i = 1; i <= n; i++) {
      long long x; cin >> x;
      B.insert(x);
    }
    cout << "Case " << ++cs << ": " << B.max_xor() << '\n';
  }
  return 0;
}
// https://lightoj.com/problem/maximum-subset-sum
