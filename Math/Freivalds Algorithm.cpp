#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, mod = 1e9 + 7;

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
};
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
// check if two n*n matrix a*b=c within complexity (iteration*n^2)
// probability of error 2^(-iteration)
int Freivalds(Mat a, Mat b, Mat c) {
  int n = a.n, iteration = 40;
  Mat zero(n, 1), r(n, 1);
  while (iteration--) {
    for(int i = 0; i < n; i++) r.a[i][0] = rnd() % 2;
    Mat ans = (a * (b * r)) - (c * r);
    if(ans != zero) return 0;
  }
  return 1;
}
int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, q; cin >> n >> q;
  Mat a(n, n), c(n, n);
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++) cin >> a.a[i][j];
  }
  while (q--) {
    int ty; cin >> ty;
    if (ty == 1) {
      int x1, y1, x2, y2, v; cin >> x1 >> y1 >> x2 >> y2 >> v;
      for(int i = x1; i <= x2; i++) for(int j = y1; j <= y2; j++) a.a[i][j] += v;
    }
    else {
      for(int i = 0; i < n; i++) for(int j = 0; j < n; j++) cin >> c.a[i][j];
      cout << (Freivalds(a, a, c) ? "yes\n" : "no\n");
    }
  }
  return 0;
}
