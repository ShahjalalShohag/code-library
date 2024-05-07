#include<bits/stdc++.h>
using namespace std;

using ll = long long;

vector<ll> get_divisors(ll n) {
  vector<ll> divs;
  for (int i = 1; 1LL * i * i <= n; i++) {
    if (n % i == 0) {
      divs.push_back(i);
      if (i != n / i) {
        divs.push_back(n / i);
      }
    }
  }
  sort(divs.begin(), divs.end());
  return divs;
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  ll n, k; cin >> n >> k;
  vector<ll> divs = get_divisors(n);
  if (k > divs.size()) {
    cout << -1 << '\n';
  } else {
    cout << divs[k - 1] << '\n';
  }
  return 0;
}
// https://codeforces.com/problemset/problem/762/A
