#include<bits/stdc++.h>
using namespace std;

const int N = 303, mod = 1e9 + 7;

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

mint inv[N];
// sum of i1 * i2 * ... ik s.t. 0 <= i1 < i2 < ... < ik <= n
// O(k^2 log n), can be optimized to O(k logk logn)
vector<mint> yo(int n, int k) {
  vector<mint> ans(k + 1, 0);
  if (n == 0) {
    ans[0] = 1;
    return ans;
  }
  if (n & 1) {
    auto l = yo(n - 1, k);
    for (int i = 0; i <= k; i++) {
      ans[i] = l[i];
      if (i) {
        ans[i] += l[i - 1] * n;
      }
    }
    return ans;
  }
  auto l = yo(n / 2, k);
  vector<mint> r(k + 1, 0);
  vector<vector<mint>> f(k + 1, vector<mint> (k + 1, 0));
  int p = n / 2;
  for (int j = 0; j <= k; j++) {
    if (p - j < 0) break;
    mint cur = 1;
    for (int i = 0; i <= k; i++) {
      f[j][i] = cur;
      if (p - j - i < 0) break;
      cur *= p - j - i;
      cur *= inv[i + 1];
    }
  }
  vector<mint> pw(k + 1);
  mint cur = 1;
  for (int i = 0; i <= k; i++) {
    pw[i] = cur;
    cur *= p;
  }
  for (int i = 0; i <= k; i++) {
    for (int j = 0; j <= i; j++) {
      r[i] += pw[i - j] * l[j] * f[j][i - j]; // f[j][i - j] = ncr(p - j, i - j)
    }
  }
  for (int i = 0; i <= k; i++) {
    for (int j = 0; i + j <= k; j++) {
      ans[i + j] += l[i] * r[j];
    }
  }
  return ans;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  for (int i = 1; i < N; i++) {
    inv[i] = mint(i).inv();
  }
  int n, k; cin >> n >> k;
  auto ans = yo(n - 1, k);
  for (int i = 2; i <= k; i++) {
    ans[i] += ans[i - 2];
  }
  for (int i = 1; i <= k; i++) {
    cout << ans[i] << ' ';
  }
  return 0;
}
// https://codeforces.com/contest/1516/problem/E
