#include<bits/stdc++.h>
using namespace std;

const int N = 205;

int sign(int x) { return (x > 0) - (x < -0); }
struct PT {
  int x, y;
  PT() { x = 0, y = 0; }
  PT(int x, int y) : x(x), y(y) {}
  PT(const PT &p) : x(p.x), y(p.y)    {}
  PT operator + (const PT &a) const { return PT(x + a.x, y + a.y); }
  PT operator - (const PT &a) const { return PT(x - a.x, y - a.y); }
  PT operator * (const int a) const { return PT(x * a, y * a); }
  friend PT operator * (const int &a, const PT &b) { return PT(a * b.x, a * b.y); }
  PT operator / (const int a) const { return PT(x / a, y / a); }
  bool operator == (PT a) const { return sign(a.x - x) == 0 && sign(a.y - y) == 0; }
  bool operator != (PT a) const { return !(*this == a); }
  bool operator < (PT a) const { return sign(a.x - x) == 0 ? y < a.y : x < a.x; }
  bool operator > (PT a) const { return sign(a.x - x) == 0 ? y > a.y : x > a.x; }
  int norm2() { return x * x + y * y; }
  double arg() { return atan2(y, x); }
};
inline int dot(PT a, PT b) { return a.x * b.x + a.y * b.y; }
inline int dist2(PT a, PT b) { return dot(a - b, a - b); }
inline int cross(PT a, PT b) { return a.x * b.y - a.y * b.x; }
inline int orientation(PT a, PT b, PT c) { return sign(cross(b - a, c - a)); }

int n;
PT p[N];
bool half(PT p) {
  return p.y > 0 || (p.y == 0 && p.x < 0);
}
double dp[N][N];
vector<pair<int, int>> v;
bool yo(double x) {
  for (int l = 1; l <= n; l++) {
    for (int r = 1; r <= n; r++) {
      dp[l][r] = -1e9;
    }
  }
  for (auto [mid, r]: v) {
    double cur = cross(p[mid], p[r]) * 0.5 - (p[r] - p[mid]).norm2() * x;
    for (int l = 1; l <= n; l++) {
      dp[l][r] = max(dp[l][r], (l == mid ? 0 : dp[l][mid]) + cur);
    }
  }
  for (int i = 1; i <= n; i++) {
    if (dp[i][i] >= 0) {
      return true;
    }
  }
  return false;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t; cin >> t;
  while (t--) {
    cin >> n;
    for (int i = 1; i <= n; i++) {
      cin >> p[i].x >> p[i].y;
    }
    v.clear();
    for (int i = 1; i <= n; i++) {
      for (int j = 1; j <= n; j++) {
        if (i != j) v.push_back({i, j});
      }
    }
    sort(v.begin(), v.end(), [&](pair<int, int> x, pair<int, int> y) {
      PT a = p[x.second] - p[x.first], b = p[y.second] - p[y.first];
      return make_tuple(half(a), 0, a.norm2()) < make_tuple(half(b), cross(a, b), b.norm2());
    });
    double l = 0, r = 1e7, ans = 0;
    int it = 70;
    while (it--) {
      double mid = (l + r) / 2;
      if (yo(mid)) {
        ans = mid;
        l = mid;
      }
      else {
        r = mid;
      }
    }
    cout << fixed << setprecision(10) << ans << '\n';
  }
  return 0;
}
// https://toph.co/p/profitable-home
