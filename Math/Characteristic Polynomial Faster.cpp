#include<bits/stdc++.h>
using namespace std;

const int N = 505, mod = 998244353;

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

// transformtation: a(i, j) = 0 for i > j + 1
vector<vector<mint>> Hessenberg(vector<vector<mint>> a) {
  int n = (int)a.size();
  for (int col = 0, row = 0; col < n && row + 1 < n; ++col) {
    int cur = -1;
    for (int i = row + 1; i < n; i++) if (a[i][col] != 0) {cur = i; break; }
    if (cur == -1) {continue;}
    for (int i = 0; i < n; i++) swap(a[row + 1][i], a[cur][i]);
    for (int i = 0; i < n; i++) swap(a[i][row + 1], a[i][cur]); // applying a similar transformation so that determinant doesn't change
    // still a[row + 1][col] is nonzero
    for (int i = row + 2; i < n; i++) {
      if (a[i][col] != 0) {
        mint c = a[i][col] / a[row + 1][col];
        for (int j = 0; j < n; j++) a[i][j] -= a[row + 1][j] * c;
        for (int j = 0; j < n; j++) a[j][row + 1] += a[j][i] * c; // applying a similar transformation so that determinant doesn't change
      }
    }
    ++row;
  }
  return a;
}

// finds the determinant of (a - xI) in O(n^2), here a is a hessenberg matrix
mint yo(vector<vector<mint>> a, mint x) {
  int n = a.size();
  for (int i = 0; i < n; i++) {
    a[i][i] -= x;
  }
  bool flip = 0;
  for (int col = 0, row = 0; col < n && row + 1 < n; ++col) {
      if (a[row][col] == 0 && a[row + 1][col] == 0) return 0;
      if (a[row][col] == 0) {
        for (int i = col; i < n; i++) swap(a[row][i], a[row + 1][i]);
        flip ^= 1;
      }
      mint c = a[row + 1][col] / a[row][col];
      for(int j = col; j < n; j++) a[row + 1][j] -= a[row][j] * c;
      ++row;
  }
  mint det = 1;
  for (int i = 0; i < n; i++) {
    det *= a[i][i];
  }
  if (flip) det *= mod - 1;
  return det;
}

//generates the polynomial explicitely
vector<mint> Lagrange(vector<mint> &x, vector<mint> &y) {
  int n = x.size();
    vector<mint> ans(n, 0);
    vector<mint> all(n + 1, 0); //(x - x0) * (x - x1) * ... * (x - x(n-1))
    all[0] = 1;
    for (int i = 0; i < n; i++) {
        for (int j = n; j >= 0; j--) {
            all[j] *= -x[i];
            if (j) all[j] += all[j - 1];
        }
    }
    for (int i = 0; i < n; i++) {
        vector <mint> up(n); //all / (x - xi)
        mint rem = all[n];
        for (int j = n - 1; j >= 0; --j) {
            up[j] = rem;
            rem = all[j] + rem * x[i];
        }
        mint down = 1;
        for (int j = 0; j < n; j++) if (i != j) down *= x[i] - x[j];
        up.resize(n); down = down.inv() * y[i];
        for (int j = 0; j < n; j++) ans[j] += up[j] * down;
    }
  return ans;
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, q; cin >> n >> q;
  vector<vector<mint>> a(n, vector<mint>(n));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      cin >> a[i][j];
    }
  }
  a = Hessenberg(a);
  vector<mint> x(n + 1), y(n + 1);
  for (int i = 0; i <= n; i++) {
    x[i] = i + 1;
    y[i] = yo(a, x[i]);
  }
  auto p = Lagrange(x, y); // characteristic polynomial
  while (q--) {
    mint x; cin >> x;
    mint ans = 0, cur = 1;
    for (int i = 0; i <= n; i++) {
      ans += cur * p[i];
      cur *= x;
    }
    cout << ans << ' ';
  }
  return 0;
}

// https://codeforces.com/gym/102984/problem/K
// https://rkm0959.tistory.com/141?category=743276
