#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;

// Credit: Benq
// returns vector of {vertex, id of edge to vertex}
// the second element of the first pair is always -1
template<int N, bool directed> struct Euler {
  vector<pair<int, int>> adj[N];
  vector<pair<int, int>>::iterator iter[N];
  bool in_vertex[N];
  vector<int> nodes;
  vector<bool> used;
  Euler() {
    for (int i = 0; i < N; i++) in_vertex[i] = 0;
  }
  vector<int> ans;
  void clear() {
    for (auto &t : nodes) adj[t].clear(), in_vertex[t] = 0;
    nodes.clear();
    used.clear();
    ans.clear();
  }
  void add(int x) {
    if (in_vertex[x]) return;
    in_vertex[x] = 1;
    nodes.push_back(x);
  }
  void add_edge(int a, int b) {
    int m = used.size();
    used.push_back(0);
    add(a);
    add(b);
    adj[a].emplace_back(b, m);
    if (!directed) adj[b].emplace_back(a, m);
  }
  void go(int src) {
    vector<pair<pair<int, int>, int>> ret, s = {{{src, -1}, -1}};
    // {{vertex, prev vertex}, edge label}
    while (s.size()) {
      int x = s.back().first.first;
      auto& it = iter[x], en = end(adj[x]);
      while (it != en && used[it->second]) it ++;
      if (it == en) { // no more edges out of vertex
        if ((int)ret.size() && ret.back().first.second != x) exit(5);
        ret.push_back(s.back()), s.pop_back();
      } else {
        s.push_back({{it->first, x}, it->second});
        used[it->second] = 1;
      }
    }
    for (int i = 0; i < (int)ret.size() - 1; i++) ans.push_back(ret[i].second);
    assert((int)ans.size() % 2 == 0);
  }
  array<vector<int>, 2> tour() {
    for (auto &v : nodes) {
      assert(adj[v].size() % 2 == 0);
      iter[v] = begin(adj[v]);
    }
    for (auto &v : nodes) for (auto &e : adj[v]) if (!used[e.second]) go(v);
    array<vector<int>, 2> res;
    for (int i = 0; i < (int)ans.size(); i++) res[i % 2].push_back(ans[i]);
    return res;
  }
};

typedef array<int, 2> T;
struct EdgeColoring {
  int n;
  vector<T> ed;
  Euler<N * 2, 0> E; // at least 2 * n
  array<vector<int>, 2> split(vector<int> lab) { // k is even, split into two parts
    E.clear();
    for (auto &t : lab) E.add_edge(ed[t][0], ed[t][1]);
    auto v = E.tour(); // get half edges on each
    for (int i = 0; i < 2; i++) for (auto &t : v[i]) t = lab[t];
    return v;
  }
  vector<int> match(vector<int> lab) { // find perfect matching in MlogM
    assert((int)lab.size() && (int)lab.size() % n == 0);
    int k = (int)lab.size() / n;
    int p = 0;
    while ((1 << p) < n * k) p ++;
    int a = (1 << p) / k;
    int b = (1 << p) - k * a;
    vector<int> cnt_good((int)lab.size(), a), cnt_bad(n, b); // now each edge is adjacent to 2^t
    for (; p; --p) { // divide by two!!
      E.clear();
      vector<int> tmp;
      for (int i = 0; i < n * k; i++) {
        if (cnt_good[i] & 1) E.add_edge(ed[lab[i]][0], ed[lab[i]][1]), tmp.push_back(i);
        cnt_good[i] /= 2;
      }
      int num_lab = tmp.size();
      for (int i = 0; i < n; i++) {
        if (cnt_bad[i] & 1) E.add_edge(i, n + i), tmp.push_back(i);
        cnt_bad[i] /= 2;
      }
      array<vector<int>, 2> x = E.tour();
      T cnt = T();
      for (int i = 0; i < 2; i++) for (auto &t : x[i]) cnt[i] += t >= num_lab;
      if (cnt[0] > cnt[1]) swap(x[0], x[1]);
      for (auto &t : x[0]) {
        if (t < num_lab) cnt_good[tmp[t]] ++;
        else cnt_bad[tmp[t]] ++;
      }
    }
    vector<int> v;
    for (int i = 0; i < lab.size(); i++) if (cnt_good[i]) v.push_back(lab[i]);
    assert((int)v.size() == n);
    return v;
  }
  vector<bool> used;
  vector<vector<int>> edge_color(vector<int> lab) { // regular bipartite graph!
    assert((int)lab.size() % n == 0);
    int k = (int)lab.size() / n;
    if (k == 0) return {};
    if (k == 1) return {lab};
    if ( __builtin_popcount(k) == 1) {
      array<vector<int>, 2> p = split(lab);
      vector<vector<int>> a = edge_color(p[0]), b = edge_color(p[1]);
      a.insert(end(a), b.begin(), b.end());
      return a;
    }
    if (k % 2 == 0) {
      array<vector<int>, 2> p = split(lab);
      auto a = edge_color(p[0]);
      int cur = k / 2;
      while ( __builtin_popcount(cur) > 1) {
        cur ++;
        p[1].insert(end(p[1]), a.back().begin(), a.back().end());
        a.pop_back();
      }
      auto b = edge_color(p[1]);
      a.insert(end(a), b.begin(), b.end());
      return a;
    } else {
      vector<int> v = match(lab);
      for (auto &t : v) used[t] = 1;
      vector<int> LAB;
      for (auto &t : lab) if (!used[t]) LAB.push_back(t);
      for (auto &t : v) used[t] = 0;
      auto a = edge_color(LAB);
      a.push_back(v);
      return a;
    }
  }
  // returns edge chromatic number, ans contains the edge coloring(colors are 1 indexed)
  // supports multiple edges
  // 0 indexed, O(M log M)
  int solve(vector<T> _ed, vector<int> &ans) {
    T side = T();
    for (auto &t : _ed) for (int i = 0; i < 2; i++) side[i] = max(side[i], t[i] + 1);
    vector<int> deg[2], cmp[2], sz[2];
    for (int i = 0; i < 2; i++) deg[i].resize(side[i]), cmp[i].resize(side[i]);
    for (auto &t : _ed) for (int i = 0; i < 2; i++) deg[i][t[i]] ++;
    int k = 0;
    for (int i = 0; i < 2; i++) for (auto &t : deg[i]) k = max(k, t);
    for (int s = 0; s < 2; s++) {
      for (int i = 0; i < side[s]; ) {
        sz[s].push_back(0);
        while (i < side[s] && sz[s].back() + deg[s][i] <= k) {
          cmp[s][i] = (int)sz[s].size() - 1;
          sz[s].back() += deg[s][i++];
        }
      }
    }
    for (int i = 0; i < 2; i++) while (sz[i].size() < sz[i ^ 1].size()) sz[i].push_back(0);
    n = sz[0].size();
    for (auto &t : _ed) ed.push_back({cmp[0][t[0]], n + cmp[1][t[1]]});
    int ind = 0;
    for (int i = 0; i < n; i++) {
      while (sz[0][i] < k) {
        while (sz[1][ind] == k) ind ++;
        sz[0][i] ++, sz[1][ind] ++;
        ed.push_back({i, n + ind});
      }
    }
    used.resize(n * k);
    vector<int> lab(n * k);
    iota(lab.begin(), lab.end(), 0);
    auto tmp = edge_color(lab);
    ans.resize(_ed.size());
    for (int i = 0; i < tmp.size(); i++) {
      for (auto x : tmp[i]) if (x < _ed.size()) ans[x] = i + 1;
    }
    return tmp.size();
  }
};

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int l, r, m;
  cin >> l >> r >> m;
  vector<T> ed(m);
  for (auto &x : ed) cin >> x[0] >> x[1];
  EdgeColoring E;
  vector<int> ans;
  int cnt = E.solve(ed, ans);
  cout << cnt << '\n';
  for (auto &x : ans) cout << x - 1 << '\n';
  return 0;
}
// https://judge.yosupo.jp/problem/bipartite_edge_coloring
