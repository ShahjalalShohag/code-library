#include<bits/stdc++.h>
using namespace std;

const int N = 25, mod = 1e9 + 7;

long long power(long long n, long long k) {
    int ans = 1 % mod;
    n %= mod;
    if (n < 0) n += mod;
    while (k) {
        if (k & 1) ans = (long long) ans * n % mod;
        n = (long long) n * n % mod;
        k >>= 1;
    }
    return ans;
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
        up.resize(n);
        down = down.inv() * y[i];
        for (int j = 0; j < n; j++) ans[j] += up[j] * down;
    }
    return ans;
}
long long ncr[N][N], d[N][N]; // N > k1 + k2
void prec() {
    for (int i = 0; i < N; i++) ncr[i][0]=1;
    for (int i = 1; i < N; i++) {
        for(int j = 1; j < N; j++) {
          ncr[i][j] = (ncr[i - 1][j - 1] + ncr[i - 1][j]) % mod;
        }
    }
  for (int k = 0; k < N; k++) {
    vector<mint> y, x;
      mint sum = 0; y.push_back(0); x.push_back(0);
      for (int i = 1; i <= k + 1; i++) {
        sum += mint(i).pow(k);
        y.push_back(sum); x.push_back(i);
      }
      auto p = Lagrange(x, y);
      assert(p.size() > k + 1);
      for (int i = 0; i <= k + 1; i++) d[k][i] = p[i].value;
  }
  d[0][0] = 1; //to satisfy 0^0 = 1
}
long long power_sum(long long n, int k) {
  assert(k >= 0);
  long long ans = 0;
    for (int i = 0; i <= k + 1; i++) {
      ans += d[k][i] * power(n, i) % mod;
      ans %= mod;
    }
    return ans;
}
// \sum{x=0}^{n}{x^k1 * ((ax + b)/c)^k2} //floor division
// 0^0 = 1
// it solves for all _k1, _k2 s.t. _k1 + _k2 <= k1 + k2
// (a * n + b) should fit in long long
// total complexity: O((k1 + k2)^3 * log(n) * log(mod)
vector<vector<long long>> solve(long long k1, long long k2, long long a, long long b, long long c, long long n) {
    if (n < 0) {
      vector<vector<long long>> ret(k1 + k2 + 1, vector<long long> (k1 + k2 + 1, 0));
      return ret;
    }
    if (!a) {
        vector<vector<long long>> ret(k1 + k2 + 1, vector<long long> (k1 + k2 + 1, 0));
        for (int _k1 = 0; _k1 <= k1 + k2; ++_k1) {
            ret[_k1][0] = power_sum(n, _k1);
            for (int _k2 = 1; _k1 + _k2 <= k1 + k2; ++_k2) {
                ret[_k1][_k2] = (ret[_k1][0] * power(b / c, _k2)) % mod;
            }
        }
        return ret;
    }

    vector<vector<long long>> ret(k1 + k2 + 1, vector<long long> (k1 + k2 + 1, 0));
    assert(c != 0);
    if (a >= c) {
        vector<vector<long long>> nxt = solve(k1, k2, a % c, b, c, n);
        for (int _k1 = 0; _k1 <= k1 + k2; ++_k1) {
            ret[_k1][0] = power_sum(n, _k1);
            for (int _k2 = 1; _k1 + _k2 <= k1 + k2; ++_k2) {
                ret[_k1][_k2] = 0;
                for (int y = 0; y <= _k2; ++y) {
                    ret[_k1][_k2] += ((ncr[_k2][y] * power(a / c, y) ) % mod) * nxt[_k1 + y][_k2 - y];
                    ret[_k1][_k2] %= mod;
                }
            }
        }
    }

    else if (b >= c) {
        vector<vector<long long>> nxt = solve(k1, k2, a, b % c, c, n);
        for (int _k1 = 0; _k1 <= k1 + k2; ++_k1) {
            ret[_k1][0] = power_sum(n, _k1);
            for (int _k2 = 1; _k1 + _k2 <= k1 + k2; ++_k2) {
                ret[_k1][_k2] = 0;
                for (int y = 0; y <= _k2; ++y) {
                    ret[_k1][_k2] += ((ncr[_k2][y] * power(b / c, y) ) % mod) * nxt[_k1][_k2 - y];
                    ret[_k1][_k2] %= mod;
                }
            }
        }
    }

    else {
        long long m = (a * n + b) / c;
        vector<vector<long long>> nxt = solve(k1, k2, c, c - b - 1, a, m - 1);
        vector<long long> psum(k1 + k2 + 1), pre(k1 + k2 + 1);
        for (int i = 0; i <= k1 + k2; ++i) psum[i] = power_sum(m - 1, i);
        for (int _k1 = 0; _k1 <= k1 + k2; ++_k1) {
            for (int i = 0; i <= k1 + k2; ++i) {
                pre[i] = 0;
                for (int j = 0; j <= _k1 + 1; ++j) {
                    pre[i] += (d[_k1][j] * nxt[i][j]) % mod;
                    pre[i] %= mod;
                }
            }
            ret[_k1][0] = power_sum(n, _k1);
            for (int _k2 = 1; _k1 + _k2 <= k1 + k2; ++_k2) {
                ret[_k1][_k2] = 0;
                for (int i = 0; i < _k2; ++i) {
                    ret[_k1][_k2] += ( (ncr[_k2][i] * psum[i]) % mod) * ret[_k1][0];
                    ret[_k1][_k2] -= ncr[_k2][i] * pre[i];
                    ret[_k1][_k2] %= mod;
                }
            }
        }
    }
    return ret;
}
int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    prec();
    int t; cin >> t;
    while (t--) {
        long long k1, k2, a, b, c, n; cin >> n >> a >> b >> c >> k1 >> k2;
        auto ret = solve(k1, k2, a, b, c, n);
        cout << ret[k1][k2] << '\n';
    }
    return 0;
}
// https://asfjwd.github.io/2020-04-24-floor-sum-ap/
// https://loj.ac/problem/138
