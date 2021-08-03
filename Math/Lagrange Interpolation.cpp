#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, mod = 1e9 + 7;

template <int32_t MOD>
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
  modint<MOD> pow(uint64_t k) const {
    modint<MOD> x = *this, y = 1;
    for (; k; k >>= 1) {
      if (k & 1) y *= x;
      x *= x;
    }
    return y;
  }
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
template <int32_t MOD> ostream & operator << (ostream & out, modint<MOD> n) { return out << n.value; }

using mint = modint<mod>;

//p = first at least n + 1 points: a, a+d, ..., a+n*d of the n degree polynomial, returns f(x)
mint Lagrange(const vector<mint> &p, mint x, mint a = 0, mint d = 1) {
  int n = p.size() - 1;
  if (a == 0 and d == 1 and x.value <= n) return p[x.value];
 
  vector<mint> pref(n + 1, 1), suf(n + 1, 1);
  for (int i = 0; i < n; i++) pref[i + 1] = pref[i] * (x - (a + d * i));
  for (int i = n; i > 0; i--) suf[i - 1] = suf[i] * (x - (a + d * i));
 
  vector<mint> fact(n + 1, 1),finv(n + 1, 1);
  for (int i = 1; i <= n; i++) fact[i] = fact[i - 1] * d * i;
  finv[n] /= fact[n];
  for (int i = n; i >= 1; i--) finv[i - 1] = finv[i] * d * i;
 
  mint ans = 0;
  for (int i = 0; i <= n; i++) {
    mint tmp = p[i] * pref[i] * suf[i] * finv[i] * finv[n-i];
    if ((n - i) & 1) ans -= tmp;
    else ans += tmp;
  }
 
  return ans;
}
int32_t main() {
  int n, k; cin >> n >> k;
  vector<mint> p;
  mint sum = 0; p.push_back(0);
  for (int i = 1; i <= k + 1; i++) {
    sum += mint(i).pow(k);
    p.push_back(sum);
  }
  cout << Lagrange(p, n) << '\n';
  return 0;
}
