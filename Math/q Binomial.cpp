#include<bits/stdc++.h>
using namespace std;

const int N = 1 << 18, mod = 998244353;

const int root = 3;
int lim, rev[N], w[N], wn[N], inv_lim;
void reduce(int &x) { x = (x + mod) % mod; }
int POW(int x, int y, int ans = 1) {
  for (; y; y >>= 1, x = (long long) x * x % mod) if (y & 1) ans = (long long) ans * x % mod;
  return ans;
}
void precompute(int len) {
  lim = wn[0] = 1; int s = -1;
  while (lim < len) lim <<= 1, ++s;
  for (int i = 0; i < lim; ++i) rev[i] = rev[i >> 1] >> 1 | (i & 1) << s;
  const int g = POW(root, (mod - 1) / lim);
  inv_lim = POW(lim, mod - 2);
  for (int i = 1; i < lim; ++i) wn[i] = (long long) wn[i - 1] * g % mod;
}
void ntt(vector<int> &a, int typ) {
  for (int i = 0; i < lim; ++i) if (i < rev[i]) swap(a[i], a[rev[i]]);
  for (int i = 1; i < lim; i <<= 1) {
    for (int j = 0, t = lim / i / 2; j < i; ++j) w[j] = wn[j * t];
    for (int j = 0; j < lim; j += i << 1) {
      for (int k = 0; k < i; ++k) {
        const int x = a[k + j], y = (long long) a[k + j + i] * w[k] % mod;
        reduce(a[k + j] += y - mod), reduce(a[k + j + i] = x - y);
      }
    }
  }
  if (!typ) {
    reverse(a.begin() + 1, a.begin() + lim);
    for (int i = 0; i < lim; ++i) a[i] = (long long) a[i] * inv_lim % mod;
  }
}
vector<int> multiply(vector<int> &f, vector<int> &g) {
  int n=(int)f.size() + (int)g.size() - 1;
  precompute(n);
  vector<int> a = f, b = g;
  a.resize(lim); b.resize(lim);
  ntt(a, 1), ntt(b, 1);
  for (int i = 0; i < lim; ++i) a[i] = (long long) a[i] * b[i] % mod;
  ntt(a, 0);
  return a;
}

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

// The code is mainly implemented for q = 2
// But I have commented out some ideas for general q

mint pw[N];
mint q_fac[N], q_inv_fac[N];
int q = 2;

// [n] = (q^n - 1) / (q - 1)
// [n]! = [1] * [2] ... [n]

void prec() {
  pw[0] = 1;
  for (int i = 1; i < N; i++) {
    pw[i] = pw[i - 1] * q;
  }
  q_fac[0] = 1;
  mint inv = mint(q - 1).inv();
  for (int i = 1; i < N; i++) {
    mint cur = (pw[i] - 1) * inv;
    q_fac[i] = q_fac[i - 1] * cur;
  }
  q_inv_fac[N - 1] = q_fac[N - 1].inv();
  for (int i = N - 2; i >= 0; i--) {
    q_inv_fac[i] = q_inv_fac[i + 1] * (pw[i + 1] - 1) * inv;
  }
}

// number of r dimensional subspaces of n dimensional vector space
// [n]! / ([r]! * [n - r]!)
mint q_binom(int n, int r) {
  if (n < r) return 0;
  return q_fac[n] * q_inv_fac[r] * q_inv_fac[n - r];
}

// [n]! / [n - r]!
mint q_perm(int n, int r) {
  if (n < r) return 0;
  return q_fac[n] * q_inv_fac[n - r];
}

// number of ways to select k linearly independant vectors of length n
// mul(q^n - q^i) over 0 <= i < k
mint yo(int n, int k) {
  return q_perm(n, k) * mint(q).pow(1LL * k * (k - 1) / 2) * mint(q - 1).pow(k);
}

// ways to select k vectors of length n having basis size r
mint count_basis_general(int k, int n, int r) {
  return yo(k, r) * q_binom(n, r);
  // same as yo(n, r) * q_binom(k, r)
}

// number of length n sequences with elements in [0, 2^k) such that its basis has size r
// same as the above function
mint count_basis(int n, int k, int r) {
  return q_binom(n, r) * q_perm(k, r) * mint(q).pow(1LL * r * (r - 1) / 2) * mint(q - 1).pow(r);
}

// number of length n sequences with elements in [0, 2^k) such that no subset has xor 0
// = number of length n independent subsets of elements in [0, 2^k)
// = \prod_{i=0}^{n-1}{(2^k-2^i)}
mint count_except_xor_0(int n, int k) {
  if (n > k) return 0;
  return q_perm(k, n) * mint(q).pow(1LL * n * (n - 1) / 2) * mint(q - 1).pow(n);
}

// number of length n sequences with elements in [0, 2^k) such that no subset has xor 1 and its basis has size r
// not sure for general q, maybe also multiply with (q - 1)^(r + 1) ?
mint count_rank_r_except_xor_1(int n, int k, int r) {
  if (r >= k) return 0;
  return q_binom(n, r) * q_perm(k - 1, r) * mint(2).pow(1LL * r * (r + 1) / 2);
}

// number of length n sequences with elements in [0, 2^k) such that no subset has xor 1
mint count_except_xor_1(int n, int k) {
  mint ans = 0;
  for (int r = 0; r <= k; r++) {
    ans += count_rank_r_except_xor_1(n, k, r);
  }
  return ans;
}

// number of length i sequences with elements in [0, 2^k) such that no subset has xor 1
// for all 1 <= i <= n
// O(n logn)
vector<mint> count_except_xor_1_for_all_n(int n, int k) {
  vector<int> a(n + 1), b(n + 1);
  for (int i = 0; i <= n; i++) {
    if (i <= k - 1) {
      a[i] = (mint(2).pow(1LL * i * (i + 1) / 2) * q_inv_fac[i] * q_inv_fac[k - 1 - i]).value;
    }
    else {
      a[i] = 0;
    }
  }
  for (int i = 0; i <= n; i++) {
    b[i] = q_inv_fac[i].value;
  }
  auto c = multiply(a, b);
  c.resize(n + 1);
  vector<mint> ans(n + 1, 0);
  for (int i = 1; i <= n; i++) {
    ans[i] = q_fac[k - 1] * q_fac[i] * c[i];
  }
  return ans;
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  prec();
  int t; cin >> t;
  while (t--) {
    int n, k, x; cin >> n >> k >> x;
    mint ans = 0;
    if (x) {
      ans = count_except_xor_1(n, k);
    }
    else {
      ans = count_except_xor_0(n, k);
    }
    cout << ans << '\n';
  }
  return 0;
}
// https://codeforces.com/problemset/problem/1603/F
// https://atcoder.jp/contests/abc278/editorial/5238
// https://www.cnblogs.com/rainybunny/p/16475568.html
