#include<bits/stdc++.h>
using namespace std;

const int N = 1e6 + 9;
using ll = long long;


/**
 * Tutorial: https://oi-wiki.org/math/number-theory/powerful-number/
 * - f() is multiplicative, need to find prefix sum of f
 * - Consider two functions g() and h() such that f = g [*] h (dirichlet convolution)
 * - So f(n) = sum_{d | n}{g(d) * h(n / d)}
 * - Condition: This g() is special 
 *    => for all primes p, f(p) = g(p)
 *    => prefix sum of g() is easy to calculate
 * - We have f(n) = sum_{d = 1 to n}{h(d) * g_prefix_sum(floor(n / d))}
 * - But h(p) is 0 from all primes (check the blog)
 * - So we can brute over all powerful numbers only and there are only O(sqrt(n)) powerful number upto n
 * 
 * General approach: 
 * - Find suitable g() and fast prefix sum of g()
 * - Calculate h(p^k) using the method described in the blog, generally its like this:
 *   => Expand f(p^k) = sum_{i = 0 to k}{g(p^(k-i)) * h(p^i)}
 *   => Separate the h(p^k) term and get its recurrence relation 
 *   => Then subtract equations from h(p^k) to h(p^(k-1))
 * - Finally brute over all powerful numbers and get the result
 * 
 * Total complexity: O(sqrt(n) * complexity of prefix sum of g())
 **/

const ll mod = 4179340454199820289;
inline ll mul(ll x, ll y) {
  return __int128(x) * y % mod;
}
inline ll add(ll x, ll y) {
  return (x += y) >= mod ? x - mod : x;
}
ll power(ll x, ll n) {
  ll ans = 1;
  for (; n; n >>= 1, x = mul(x, x)) {
    if (n & 1) ans = mul(ans, x);
  }
  return ans;
}

// sieve upto sqrt(n)
int spf[N];
vector<int> primes;
void sieve() {
  for(int i = 2; i < N; i++) {
    if (spf[i] == 0) spf[i] = i, primes.push_back(i);
    int sz = primes.size();
    for (int j = 0; j < sz && i * primes[j] < N && primes[j] <= spf[i]; j++) {
      spf[i * primes[j]] = primes[j];
    }
  }
}

ll inv[60];

// g(x) = x
ll g_prefix_sum(ll n) { // return prefix sum of g()
  return mul(mul(n, n + 1), inv[2]);
}

// h(p^k) = -p^k / (k * (k - 1))
ll get_h(int p, int k, ll pk) { // return h(p^k), k >= 2
  return mod - mul(mul(pk, inv[k]), inv[k - 1]);
}

ll n;
// generate all powerful numbers and add up the results
// params: current prime index, current number generated, and h(m)
ll brute(int i, ll m, ll h) {
  ll ans = mul(h, g_prefix_sum(n / m));
  ll lim = n / m;
  while (i < primes.size() and 1LL * primes[i] * primes[i] <= lim) {
    int k = 2; ll pk = 1LL * primes[i] * primes[i];
    while (pk <= lim) {
      ans = add(ans, brute(i + 1, m * pk, mul(h, get_h(primes[i], k, pk))));
      ++k;
      pk *= primes[i];
    }
    ++i;
  }
  return ans;
}


void solve() {
  cin >> n;
  ll prefix_sum = brute(0, 1, 1);
  ll ans = mul(power(n, mod - 2), prefix_sum);
  cout << ans << '\n';
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  inv[1] = 1;
  for (int i = 2; i < 60; i++) {
    inv[i] = power(i, mod - 2);
  }
  sieve();

  int t = 1;
  cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}
// https://vjudge.net/problem/HDU-7186
