#include<bits/stdc++.h>
using namespace std;

using ll = long long;

// \sum{k=1}^{n}{floor(n/k)}
// count of numbers such that n/i = k -> n/k - n/(k+1)
ll floor_sum(ll n) {
  ll sum = 0;
  for (ll i = 1, last; i <= n; i = last + 1) {
    last = n / (n / i);
    sum += (n / i) * (last - i + 1);
    // n / x yields the same value for i <= x <= last.
  }
  return sum;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  return 0;
}