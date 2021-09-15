#include<bits/stdc++.h>
using namespace std;

const int N = 1010;
using ll = long long;
/*
Knuths optimization works for optimization over sub arrays
for which optimal middle point depends monotonously on the end points.
Let mid[l,r] be the first middle point for (l,r) sub array which gives optimal result.
It can be proven that mid[l,r-1] <= mid[l,r] <= mid[l+1,r]
- this means monotonicity of mid by l and r.
Applying this optimization reduces time complexity from O(k^3) to O(k^2)
because with fixed s (sub array length) we have m_right(l) = mid[l+1][r] = m_left(l+1).
That's why nested l and m loops require not more than 2k iterations overall.
*/

int n, k;
int a[N], mid[N][N];
ll res[N][N];
ll solve() {
  for (int s = 0; s <= k; s++) {          // s - length of the subarray
    for (int l = 0; l + s <= k; l++) {  // l - left point
      int r = l + s;                  // r - right point
      if (s < 2) {
        res[l][r] = 0;              // base case- nothing to break
        mid[l][r] = l;              // mid is equal to left border
        continue;
      }
      int mleft = mid[l][r - 1];
      int mright = mid[l + 1][r];
      res[l][r] = 2e18;
      for (int m = mleft; m <= mright; m++) {    // iterating for m in the bounds only
        ll tmp = res[l][m] + res[m][r] + (a[r] - a[l]);
        if (res[l][r] > tmp) {                 // relax current solution
          res[l][r] = tmp;
          mid[l][r] = m;
        }
      }
    }
  }
  ll ans = res[0][k];
  return ans;
}
int main() {
  int i, j, m;
  while(cin >> n >> k) {
    for(i = 1; i <= k; i++) cin >> a[i];
    a[0] = 0;
    a[k + 1] = n;
    k++;
    cout << solve() << endl;
  }
  return 0;
}
// https://vjudge.net/problem/ZOJ-2860
