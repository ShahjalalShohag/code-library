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

struct Hafnian {
    int h;
    void add(vector<mint>& ans, const vector<mint>& a, const vector<mint>& b) {
        for (int i = 0; i < h; i++) {
        	for (int j = 0; j < h - 1 - i; j++) ans[i + j + 1] += a[i] * b[j];
        }
    }
    vector<mint> yo(const vector<vector<vector<mint>>>& v) {
        vector<mint> ans(h);
        if (!(int)v.size()) {
            ans[0] = 1; return ans;
        }
        int m = (int)v.size() - 2;
        auto V = v; V.resize(m);
        vector<mint> zero = yo(V);
        for (int i = 0; i < m; i++) {
			for (int j = 0; j < i; j++) {
            	add(V[i][j], v[m][i], v[m + 1][j]);
            	add(V[i][j], v[m + 1][i], v[m][j]);
            }
        }
        vector<mint> one = yo(V);
        for (int i = 0; i < h; i++) ans[i] += one[i] - zero[i];
        add(ans, one, v[m + 1][m]);
        return ans;
    }
    // Number of Perfect Matchings of a general graph
    // SQRT of the Permanent of a zero diagonal symmetric matrix
    // Takes ~2s for (38 * 38) matrix
    mint solve(vector<vector<mint>> a) {
        int n = a.size();
        assert(n % 2 == 0);
        h = n / 2 + 1;
        vector<vector<vector<mint>>> v(n);
        for (int i = 0; i < n; i++) {
            v[i].resize(i);
           	for (int j = 0; j < i; j++) {
           		v[i][j] = vector<mint>(h), v[i][j][0] = a[i][j];
           	}
        }
        return yo(v).back();
    }
}H;
int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n; cin >> n;
    vector<vector<mint>> a(n, vector<mint>(n));
    for (int i = 0; i < n; i++) {
    	for (int j = 0; j < n; j++) cin >> a[i][j];
    }
	cout << H.solve(a) << '\n';
    return 0;
}
// https://judge.yosupo.jp/problem/hafnian_of_matrix
// https://dl.acm.org/doi/pdf/10.5555/2095116.2095189
