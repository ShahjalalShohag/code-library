#include<bits/stdc++.h>
using namespace std;

const int N = 1e7 + 9, mod = 998244353;

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

struct Combi{
  int n; vector<mint> facts, finvs, invs;
  Combi(int _n): n(_n), facts(_n), finvs(_n), invs(_n){
    facts[0] = finvs[0] = 1;
    invs[1] = 1;
    for (int i = 2; i < n; i++) invs[i] =  invs[mod % i] * (-mod / i);
    for(int i = 1; i < n; i++){
      facts[i] = facts[i - 1] * i;
      finvs[i] = finvs[i - 1] * invs[i];
    }
  }
  inline mint fact(int n) { return facts[n]; }
  inline mint finv(int n) { return finvs[n]; }
  inline mint inv(int n) { return invs[n]; }
  inline mint ncr(int n, int k) { return n < k ? 0 : facts[n] * finvs[k] * finvs[n-k]; }
};
Combi C(N);

//p = first at least n + 1 points: a, a+d, ..., a+n*d of the n degree polynomial, returns f(k)
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
// Given a polynomial of degree k, 
// f(x) = \sum_{i = 0}^{k}{c_i * x^i} and c_k != 0
// Let S(n) = \sum_{i = 0}^{n}{a^i * f(i)}
// Given S(0), S(1), ... S(k) find S(n)
// O(k log mod), can be optimized to O(k)
// Take care of the special case [a = 1] separately
mint yo(vector<mint> S, long long n, mint a, int k) {
  assert(k + 1 == S.size());
  assert(a != 1);
  mint c = 0;
  for (int i = 0; i <= k; i++) {
    c += C.ncr(k + 1, i + 1) * (-a).pow(k - i) * S[i];
  }
  c /= (-a + 1).pow(k + 1);
  mint nw = 1;
  for (int i = 0; i <= k; i++) {
    S[i] = (S[i] - c) * nw;
    nw /= a;
  }
  return Lagrange(S, mint(n % mod)) * a.pow(n) + c;
}
int k;
mint f(int i) { // f(i)
  return mint(i).pow(k);
}
// Find \sum{i = 0}{n - 1}{a^i * i^k}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  long long a, n; cin >> a >> k >> n; n--;
  if (n < 0) {
    cout << 0 << '\n'; return 0;
  }
  vector<mint> p;
  mint sum = 0;
  for (int i = 0; i <= k; i++) {
    sum += f(i) * mint(a).pow(i);
    p.push_back(sum);
  }
  if (a == 1) {
    sum += f(k + 1) * mint(a).pow(k + 1);
    p.push_back(sum);
    cout << Lagrange(p, mint(n % mod)) << '\n';
  }
  else cout << yo(p, n, a, k) << '\n';
  return 0;
}
// https://judge.yosupo.jp/problem/sum_of_exponential_times_polynomial
// https://min-25.hatenablog.com/entry/2015/04/24/031413
