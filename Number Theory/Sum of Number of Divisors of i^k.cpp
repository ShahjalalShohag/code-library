#include<bits/stdc++.h>
using namespace std;

const int N = 2e5 + 9, D = 1; // N >= 2 * sqrt(n)
using ll = long long;
using T = unsigned long long;

/**
Let f(x) be a multiplicative function and
when p is a prime, f(p) has a polynomial representation 
and f(p^k) can be calculated quickly

We can compute the prefix sum of f(n) in O((n^(3/4)) / logn) using Min_25 sieve
Takes ~1s for n = 10^10 (deg = 3) in CF
unfold the loops over poly to make the code faster
Tutorial: https://www.luogu.com.cn/problem/solution/P5325
**/
int primes[N], p, deg;
T pref[D][N];
T poly[D]; // polynomial representation of f(p)
void sieve(int n) {
  vector<bool> f(n + 1, false);
  p = 0;
  for (int i = 2; i <= n; i++) {
    if (!f[i]) {
      primes[++p] = i;
      T cur = 1;
      // pref[k][p] = sum of primes[i]^k s.t. i <= p
      for (int k = 0; k < deg; k++) {
        pref[k][p] = pref[k][p - 1] + cur;
        cur *= i;
      }
    }
    for(int j = 1; j <= p and primes[j] * i <= n; j++) {
      f[i * primes[j]] = 1;
      if (i % primes[j] == 0) break;
    }
  }
}

int r, id1[N], id2[N];
ll n;
int get_id(ll x) {
  return x <= r ? id1[x] : id2[n / x];
}
int tot;
T g[D][N];
ll a[N];

// f(p^k) in O(1)
inline T eval(int p, int k, ll pw) { // pw = p^k, pw <= n
  return (poly[0] - 1) * k + 1;
}

// no memorization needed
// sum of f(i) s.t. spf[i] > primes[j]
T yo(ll x, int j) {
  if (primes[j] >= x) return 0;
  int id = get_id(x);
  T ans = 0;
  // initialization: ans = g(x) - g(primes[j])
  for (int k = 0; k < deg; k++) {
    ans += poly[k] * g[k][id];;
  }
  for (int k = 0; k < deg; k++) {
    ans -= poly[k] * pref[k][j];
  }
  for (int i = j + 1; i <= p and primes[i] <= x / primes[i]; i++) {
    ll pw = primes[i];
    for (int e = 1; pw <= x; e++) {
      ans += eval(primes[i], e, pw) * (yo(x / pw, i) + (e != 1));
      if (!(pw <= x / primes[i])) break;
      pw *= primes[i];
    }
  }
  return ans;
}

// sum of f(i) for 1 <= i <= n
// pol is the polynomial representation of f(p)
T solve(ll _n, vector<T> pol) {
  n = _n;
  deg = pol.size();
  for (int i = 0; i < deg; i++) {
    poly[i] = pol[i];
  }
  r = sqrt(n);
  while (1LL * r * r < n) ++r;
  while (1LL * r * r > n) --r;
  sieve(r);
  tot = 0;
  ll i = 1;
  while (i <= n) {
    ll x = n / i;
    ll j = n / x;
    a[++tot] = x;

    // initialization g[k][tot] = sum of i^k for 2 <= i <= x
    T z = x;
    for (int k = 0; k < deg; k++) {
      if (k == 0) {
        g[k][tot] = z - 1;
      }
    }

    if (x <= r) id1[x] = tot;
    else id2[n / x] = tot;
    i = j + 1;
  }

  // an integer x belongs to the array a iff for some integer z, n / z = x
  // g[k][i] = sum of prime^k for prime <= a[i]
  for (int i = 1; i <= p; i++) {
    for (int j = 1; j <= tot && primes[i] <= a[j] / primes[i]; j++) {
      int id = get_id(a[j] / primes[i]);
      T cur = 1;
      for (int k = 0; k < deg; k++) {
        g[k][j] -= cur * (g[k][id] - pref[k][i - 1]);
        cur *= primes[i];
      }
    }
  }
  return yo(n, 0) + 1; // add f(1)
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t; cin >> t;
  while (t--) {
    ll n, m; cin >> n >> m;
    vector<T> p({T(m) + 1});
    auto ans = solve(n, p);
    string s = "";
    while (ans > 0) {
      s += char('0' + ans % 10);
      ans /= 10;
    }
    reverse(s.begin(), s.end());
    cout << s << '\n';  
  }
  return 0;
}
// https://www.spoj.com/problems/DIVCNTK/
