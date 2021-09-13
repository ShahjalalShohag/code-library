#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, mod = 998244353;

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
};
template <int32_t MOD> modint<MOD> operator * (int64_t value, modint<MOD> n) { return modint<MOD>(value) * n; }
template <int32_t MOD> modint<MOD> operator * (int32_t value, modint<MOD> n) { return modint<MOD>(value % MOD) * n; }
template <int32_t MOD> ostream & operator << (ostream & out, modint<MOD> n) { return out << n.value; }

using mint = modint<mod>;

/*The chromatic polynomial P(x), is the number of ways to color a graph in <= x colors
trees with n nodes = x * (x - 1) ^ (n - 1)
Cycle with n nodes = (x - 1) ^ n + (-1) ^ (n) * (x-1)
For chromatic number check the smallest k such that P(k) != 0. For high probability compute P(k) under different modulo*/

const int V = 18;
struct ChromaticPolynomial {
  int n;
  mint nCr(int n, int k) {
    if(n < 0) return 0;
    if(n - k < k) k = n - k;
    if(k < 0) return 0;
    mint ret = 1;
    for(int i = 0; i < k; ++i) ret *= n - i, ret /= i + 1;
    return ret;
  }

  void bitflip(vector<mint> &v) {
    for(int i = 1, j = 0; i < n; ++i) {
      int m = n >> 1;
      for(; j >= m; m >>= 1) j -= m;
      j += m;
      if(i < j) swap(v[i], v[j]);
    }
  }
  // fast mobius transform.
  // turns or convolution into point-wise multiplication
  void fmt(vector<mint> &vec, bool inv) {
    int n = vec.size();
    for(int iter = 1, sh = 0; iter < n; iter *= 2, ++sh) {
      for(int x = 0; x < n; x += 2 * iter) {
        for(int y = 0; y < iter; ++y) {
          mint v = vec[x + y], w = vec[x + y + iter];
          vec[x + y + iter] = inv ? w - v : v + w;
        }
      }
    }
  }
  // O(n^3 2^n)
  // Let P(x) = chromatic polynomial of g. it returns {P(0), P(1), ... P(n)}
  vector<mint> polynomial(vector<bitset<V>> g) {
    if(!n) {
      vector<mint> ans(n + 1);
      fill(ans.begin(), ans.end(), 1);
      return ans;
    }
    vector<vector<mint>> f(n + 1, vector<mint>(1 << n));
    for(int m = 1; m < (1 << n); ++m) {
      bitset<V> bs(0);
      bool fail = 0;
      for(int i = 0; i < n; ++i) {
        if((m >> i) & 1) {
          if(bs[i]) fail = 1;
          bs |= g[i];
        }
      }
      if(!fail) f[__builtin_popcount(m)][m] = 1;
    }

    for(auto &e : f) fmt(e, 0);
    vector<vector<mint>> f_cur(n + 1, vector<mint>(1 << n));
    auto f_next = f_cur;
    f_cur[0][0] = 1;
    fmt(f_cur[0], 0);
    vector<mint> ans(n + 1, 0);
    vector<mint> tmp(1 << n);
    for(int i = 0; i < n; ++i) {
      for(int j = 0; j <= n; ++j) {
        auto &e = f_next[j];
        fill(tmp.begin(), tmp.end(), 0);
        for(int k = 0; k <= j; ++k) {
          for(int l = 0; l < (1 << n); ++l) tmp[l] += f_cur[j - k][l] * f[k][l];
        }
        for(int l = 0; l < (1 << n); ++l) e[l] = tmp[l];
      }
      f_next.swap(f_cur);
      auto cnts = f_cur[n];
      fmt(cnts, 1);
      for(int j = 0; j <= n; ++j) ans[j] += nCr(j, i + 1) * cnts.back();
    }
    return ans;
  }
  vector<mint> p;
  void init(vector<bitset<V>> g) {
    n = g.size();
    p = polynomial(g);
  }
  mint get(long long c) { //returns P(c)
    if(c >= 0 && c < (int)p.size()) return p[c];
    c = (c % mod + mod) % mod;
    // polynomial interpolation
    mint ans = 0;
    for(int i = 0; i <= n; ++i) {
      mint mul = p[i];
      for(int j = 0; j <= n; ++j) if(j != i) {
          mul *= mint(c) - mint(j);
          mul /= mint(i) - mint(j);
        }
      ans += mul;
    }
    return ans;
  }
} C;

vector<int> g[N];
int32_t main() {
  int n, m;
  cin >> n >> m;
  for(int i = 1; i <= m; i++) {
    int u, v;
    cin >> u >> v;
    --u; --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  vector<bitset<V>> e(n);
  for(int u = 0; u < n; u++) for(auto v : g[u]) e[u][v] = 1;
  C.init(e);
  mint ans = C.get(-1);
  ans *= n & 1 ? -1 : 1; //number of DAG
  ans *= m;
  ans /= 2;
  cout << ans << '\n';
  return 0;
}
// https://codeforces.com/contest/1193/submission/57949255
