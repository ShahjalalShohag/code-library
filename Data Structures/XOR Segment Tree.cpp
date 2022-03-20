#include<bits/stdc++.h>
using namespace std;

const int N = 2e5 + 9;

using T = long long;
T a[N];
struct XORSegmentTree {
  // the length of the array should be 2^LOG for some LOG
  vector<T> t[4 * N]; // O(LOG 2^LOG) memory
  T lazy[4 * N];
  XORSegmentTree() {
    for (int i = 0; i < 4 * N; i++) {
      t[i].clear();
      lazy[i] = 0;
    }
  }
  // b = 0, e = 2^LOG - 1
  // O(LOG 2^LOG)
  void build(int n, int b, int e) {
    lazy[n] = 0;
    t[n].clear();
    if (b == e) {
      t[n].push_back(a[b]);
      return;
    }
    int mid = (b + e) >> 1, l = n << 1, r = l | 1;
    build(l, b, mid);
    build(r, mid + 1, e);
    int len = e - b + 1;
    // t[n][i] = sum of a[p ^ i] over all b <= p <= e
    t[n].resize(len);
    for (int i = 0; i < len; i++) {
      if (i < (len >> 1)) t[n][i] = t[l][i] + t[r][i];
      else t[n][i] = t[r][i - (len >> 1)] + t[l][i - (len >> 1)];
    }
  }
  // add x to a[i], basically the change occurs for all t[n][i] where n is a candidate node
  // so we just create a lazy array to remember the change for each node
  // O(LOG)
  void upd(int n, int b, int e, int i, T x) {
    if (b > i || e < i) return;
    lazy[n] += x;
    if (b == e && b == i) {
      return;
    }
    int mid = (b + e) >> 1, l = n << 1, r = l | 1;
    upd(l, b, mid, i, x);
    upd(r, mid + 1, e, i, x);
  }
  // layer = LOG - 1
  // return sum of a[p ^ x] over all i <= p <= j
  // O(LOG)
  T query(int n, int b, int e, int i, int j, int x, int layer) {
    if (i > j or b > j or e < i) return 0;
    if (b >= i and e <= j) return lazy[n] + (layer == -1 ? t[n][0] : t[n][x & ((1 << layer + 1) - 1)]);
    int mid = (b + e) >> 1, l = n << 1, r = l | 1;
    if (~x >> layer & 1) return query(l, b, mid, i, j, x, layer - 1) + query(r, mid + 1, e, i, j, x, layer - 1);
    else {
      int i1 = i, j1 = min(mid, j), i2 = max(i, mid + 1), j2 = j;
      return query(r, mid + 1, e, mid + 1 + i1 - b, mid + 1 + j1 - b, x, layer - 1) +
        query(l, b, mid, b + i2 - (mid + 1), b + j2 - (mid + 1), x, layer - 1);
    }
  }
}t;
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int k = 17, n = (1 << k);
  for (int i = 0; i < n; i++) {
    a[i] = rnd() % 1000000000;
  }
  t.build(1, 0, n - 1);
  int q = 1000;
  while (q--) {
    int l = rnd() % n, r = rnd() % (n - l) + l, x = rnd() % (1 << k);
    int z = rnd() % n, add = rnd() % 100;
    t.upd(1, 0, n - 1, z, add);
    a[z] += add;
    T sum = 0;
    for (int i = l; i <= r; i++) {
      sum += a[i ^ x];
    }
    T res = t.query(1, 0, n - 1, l, r, x, k - 1);
    cout << sum << ' ' << res << '\n';
    assert(sum == res);
  }
  return 0;
}
// relevant problem: https://codeforces.com/contest/1654/problem/F
