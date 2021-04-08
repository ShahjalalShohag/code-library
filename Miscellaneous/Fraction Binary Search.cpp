#include<bits/stdc++.h>
using namespace std;

/**
Given a function f and n, finds the smallest fraction p / q in [0, 1] or [0,n]
such that f(p / q) is true, and p, q <= n.
Time: O(log(n))
**/
struct frac { long long p, q; };
bool f(frac x) {
 return 6 + 8 * x.p >= 17 * x.q + 12;
}
frac fracBS(long long n) {
  bool dir = 1, A = 1, B = 1;
  frac lo{0, 1}, hi{1, 0}; // Set hi to 1/0 to search within [0, n] and {1, 1} to search within [0, 1]
  if (f(lo)) return lo;
  assert(f(hi)); //checking if any solution exists or not
  while (A || B) {
    long long adv = 0, step = 1; // move hi if dir, else lo
    for (int si = 0; step; (step *= 2) >>= si) {
      adv += step;
      frac mid{lo.p * adv + hi.p, lo.q * adv + hi.q};
      if (abs(mid.p) > n || mid.q > n || dir == !f(mid)) {
        adv -= step; si = 2;
      } 
    }
    hi.p += lo.p * adv;
    hi.q += lo.q * adv;
    dir = !dir;
    swap(lo, hi);
    A = B; B = !!adv;
  }
  return dir ? hi : lo;
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  frac ans=fracBS(10);
  cout << ans.p << ' ' << ans.q << '\n';
  return 0;
}
// Relevant Problem: https://codeforces.com/gym/102354/submission/69260720