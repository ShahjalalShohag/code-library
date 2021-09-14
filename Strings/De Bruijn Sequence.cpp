#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9; // >= k^n

// creates a cyclic string of length k^n that contains every length n string as a substring. alphabet = [0, k - 1]
// O(k^n)
int ans[N], aux[N];
int de_bruijn(int k, int n) { // returns size (k^n)
  if (k == 1) {
    ans[0] = 0;
    return 1;
  }
  for (int i = 0; i < k * n; i++) {
    aux[i] = 0;
  }
  int sz = 0;
  function<void(int, int)> db = [&](int t, int p) {
    if (t > n) {
      if (n % p == 0) {
        for (int i = 1; i <= p; i++) {
          ans[sz++] = aux[i];
        }
      }
    }
    else {
      aux[t] = aux[t - p];
      db(t + 1, p);
      for (int i = aux[t - p] + 1; i < k; i++) {
        aux[t] = i;
        db(t + 1, t);
      }
    }
  };
  db(1, 1);
  return sz;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int k, n; cin >> k >> n;
  int len = de_bruijn(k, n);
  for (int i = 0; i < len; i++) cout << ans[i] << ' '; 
  cout << '\n';
  return 0;
}
