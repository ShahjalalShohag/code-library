#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9, mod = 1e9 + 7;

struct base {
  double x, y;
  base() { x = y = 0; }
  base(double x, double y): x(x), y(y) { }
};
inline base operator + (base a, base b) { return base(a.x + b.x, a.y + b.y); }
inline base operator - (base a, base b) { return base(a.x - b.x, a.y - b.y); }
inline base operator * (base a, base b) { return base(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x); }
inline base conj(base a) { return base(a.x, -a.y); }
int lim = 1;
vector<base> roots = {{0, 0}, {1, 0}};
vector<int> rev = {0, 1};
const double PI = acosl(- 1.0);
void ensure_base(int p) {
  if(p <= lim) return;
  rev.resize(1 << p);
  for(int i = 0; i < (1 << p); i++) rev[i] = (rev[i >> 1] >> 1) + ((i & 1)  <<  (p - 1));
  roots.resize(1 << p);
  while(lim < p) {
    double angle = 2 * PI / (1 << (lim + 1));
    for(int i = 1 << (lim - 1); i < (1 << lim); i++) {
      roots[i << 1] = roots[i];
      double angle_i = angle * (2 * i + 1 - (1 << lim));
      roots[(i << 1) + 1] = base(cos(angle_i), sin(angle_i));
    }
    lim++;
  }
}
void fft(vector<base> &a, int n = -1) {
  if(n == -1) n = a.size();
  assert((n & (n - 1)) == 0);
  int zeros = __builtin_ctz(n);
  ensure_base(zeros);
  int shift = lim - zeros;
  for(int i = 0; i < n; i++) if(i < (rev[i] >> shift)) swap(a[i], a[rev[i] >> shift]);
  for(int k = 1; k < n; k <<= 1) {
    for(int i = 0; i < n; i += 2 * k) {
      for(int j = 0; j < k; j++) {
        base z = a[i + j + k] * roots[j + k];
        a[i + j + k] = a[i + j] - z;
        a[i + j] = a[i + j] + z;
      }
    }
  }
}
//eq = 0: 4 FFTs in total
//eq = 1: 3 FFTs in total
vector<int> multiply(vector<int> &a, vector<int> &b, int eq = 0) {
  int need = a.size() + b.size() - 1;
  int p = 0;
  while((1 << p) < need) p++;
  ensure_base(p);
  int sz = 1 << p;
  vector<base> A, B;
  if(sz > (int)A.size()) A.resize(sz);
  for(int i = 0; i < (int)a.size(); i++) {
    int x = (a[i] % mod + mod) % mod;
    A[i] = base(x & ((1 << 15) - 1), x >> 15);
  }
  fill(A.begin() + a.size(), A.begin() + sz, base{0, 0});
  fft(A, sz);
  if(sz > (int)B.size()) B.resize(sz);
  if(eq) copy(A.begin(), A.begin() + sz, B.begin());
  else {
    for(int i = 0; i < (int)b.size(); i++) {
      int x = (b[i] % mod + mod) % mod;
      B[i] = base(x & ((1 << 15) - 1), x >> 15);
    }
    fill(B.begin() + b.size(), B.begin() + sz, base{0, 0});
    fft(B, sz);
  }
  double ratio = 0.25 / sz;
  base r2(0,  - 1), r3(ratio, 0), r4(0,  - ratio), r5(0, 1);
  for(int i = 0; i <= (sz >> 1); i++) {
    int j = (sz - i) & (sz - 1);
    base a1 = (A[i] + conj(A[j])), a2 = (A[i] - conj(A[j])) * r2;
    base b1 = (B[i] + conj(B[j])) * r3, b2 = (B[i] - conj(B[j])) * r4;
    if(i != j) {
      base c1 = (A[j] + conj(A[i])), c2 = (A[j] - conj(A[i])) * r2;
      base d1 = (B[j] + conj(B[i])) * r3, d2 = (B[j] - conj(B[i])) * r4;
      A[i] = c1 * d1 + c2 * d2 * r5;
      B[i] = c1 * d2 + c2 * d1;
    }
    A[j] = a1 * b1 + a2 * b2 * r5;
    B[j] = a1 * b2 + a2 * b1;
  }
  fft(A, sz); fft(B, sz);
  vector<int> res(need);
  for(int i = 0; i < need; i++) {
    long long aa = A[i].x + 0.5;
    long long bb = B[i].x + 0.5;
    long long cc = A[i].y + 0.5;
    res[i] = (aa + ((bb % mod) << 15) + ((cc % mod) << 30))%mod;
  }
  return res;
}
int f[N],inv[N],finv[N];
void pre() {
  f[0]=1;
  for(int i=1;i<N;i++) f[i]=1LL*i*f[i-1]%mod;
  inv[1] = 1;
  for (int i = 2; i <N; i++ ) {
    inv[i] = (-(1LL*mod/i) * inv[mod%i] ) % mod;
    inv[i] = (inv[i] + mod)%mod;
  }
  finv[0]=1;
  for(int i=1;i<N;i++) finv[i]=1LL*inv[i]*finv[i-1]%mod;
}
int ncr(int n,int r) {
  if(n<r||n<0||r<0) return 0;
  return 1LL*f[n]*finv[n-r]%mod*finv[r]%mod;
}
int catalan(int n) {
  return 1LL * inv[n + 1] * ncr(2 * n, n) % mod;
}
int ans[N], a[N], dp[N];
//ans[n] = \sum_{i = 0}^{n-1}{ans[i] * a[n - i]}
void yo(int l, int r) {
  if (l == r) {
    dp[l] = (l ? dp[l - 1] : 0) + ans[l];
    if (dp[l] >= mod) dp[l] -= mod;
    return;
  }
  int mid = l + r >> 1;
  yo(l, mid);
  vector<int> b;
  for (int i = l; i <= r; i++) b.push_back(a[i - l]);
  vector<int> f;
  for (int i = l; i <= mid; i++) f.push_back(dp[i]);
  auto res = multiply(b, f); res.resize(r - l + 2);
  for (int i = mid + 1; i <= r; i++) {
    ans[i] += res[i - l];
    if (ans[i] >= mod) ans[i] -= mod;
  }
  yo(mid + 1, r);
}
int32_t main() {
  pre();
  int n, k; cin >> n >> k;
  k = min(k, n / 2);
  for (int i = 1; i <= 2 * k; i++) a[i] = (i & 1) ? 0 : catalan(i / 2 - 1);
  ans[0] = 1; //initial value
  yo(0, n);
  cout << dp[n] << '\n';
  return 0;
}
// https://csacademy.com/contest/round-9/task/jetpack/statement/
