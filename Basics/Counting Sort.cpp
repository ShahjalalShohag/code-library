#include<bits/stdc++.h>
using namespace std;

const int MAX = 100;
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n; cin >> n;
  vector<int> cnt(MAX + 1, 0);
  for (int i = 1; i <= n; i++) {
    int x; cin >> x;
    cnt[x]++;
  }
  for (int i = 0; i <= MAX; i++) {
    while (cnt[i] > 0) {
      cout << i << ' ';
      cnt[i]--;
    }
  }
  cout << '\n';
  return 0;
}
// https://vjudge.net/problem/HackerRank-countingsort2
