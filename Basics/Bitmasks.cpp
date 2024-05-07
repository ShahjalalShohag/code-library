#include<iostream>
using namespace std;

int main() {
  int n, l, r, x; cin >> n >> l >> r >> x;
  int c[n];
  for (int i = 0; i < n; i++) {
    cin >> c[i];
  }
  int ans = 0;
  for (int mask = 0; mask < (1 << n); mask++) {
    // fix the subset as the elements under this bitmask
    int total_problems = 0, total_difficulty = 0;
    int min_difficulty = 1e6 + 9, max_difficulty = 0;
    for (int i = 0; i < n; i++) {
      if ((mask >> i) & 1) {
        total_problems++;
        total_difficulty += c[i];
        min_difficulty = min(min_difficulty, c[i]);
        max_difficulty = max(max_difficulty, c[i]);
      }
    }
    if (total_problems >= 2 and total_difficulty >= l and total_difficulty <= r
      and abs(min_difficulty - max_difficulty) >= x) {
      ans++;
    }
  }
  cout << ans << '\n';
  return 0;
}
// Time: O(n * 2^n)
// Space: O(n)
// https://codeforces.com/problemset/problem/550/B
