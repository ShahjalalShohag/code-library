#include<bits/stdc++.h>
using namespace std;

const int N = 1e6 + 9;
int d[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  // total complexity of the following two loops = O(n logn)
  // because the number of times the loops are running 
  // = n / 1 + n / 2 + ... + n / n
  // = n * (1 / 2 + 1 / 2 + ... + 1 / n)
  // = n * H(n) [here H(n) is nth Harmonic number]
  // = O(n * log(n)) because H(n) is around O(log n)
  for (int i = 1; i < N; i++) {
    for (int j = i; j < N; j += i) {
      d[j]++;
    }
  }

  int t; cin >> t;
  while (t--) {
    int n; cin >> n;
    cout << d[n] << '\n';
  }
  return 0;
}
// https://cses.fi/problemset/task/1713/
