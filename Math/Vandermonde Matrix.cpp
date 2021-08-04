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

vector<mint> multiply(vector<mint> a, vector<mint> b) {
  vector<mint> ans(a.size() + b.size() - 1, 0);
  for (int i = 0; i < a.size(); i++) {
    for (int j = 0; j < b.size(); j++) {
      ans[i + j] += a[i] * b[j];
    }
  }
  return ans;
}
/*
matrix form:
  1 x0  x0^2  ... x0^(n-1)
  1 x1  x1^2  ... x1^(n-1)
  1 x2  x2^2  ... x2^(n-1)
  . ..  ....  ... .......
  1 x(n-1)  x(n-1)^2  ... x(n-1)^(n-1)
if the 1st coloumn starts from x_i(2nd column), then inverse will be changed by: ans[i][j] /= x[i]
determinant of this matrix = mul_{1 <= i < j <= n}{x[j] - x[i]}
*/
#define vv vector<vector<mint>>
vv inverse(vector<mint> &x) {
  int n = x.size();
  vv ans(n, vector<mint> (n, 0));
  vector<mint> all(n + 1, 0); //(x - x0) * (x - x1) * ... * (x - x(n-1))
  all[0] = 1;
  for (int i = 0; i < n; i++) {
    for (int j = n; j >= 0; j--) {
      all[j] *= -x[i];
      if (j) all[j] += all[j - 1];
    }
  }
  //lagrange
  for (int i = 0; i < n; i++) {
    vector <mint> up(n); //all / (x - xi)
    mint rem = all[n];
    for (int j = n - 1; j >= 0; --j) {
      up[j] = rem;
      rem = all[j] + rem * x[i];
    }
    mint down = 1;
    for (int j = 0; j < n; j++) if (i != j) down *= x[i] - x[j];
    up.resize(n); down = down.inv();
    for (int j = 0; j < n; j++) ans[i][j] = up[j] * down;
  }
  //transpose
  auto tmp = ans;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) ans[j][i] = tmp[i][j];
  }
  return ans;
}
int32_t main() {
  int n; cin >> n;
  vector<mint> f(n);
  for (int i = 0; i < n; i++) cin >> f[i].value;
  vector<mint> x(n);
  for (int i = 1; i <= n; i++) x[i - 1] = i;
  auto v = inverse(x);
  //transpose
  auto t = v;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) t[j][i] = v[i][j];
  }
  vector<mint> a(n);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      a[i] += t[i][j] * f[j];
    }
  }
  for (int x = 0; x < n; x++) {
    mint ans = 0;
    for (int i = 0; i < n; i++) ans += a[i] * mint(x).pow(i + 1);
    cout << ans << ' ';
  }
  cout << '\n';
  return 0;
}
//https://www.codechef.com/problems/SATA05
