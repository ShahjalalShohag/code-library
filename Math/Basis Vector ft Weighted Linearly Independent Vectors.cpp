#include<bits/stdc++.h>
using namespace std;

template<typename T>
struct Basis {
  static const int B = 127;
  T a[B];
  long long wt[B];
  Basis() {
    memset(a, 0, sizeof a);
    memset(wt, 0, sizeof wt);
  }
  void insert(T x, long long w) {
    for (int i = B - 1; i >= 0; i--) {
      if (x >> i & 1) {
        if (a[i] == 0) {
          a[i] = x;
          wt[i] = w;
          break;
        }
        if (wt[i] < w) {
          swap(wt[i], w);
          swap(a[i], x);
        }
        x ^= a[i];
      }
    }
  }
  // maximum sum of linearly independent vectors
  long long query() {
    long long ans = 0;
    for (int i = 0; i < B; i++) {
      ans += wt[i];
    }
    return ans;
  }
};
Basis<__int128> t;
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, q; cin >> n >> q;
  while (q--) {
    int u, v;
    long long x, w; cin >> u >> v >> x >> w;
    __int128 cur = x, b = 1;
    cur |= b << (62 + u); 
    cur |= b << (62 + v);
    t.insert(cur, w);
    cout << t.query() << '\n';
  }
  return 0;
}
// https://codeforces.com/gym/102331/problem/E