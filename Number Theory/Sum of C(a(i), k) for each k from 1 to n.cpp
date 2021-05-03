#include<bits/stdc++.h>
using namespace std;
 
const int N = 2e5 + 9, mod = 924844033;
 
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
int power(long long n, long long k) {
  int ans = 1 % mod; n %= mod; if (n < 0) n += mod;
  while (k) {
    if (k & 1) ans = (long long) ans * n % mod;
    n = (long long) n * n % mod;
    k >>= 1;
  }
  return ans;
}
vector<int> g[N];
int sz[N];
void dfs(int u, int p = 0) {
  sz[u] = 1;
  for (auto v: g[u]) {
    if (v == p) continue;
    dfs(v, u);
    sz[u] += sz[v];
  }
}
int f[N], ifac[N];
// ans_k = sum_(C(a_i, k))
vector<int> yo(int n, vector<int> a) {
  vector<int> cnt(N, 0);
  for (auto x: a) cnt[x]++;
  for (int i = 0; i < N; i++) cnt[i] = 1LL * cnt[i] * f[i] % mod;
  vector<int> b(N, 0);
  for (int i = 0; i < N ; i++) b[i] = ifac[i];
  reverse(b.begin(), b.end());
  auto ans = multiply(cnt, b);
  ans.erase(ans.begin(), ans.begin() + N - 1);
  ans.resize(n + 1);
  for (int i = 0; i <= n; i++) ans[i] = 1LL * ans[i] * power(f[i], mod - 2) % mod;
  return ans;
}
int ncr(int n, int k) {
  return 1LL * f[n] * ifac[k] % mod * ifac[n - k] % mod;
}
int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  f[0] = 1;
  for (int i = 1; i < N; i++) f[i] = 1LL * i * f[i - 1] % mod;
  for (int i = 0; i < N; i++) ifac[i] = power(f[i], mod - 2);
  int n; cin >> n;
  for (int i = 1; i < n; i++) {
    int u, v; cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  dfs(1);
  vector<int> a;
  for (int i = 2; i <= n; i++) a.push_back(sz[i]);
  vector<int> ans(n + 1, 0);
  for (int i = 1; i <= n; i++) ans[i] = 1LL * ncr(n, i) * n % mod;
  auto p = yo(n, a);
  for (int i = 1; i <= n; i++) ans[i] =(ans[i] - p[i] + mod) % mod;
  for (auto &x: a) x = n - x;
  p = yo(n, a);
  for (int i = 1; i <= n; i++) ans[i] =(ans[i] - p[i] + mod) % mod;
  for (int i = 1; i <= n; i++) cout << ans[i] << '\n';
  return 0;
}
// https://atcoder.jp/contests/agc005/tasks/agc005_f