#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;

const int kinds = N;
int str[N];
int buc[N], r[N], suf[N], X[N], Y[N], high[N];
bool cmp(int *r, int a, int b, int x) {
  return (r[a] == r[b] && r[a + x] == r[b + x]);
}
void suffix_array_DA(int n, int m) {
  int *x = X, *y = Y, i, j, k = 0, l;
  for (i = 0; i <= max(n, m); i++) buc[i] = 0;
  for (i = 0; i < n; i++) buc[x[i] = str[i]]++;
  for (i = 1; i < m; i++) buc[i] += buc[i - 1];
  for (i = n - 1; i >= 0; i--) suf[--buc[x[i]]] = i;
  for (l = 1, j = 1; j < n; m = j, l <<= 1) {
    j = 0;
    for (i = n - l; i < n; i++) y[j++] = i;
    for (i = 0; i < n; i++) if(suf[i] >= l) y[j++] = suf[i] - l;
    for (i = 0; i < m; i++) buc[i] = 0;
    for (i = 0; i < n; i++) buc[x[y[i]]]++;
    for (i = 1; i < m; i++) buc[i] += buc[i - 1];
    for (i = n - 1; i >= 0; i--) suf[--buc[x[y[i]]]] = y[i];
    for (swap(x, y), x[suf[0]] = 0, i = 1, j = 1; i < n; i++) {
      x[suf[i]] = cmp(y, suf[i - 1], suf[i], l) ? j - 1 : j++;
    }
  }
  for (i = 1; i < n; i++) r[suf[i]] = i;
  for (i = 0; i < n - 1; high[r[i++]] = k) {
    for (k ? k-- : 0, j = suf[r[i] - 1]; str[i + k] == str[j + k]; k++);
  }
}
struct SuffixArray {
  int n;
  vector<int> s;
  vector<int> sa, rank, lcp;
  static const int LG = 18;
  vector<vector<int>> t;
  vector<int> lg;
  SuffixArray() {}
  SuffixArray(vector<int> _s) {
    n = _s.size();
    s = _s;
    for (int i = 0; i < n; i++) str[i] = s[i]; //for integers, each element should be positive
    str[n] = 0;
    suffix_array_DA(n + 1, kinds);
    for (int i = 1; i <= n; i++) sa.push_back(suf[i]);
    rank.resize(n);
    for (int i = 0; i < n; i++) rank[sa[i]] = i;
    costruct_lcp();
    prec();
    build();
  }
  void costruct_lcp() {
    int k = 0;
    lcp.resize(n - 1, 0);
    for (int i = 0; i < n; i++) {
      if (rank[i] == n - 1) {
        k = 0;
        continue;
      }
      int j = sa[rank[i] + 1];
      while (i + k < n && j + k < n && s[i + k] == s[j + k])  k++;
      lcp[rank[i]] = k;
      if (k)  k--;
    }
  }
  void prec() {
    lg.resize(n, 0);
    for (int i = 2; i < n; i++) lg[i] = lg[i / 2] + 1;
  }
  void build() {
    int sz = n - 1;
    t.resize(sz);
    for (int i = 0; i < sz; i++) {
      t[i].resize(LG);
      t[i][0] = lcp[i];
    }
    for (int k = 1; k < LG; ++k) {
      for (int i = 0; i + (1 << k) - 1 < sz; ++i) {
        t[i][k] = min(t[i][k - 1], t[i + (1 << (k - 1))][k - 1]);
      }
    }
  }
  int query(int l, int r) { // minimum of lcp[l], ..., lcp[r]
    int k = lg[r - l + 1];
    return min(t[l][k], t[r - (1 << k) + 1][k]);
  }
  int get_lcp(int i, int j) { // lcp of suffix starting from i and j
    if (i == j) return n - i;
    int l = rank[i], r = rank[j];
    if (l > r) swap(l, r);
    return query(l, r - 1);
  }
};
struct IsomorphicSuffixArray {
  string s;
  int n;
  vector<int> sa, rank, lcp, a;
  static const int LG = 18;
  vector<vector<int>> t;
  vector<int> lg;
  SuffixArray S;
  IsomorphicSuffixArray() {}
  IsomorphicSuffixArray(string _s) {
    s = _s;
    n = s.size();
    vector<int> last(26, -1);
    a.resize(n);
    for (int i = 0; i < n; i++) {
      a[i] = last[s[i] - 'a'] == -1 ? 1 : i - last[s[i] - 'a'] + 1;
      last[s[i] - 'a'] = i;
    }
    S = SuffixArray(a);
    sa.resize(n);
    iota(sa.begin(), sa.end(), 0);
    sort(sa.begin(), sa.end(),
    [&](const int &i, const int &j) { // O(alpha)
      for (int l = 0; l < n; ) {
        int x = i + l, y = j + l;
        if (x >= n) return true;
        if (y >= n) return false;
        int vx = a[x] > l + 1 ? 1 : a[x];
        int vy = a[y] > l + 1 ? 1 : a[y];
        if (vx != vy) return vx < vy;
        l += max(1, S.get_lcp(x, y));
      }
      return false;
    });
    rank.resize(n);
    for (int i = 0; i < n; i++) rank[sa[i]] = i;
    costruct_lcp();
    prec();
    build();
  }
  int get_lcp_brute(const int &i, const int &j) { // O(alpha)
    for (int l = 0; l < n; ) {
      int x = i + l, y = j + l;
      if (x >= n || y >= n) return l;
      int vx = a[x] > l + 1 ? 1 : a[x];
      int vy = a[y] > l + 1 ? 1 : a[y];
      if (vx != vy) return l;
      l += max(1, S.get_lcp(x, y));
    }
    return n;
  }
  void costruct_lcp() {
    lcp.resize(n - 1, 0);
    for (int i = 0; i < n - 1; i++) {
      lcp[i] = get_lcp_brute(sa[i], sa[i + 1]);
    }
  }
  void prec() {
    lg.resize(n, 0);
    for (int i = 2; i < n; i++) lg[i] = lg[i / 2] + 1;
  }
  void build() {
    int sz = n - 1;
    t.resize(sz);
    for (int i = 0; i < sz; i++) {
      t[i].resize(LG);
      t[i][0] = lcp[i];
    }
    for (int k = 1; k < LG; ++k) {
      for (int i = 0; i + (1 << k) - 1 < sz; ++i) {
        t[i][k] = min(t[i][k - 1], t[i + (1 << (k - 1))][k - 1]);
      }
    }
  }
  int query(int l, int r) { // minimum of lcp[l], ..., lcp[r]
    int k = lg[r - l + 1];
    return min(t[l][k], t[r - (1 << k) + 1][k]);
  }
  int get_lcp(int i, int j) { // lcp of suffix starting from i and j
    if (i == j) return n - i;
    int l = rank[i], r = rank[j];
    if (l > r) swap(l, r);
    return query(l, r - 1);
  }
  // occurrences of s[p, ..., p + len - 1]
  pair<int, int> find_occurrence(int p, int len) {
    p = rank[p];
    pair<int, int> ans = {p, p};
    int l = 0, r = p - 1;
    while (l <= r) {
      int mid = l + r >> 1;
      if (query(mid, p - 1) >= len) ans.first = mid, r = mid - 1;
      else l = mid + 1;
    }
    l = p + 1, r = n - 1;
    while (l <= r) {
      int mid = l + r >> 1;
      if (query(p, mid - 1) >= len) ans.second = mid, l = mid + 1;
      else r = mid - 1;
    }
    return ans;
  }
};
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, q;
  cin >> n >> q;
  string s;
  cin >> s;
  IsomorphicSuffixArray sa(s);
  while (q--) {
    int l, r;
    cin >> l >> r;
    --l, --r;
    auto ans = sa.find_occurrence(l, r - l + 1);
    cout << ans.second - ans.first + 1 << '\n';
  }
  return 0;
}
// https://www.hackerrank.com/challenges/similar-strings/problem
