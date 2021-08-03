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
struct Mat {
  int n, m;
  vector< vector<int> > a;
  Mat() { }
  Mat(int _n, int _m) {n = _n; m = _m; a.assign(n, vector<int>(m, 0)); }
  Mat(vector< vector<int> > v) { n = v.size(); m = n ? v[0].size() : 0; a = v; }
  inline void make_unit() {
    assert(n == m);
    for (int i = 0; i < n; i++)  {
      for (int j = 0; j < n; j++) a[i][j] = i == j;
    }
  }
  inline Mat operator + (const Mat &b) {
    assert(n == b.n && m == b.m);
    Mat ans = Mat(n, m);
    for(int i = 0; i < n; i++) {
      for(int j = 0; j < m; j++) {
        ans.a[i][j] = (a[i][j] + b.a[i][j]) % mod;
      }
    }
    return ans;
  } 
  inline Mat operator - (const Mat &b) {
    assert(n == b.n && m == b.m);
    Mat ans = Mat(n, m);
    for(int i = 0; i < n; i++) {
      for(int j = 0; j < m; j++) {
        ans.a[i][j] = (a[i][j] - b.a[i][j] + mod) % mod;
      }
    }
    return ans;
  }
  inline Mat operator * (const Mat &b) {
    assert(m == b.n);
    Mat ans = Mat(n, b.m);
    for(int i = 0; i < n; i++) {
      for(int k = 0; k < m; k++) {
        for(int j = 0; j < b.m; j++) {
          ans.a[i][j] = (ans.a[i][j] + 1LL * a[i][k] * b.a[k][j] % mod) % mod;
        }
      }
    }
    return ans;
  } 
  inline Mat operator * (int k) {
    Mat ans = *this;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        ans.a[i][j] = 1LL * ans.a[i][j] * k % mod;
      }
    }
    return ans;
  }
  inline Mat pow(long long k) {
    assert(n == m);
    Mat ans(n, n), t = a; ans.make_unit();
    while (k) {
      if (k & 1) ans = ans * t;
      t = t * t;
      k >>= 1;
    }
    return ans;
  }
  inline Mat& operator += (const Mat& b) { return *this = (*this) + b; }
  inline Mat& operator -= (const Mat& b) { return *this = (*this) - b; }
  inline Mat& operator *= (const Mat& b) { return *this = (*this) * b; }
  inline bool operator == (const Mat& b) { return a == b.a; }
  inline bool operator != (const Mat& b) { return a != b.a; }
  void print() {
    cout << "Matrix: \n";
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) cout << a[i][j] << ' ';
      cout << '\n';
    }
    cout << '\n';
  }
};
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());

// Let A be an n * n matrix
// A^0, A^1, ..., A^inf follows a recurrence of degree <= n (check Cayley-Hamilton Theorem)
// Find the smallest recurrence P of degree k such that
// A^z = \sum_{i=0}^{k-1}{A^(z - i - 1) \times P_i}
// M contains (x, y, v) which means
// A[x][y] = v, 0-based and all (x, y) should be distinct
// all other elements are zeroes
struct Elem { int x, y; mint v; };
vector<mint> MatrixRecurrence(int n, vector<Elem> M) {
  vector<mint> rd1, rd2;
  for (int i = 0; i < n; i++) {
    rd1.push_back(1 + rnd() % (mod - 1));
    rd2.push_back(1 + rnd() % (mod - 1));
  }
  vector<mint> v;
  for (int i = 0; i < 2 * n + 2; i++) {
    mint tmp = 0;
    for (int j = 0; j < n; j++) tmp += rd2[j] * rd1[j];
    v.push_back(tmp);
    vector<mint> nxt(n);
    for (auto &e : M) nxt[e.x] += e.v * rd1[e.y];
    rd1 = nxt;
  }
  auto ans = BerlekampMassey(v);
  return ans;
}
// O(n^2 + n*k) where k is the number of non-zero elements
mint Determinant(int n, vector<Elem> M) {
  vector<mint> rd;
  for (int i = 0; i < n; i++) rd.push_back(1 + rnd() % (mod - 1));
  for (auto &e : M) e.v *= rd[e.y];
  auto ans = MatrixRecurrence(n, M).back();
  if (n % 2 == 0) ans *= -1;
  for (auto &x : rd) ans /= x;
  return ans;
}
void verify_matrix_recurrence(int n, Mat a, vector<Elem> M) {
  //cout << "Original "; a.print();
  auto p = MatrixRecurrence(n, M);
  //for (auto x: p) cout << x << ' '; cout << '\n';
  for (int i = p.size(); i <= p.size() + 1; i++) {
    auto PW = (a.pow(i));
    Mat ans(n, n); int sz = p.size();
    for (int j = 0; j < sz; j++) {
      ans = ans + ((a.pow(i - j - 1)) * p[j].value);
    }
    assert(ans == PW);
  }
}
int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, k; cin >> n >> k;
  vector<Elem> M;
  Mat a(n, n);
  for (int i = 0; i < k; i++) {
    int x, y, z; cin >> x >> y >> z;
    M.push_back({x, y, z});
    a.a[x][y] = z;
  }
  //verify_matrix_recurrence(n, a, M);
  cout << Determinant(n, M) << '\n';
  return 0;
}
// https://judge.yosupo.jp/problem/sparse_matrix_det
