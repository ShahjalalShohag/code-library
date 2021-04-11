#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, mod = 998244353;

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

// thomas algorithm for solving tri-diagonal system of equations
vector<mint> Thomas(vector<vector<mint>> a) {
    int n = a.size();
    for (int i = 1; i < n; i++) {
        mint x = a[i][0] * a[i - 1][1].inv();
        a[i][1] -= x * a[i - 1][2];
        a[i][3] -= x * a[i - 1][3];
    }
    for (int i = n - 2; i >= 0; i--) {
        mint x = a[i][2] * a[i + 1][1].inv();
        a[i][3] -= x * a[i + 1][3];
    }
    vector<mint> ans;
    for(int i = 0; i < n; i++) {
        ans.push_back(a[i][3] * a[i][1].inv());
    }
    return ans;
}
int a[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n; cin >> n;
  mint m = 0;
  for (int i = 0; i < n; i++) cin >> a[i], m += a[i];
  vector<vector<mint>> mat(m.value + 1);
  for (int i = 0; i <= m.value; i++) {
    vector<mint> v(4, 0);
    if (i) v[0] = mint(i) / m;
    v[1] = (m - i) * (n - 2) / m / (n - 1) - 1;
    if (i + 1 <= m.value) v[2] = (m - i) / m / (n - 1);
    v[3] = mint(-i) / m;
    mat[i] = v;
  }
  vector<mint> f = Thomas(mat);
  mint ans = 0;
  for (int i = 0; i < n; i++) ans += f[a[i]];
  ans -= (n - 1) * f[0] + f[m.value];
  cout << ans << '\n';
  return 0;
}
// https://codeforces.com/contest/1349/problem/D
// https://codeforces.com/blog/entry/77284#comment-620956