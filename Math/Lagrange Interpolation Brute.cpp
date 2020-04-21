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

//generates the polynomial explicitely
vector<mint> Lagrange(vector<mint> &x, vector<mint> &y) {
	int n = x.size();
	vector<mint> res(n, 0), tmp(n, 0);
	for(int k = 0; k < n; k++) {
		for(int i = k + 1; i < n; i++) y[i] = (y[i] - y[k]) / (x[i] - x[k]);
	}
	mint last = 0; tmp[0] = 1;
	for(int k = 0; k < n; k++) for(int i = 0; i < n; i++) {
		res[i] += y[k] * tmp[i];
		swap(last, tmp[i]);
		tmp[i] -= last * x[k];
	}
	return res;
}

int32_t main() {
    int n, k; cin >> n >> k;
    vector<mint> y, x;
    mint sum = 0; y.push_back(0); x.push_back(0);
    for (int i = 1; i <= k + 1; i++) {
    	sum += mint(i).pow(k);
    	y.push_back(sum); x.push_back(i);
    }
    auto p = Lagrange(x, y);
    mint ans = 0;
    for (int i = 0; i < p.size(); i++) ans += p[i] * mint(n).pow(i);
    cout << ans << '\n';
    return 0;
}