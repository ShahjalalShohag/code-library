#include<bits/stdc++.h>
using namespace std;

const int mod = 998244353;
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

struct field { // a . 1 + b . sqrt(5)
  mint a, b;
  field (mint a = 0, mint b = 0) : a(a), b(b) {}
  bool operator == (const field &oth) const {
    return a == oth.a and b == oth.b;
  }
  field operator + (const field &oth) const {
    return field(a + oth.a, b + oth.b);
  }
  field operator - (const field &oth) const {
    return field(a - oth.a, b - oth.b);
  }
  field operator * (const field &oth) const {
    return field(a * oth.a + 5 * b * oth.b, a * oth.b + b * oth.a);
  }
  field operator * (const mint &oth) const {
    return field(a * oth, b * oth);
  }
  field inv() {
    mint d = (a * a - b * b * 5).inv();
    return field(a * d, -b * d);
  }
  field operator / (field oth) {
    field f = oth.inv();
    return (*this) * f;
  }
  field operator / (const mint &oth) const {
    mint p = oth.inv();
    return field(a * p, b * p);
  }
  field pow(ll k) {
    field ans(1, 0), cur = *this;
    while (k) {
      if (k & 1) ans = ans * cur;
      cur = cur * cur;
      k >>= 1;
    }
    return ans;
  }
}phi((mod + 1) / 2, (mod + 1) / 2);

mint fib(ll n) {
  field ans = phi.pow(n) - (field(1, 0) - phi).pow(n);
  ans = ans / field(0, 1);
  assert(ans.b == 0);
  return ans.a;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  for (int i = 0; i <= 10; i++) {
    cout << fib(i) << '\n';
  }
  return 0;
}