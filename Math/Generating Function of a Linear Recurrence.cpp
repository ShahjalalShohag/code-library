#include<bits/stdc++.h>
using namespace std;

const int N = 3e3 + 9, mod = 998244353;

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
template <int32_t MOD> istream & operator >> (istream & in, modint<MOD> &n) { return in >> n.value; }
template <int32_t MOD> ostream & operator << (ostream & out, modint<MOD> n) { return out << n.value; }

using mint = modint<mod>;

vector<mint> BerlekampMassey(vector<mint> S) {
  int n = (int)S.size(), L = 0, m = 0;
  vector<mint> C(n), B(n), T;
  C[0] = B[0] = 1;
  mint b = 1;
  for(int i = 0; i < n; i++) {
        ++m; mint d = S[i];
    for(int j = 1; j <= L; j++) d += C[j] * S[i - j];
    if (d == 0) continue;
    T = C; mint coef = d * b.inv();
    for(int j = m; j < n; j++) C[j] -= coef * B[j - m];
    if (2 * L > i) continue;
    L = i + 1 - L; B = T; b = d; m = 0;
  }
  C.resize(L + 1); C.erase(C.begin());
  for(auto &x: C)  x *= -1;
  return C;
}

// a[k] = c[0] * a[k - 1] + c[1] * a[k - 2] + ... for k >= n
mint yo(vector<mint> a, vector<mint> c) {
  int n = a.size();
  if (!n) return 0;
  vector<mint> p(n + 1, 0);
  p[0] = 1;
  for (int i = 0; i < n; i++) {
    p[i + 1] = -c[i];
  }
  vector<mint> up(n + n, 0);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j <= n; j++) {
      up[i + j] += a[i] * p[j];
    }
  }
  up.resize(n); 
  // generating function of the recurrence is up / p
  // now find its derivative and find the value at x = 1
  mint v = 0;
  for (int i = 0; i < p.size(); i++) {
    v += p[i];
  }
  mint u = 0;
  for (int i = 0; i < up.size(); i++) {
    u += up[i];
  }
  mint du = 0;
  for (int i = 0; i < up.size(); i++) {
    du += up[i] * i;
  }
  mint dv = 0;
  for (int i = 0; i < p.size(); i++) {
    dv += p[i] * i;
  }
  return (v * du - u * dv) / (v * v);
}

mint dp[N * 2][N];
vector<int> g[N];
int deg[N];
mint ideg[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n; cin >> n;
  for (int i = 1; i <= n; i++) {
    int x, y; cin >> x >> y;
  }
  int m; cin >> m;
  while (m--) {
    int u, v; cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
    ++deg[u]; ++deg[v];
  }
  for (int i = 1; i <= n; i++) {
    ideg[i] = mint(deg[i]).inv();
  }
  dp[0][1] = 1;
  for (int k = 1; k <= n * 2; k++) {
    for (int u = 1; u <= n; u++) {
      if (u == n) continue;
      for (auto v: g[u]) {
        dp[k][v] += dp[k - 1][u] * ideg[u];
      }
    }
  }
  vector<mint> cur;
  for (int i = 0; i <= 2 * n; i++) {
    cur.push_back(dp[i][n]);
  }
  auto tr = BerlekampMassey(cur);
  cur.resize(tr.size());
  cout << yo(cur, tr) << '\n';
  return 0;
}
// https://codeforces.com/gym/102268/problem/E