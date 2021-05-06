#include<bits/stdc++.h>
using namespace std;

const int N = 2e5 + 9, D = 3; // N >= 2 * sqrt(n)
const int mod = 1e9 + 7;
using ll = long long;

template <const int32_t MOD>
struct modint {
  int32_t value;
  modint() = default;
  modint(int32_t value_) : value(value_) {}
  inline modint<MOD> operator + (modint<MOD> other) const { int32_t c = this->value + other.value; return modint<MOD>(c >= MOD ? c - MOD : c); }
  inline modint<MOD> operator - (modint<MOD> other) const { int32_t c = this->value - other.value; return modint<MOD>(c <    0 ? c + MOD : c); }
  inline modint<MOD> operator * (modint<MOD> other) const { int32_t c = (int64_t)this->value * other.value % MOD; return modint<MOD>(c < 0 ? c + MOD : c); }
  inline modint<MOD> & operator += (modint<MOD> other) { this->value += other.value; if (this->value >= MOD) this->value -= MOD; return *this; }
  inline modint<MOD> & operator -= (modint<MOD> other) { this->value -= other.value; if (this->value <    0) this->value += MOD; return *this; }
  inline modint<MOD> & operator *= (modint<MOD> other) { this->value = (int64_t)this->value * other.value % MOD; if (this->value < 0) this->value += MOD; return *this; }
  inline modint<MOD> operator - () const { return modint<MOD>(this->value ? MOD - this->value : 0); }
  modint<MOD> pow(uint64_t k) const { modint<MOD> x = *this, y = 1; for (; k; k >>= 1) { if (k & 1) y *= x; x *= x; } return y; }
  modint<MOD> inv() const { return pow(MOD - 2); }  // MOD must be a prime
  inline modint<MOD> operator /  (modint<MOD> other) const { return *this *  other.inv(); }
  inline modint<MOD> operator /= (modint<MOD> other)       { return *this *= other.inv(); }
  inline bool operator == (modint<MOD> other) const { return value == other.value; }
  inline bool operator != (modint<MOD> other) const { return value != other.value; }
  inline bool operator < (modint<MOD> other) const { return value < other.value; }
  inline bool operator > (modint<MOD> other) const { return value > other.value; }
};
template <int32_t MOD> modint<MOD> operator * (int64_t value, modint<MOD> n) { return modint<MOD>(value) * n; }
template <int32_t MOD> modint<MOD> operator * (int32_t value, modint<MOD> n) { return modint<MOD>(value % MOD) * n; }
template <int32_t MOD> istream & operator >> (istream & in, modint<MOD> &n) { return in >> n.value; }
template <int32_t MOD> ostream & operator << (ostream & out, modint<MOD> n) { return out << n.value; }

using mint = modint<mod>;

using T = mint;

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
  T ans = pw % mod;
  return ans * (ans - 1);
}

// no memorization needed!
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

// Min_25: Say my name
// Dirichlet: You're heisenberg
// Min_25: You're goddamn right

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
    T z = x % mod; // remove this mod if not needed
    for (int k = 0; k < deg; k++) {
      if (k == 0) {
        g[k][tot] = z - 1;
      }
      if (k == 1) {
        g[k][tot] = z * (z + 1) / 2 - 1;
      }
      if (k == 2) {
        g[k][tot] = z * (z + 1) * (2 * z + 1) / 6 - 1;
      }
      // compute for larger values using lagrange if needed
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
  vector<T> p({0, -1, 1});
  ll n; cin >> n;
  cout << solve(n, p) << '\n';
  return 0;
}

// https://www.luogu.com.cn/problem/P5325
