#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;

const double PI = acos(-1);
struct base {
  double a, b;
  base(double a = 0, double b = 0) : a(a), b(b) {}
  const base operator + (const base &c) const {
    return base(a + c.a, b + c.b);
  }
  const base operator - (const base &c) const {
    return base(a - c.a, b - c.b);
  }
  const base operator * (const base &c) const {
    return base(a * c.a - b * c.b, a * c.b + b * c.a);
  }
};
void fft(vector<base> &p, bool inv = 0) {
  int n = p.size(), i = 0;
  for(int j = 1; j < n - 1; ++j) {
    for(int k = n  >>  1; k > (i ^= k); k  >>= 1);
    if(j < i) swap(p[i], p[j]);
  }
  for(int l = 1, m; (m = l << 1) <= n; l <<= 1) {
    double ang = 2 * PI / m;
    base wn = base(cos(ang), (inv ? 1. : -1.) * sin(ang)), w;
    for(int i = 0, j, k; i < n; i += m) {
      for(w = base(1, 0), j = i, k = i + l; j < k; ++j, w = w * wn) {
        base t = w * p[j + l];
        p[j + l] = p[j] - t;
        p[j] = p[j] + t;
      }
    }
  }
  if(inv) for(int i = 0; i < n; ++i) p[i].a /= n, p[i].b /= n;
}
vector<long long> multiply(vector<int> &a, vector<int> &b) {
  int n = a.size(), m = b.size(), t = n + m - 1, sz = 1;
  while(sz < t) sz <<= 1;
  vector<base> x(sz), y(sz), z(sz);
  for(int i = 0 ; i < sz; ++i) {
    x[i] = i < (int)a.size() ? base(a[i], 0) : base(0, 0);
    y[i] = i < (int)b.size() ? base(b[i], 0) : base(0, 0);
  }
  fft(x), fft(y);
  for(int i = 0; i < sz; ++i) z[i] = x[i] * y[i];
  fft(z, 1);
  vector<long long> ret(sz);
  for(int i = 0; i < sz; ++i) ret[i] = z[i].a + 0.5;
  while((int)ret.size() > 1 && ret.back() == 0) ret.pop_back();
  return ret;
}

vector<int> g[N];
int sz[N];
int tot, done[N], cenpar[N];
void calc_sz(int u, int p) {
  tot ++;
  sz[u] = 1;
  for (auto v : g[u]) {
    if(v == p || done[v]) continue;
    calc_sz(v, u);
    sz[u] += sz[v];
  }
}
int find_cen(int u, int p) {
  for (auto v : g[u]) {
    if(v == p || done[v]) continue;
    else if(sz[v] > tot / 2) return find_cen(v, u);
  }
  return u;
}
vector<int> vec;
void dfs(int u, int pre, int len) {
  vec.push_back(len);
  for(auto v : g[u]) {
    if(v == pre || done[v]) continue;
    dfs(v, u, len + 1);
  }
}
long long ans[N]; // number of paths of length i = ans[i]
void solve(int u, int pre, int len, int add) {
  vec.clear();
  dfs(u, pre, len);
  int mx = 0;
  for(auto x : vec) mx = max(mx, x);
  vector<int> a(mx + 1, 0);
  for(auto x : vec) a[x]++;
  vector<long long> res = multiply(a, a);
  for(int i = 0; i < res.size(); i++) ans[i] += res[i] * add;
}
void decompose(int u, int pre) {
  tot = 0;
  calc_sz(u, pre);
  int cen = find_cen(u, pre);
  cenpar[cen] = pre;
  done[cen] = 1;
  solve(cen, pre, 0, 1);
  for(auto v : g[cen]) {
    if(v == pre || done[v]) continue;
    solve(v, cen, 1, -1);
  }
  for(auto v : g[cen]) {
    if(v == pre || done[v]) continue;
    decompose(v, cen);
  }
}
int main() {
  freopen("awesome.in", "r", stdin);
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t;
  cin >> t;
  while (t--) {
    int n, l, r;
    cin >> n >> l >> r;
    l = n - 1 - l;
    r = n - 1 - r;
    swap(l, r);
    for (int i = 1; i < n; i++) {
      int u, v;
      cin >> u >> v;
      g[u].push_back(v);
      g[v].push_back(u);
    }
    decompose(1, 0);
    ans[0] = 0;
    long long res = 0;
    for(int i = l; i <= r; i++) res += ans[i] / 2;
    cout << res << '\n';
    for (int i = 1; i <= n; i++) g[i].clear(), done[i] = 0;
    for (int i = 0; i < n; i++) ans[i] = 0;
  }
  return 0;
}
// https://codeforces.com/gym/101991/problem/A
