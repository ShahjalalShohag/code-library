#include<bits/stdc++.h>
using namespace std;

const int N = 1010, mod = 1e9 + 7;

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

struct ST {
  struct Int { // arithmetic progression a, a + d, a + 2 * d, ...
    mint a = 0, d = 0;
    Int() {};
    Int(mint _a, mint _d) {a = _a, d = _d;}
    mint at(mint n) {return (a + (n - 1) * d);}
    Int shift(mint n) {return Int((a + (n - 1) * d), d);}
    mint get_sum(mint n) {return (((a * 2) + (n - 1) * d) * n) * ((mod + 1) / 2);}
    const Int operator + (const Int &b) const {
      return Int(a + b.a, d + b.d);
    }
  };
  mint t[N << 2];
  Int lazy[N << 2];
  ST() {
    memset(t, 0, sizeof t);
    memset(lazy, 0, sizeof lazy);
  }
  void push(int n, int b, int e) {
    if (lazy[n].a == 0 && lazy[n].d == 0) return;
    t[n] += lazy[n].get_sum(e - b + 1);
    if (b != e) {
      lazy[n << 1] = lazy[n << 1] + lazy[n];
      lazy[n << 1 | 1] = lazy[n << 1 | 1] + lazy[n].shift(((b + e) >> 1) + 2 - b);
    }
    lazy[n] = Int(0, 0);
  }
  void build(int n, int b, int e) {
    lazy[n] = Int(0, 0);
    if (b == e) {
      t[n] = 0;
      return;
    }
    int m = b + e >> 1, l = n << 1, r = n << 1 | 1;
    build(l, b, m);
    build(r, m + 1, e);
    t[n] = t[l] + t[r];
  }
  void upd(int n, int b, int e, int i, int j, pair<mint, mint> v) {
    push(n, b, e);
    if (b > j || e < i) return;
    if (i <= b && e <= j) {
      Int temp(v.first, v.second);
      lazy[n] = lazy[n] + temp.shift(b - i + 1);
      push(n, b, e);
      return;
    }
    int m = b + e >> 1, l = n << 1, r = n << 1 | 1;
    upd(l, b, m, i, j, v);
    upd(r, m + 1, e, i, j, v);
    t[n] = t[l] + t[r];
  }
  mint query(int n, int b, int e, int i, int j) {
    push(n, b, e);
    if (e < i || b > j) return 0;
    if (i <= b && e <= j) return t[n];
    int m = b + e >> 1, l = n << 1, r = n << 1 | 1;
    return query(l, b, m, i, j) + query(r, m + 1, e, i, j);
  }
};

int n;
int C(int i) {
  return i * (i + 1) / 2;
}
mint g[N][N];
ST t;
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int k; cin >> n >> k;
  vector<mint> a(n + 1);
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }
  for (int w = 0; w < k; w++) {
    t.build(1, 1, n);
    for (int i = 1; i <= n; i++) {
      t.upd(1, 1, n, i, i, {a[i] * (C(i - 1) + C(n - i)), 0});
      if (i * 2 <= n) {
        int j = n - i + 1;
        t.upd(1, 1, n, j, n, {a[i] * (n - j + 1), mint(mod) - a[i]});
        if (i < j) t.upd(1, 1, n, i, j - 1, {a[i] * i, 0});

        if (i > 1) t.upd(1, 1, n, 1, i - 1, {a[i], a[i]});
      }
      else {
        if (i < n) t.upd(1, 1, n, i + 1, n, {a[i] * (n - i), mint(mod) - a[i]});

        int j = n - i + 1;
        t.upd(1, 1, n, 1, j, {a[i], a[i]});
        if (j < i) t.upd(1, 1, n, j + 1, i, {a[i] * (n - i + 1), 0});
      }
    }
    for (int i = 1; i <= n; i++) {
      a[i] = t.query(1, 1, n, i, i);
    }
  }
  for (int i = 1; i <= n; i++) {
    cout << a[i] / mint(C(n)).pow(k) << ' ';
  }
  cout << '\n';
  return 0;
}

// https://www.codechef.com/problems/RVRSE