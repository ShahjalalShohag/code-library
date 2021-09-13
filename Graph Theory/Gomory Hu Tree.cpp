#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

using F = long long;
using W = long long; // types for flow and weight/cost
struct S {
  const int v;  // neighbour
  const int r;  // index of the reverse edge
  F f;      // current flow
  const F cap;  // capacity
  const W cost; // unit cost
  S(int v, int ri, F c, W cost = 0) :
    v(v), r(ri), f(0), cap(c), cost(cost) {}
  inline F res() const {
    return cap - f;
  }
};
struct FlowGraph : vector<vector<S>> {
  FlowGraph(size_t n) : vector<vector<S>>(n) {}
  void add_arc(int u, int v, F c, W cost = 0) {
    auto &t = *this;
    t[u].emplace_back(v, t[v].size(), c, cost);
    t[v].emplace_back(u, t[u].size() - 1, 0, -cost);
  }
  void add_edge(int u, int v, F c, W cost = 0) {
    auto &t = *this;
    t[u].emplace_back(v, t[v].size(), c, cost);
    t[v].emplace_back(u, t[u].size() - 1, c, -cost);
  }
  void clear() {
    for (auto &ed : *this) for (auto &e : ed) e.f = 0LL;
  }
};
//0-indexed
struct Dinic {
  FlowGraph &edges;
  int V, s, t;
  vector<int> l;
  vector<vector<S>::iterator> its; // levels and iterators
  Dinic(FlowGraph &edges, int s, int t) :
    edges(edges), V(edges.size()), s(s), t(t), l(V, -1), its(V) {}
  long long augment(int u, F c) { // we reuse the same iterators
    if (u == t) return c;
    for(auto &i = its[u]; i != edges[u].end(); i++) {
      auto &e = *i;
      if (e.cap > e.f && l[u] < l[e.v]) {
        auto d = augment(e.v, min(c, e.cap - e.f));
        if (d > 0) {
          e.f += d;
          edges[e.v][e.r].f -= d;
          return d;
        }
      }
    }
    return 0;
  }
  long long run() {
    long long flow = 0, f;
    while(true) {
      fill(l.begin(), l.end(), -1);
      l[s] = 0; // recalculate the layers
      queue<int> q;
      q.push(s);
      while(!q.empty()) {
        auto u = q.front();
        q.pop();
        for(auto &&e : edges[u]) if(e.cap > e.f && l[e.v] < 0)
            l[e.v] = l[u] + 1, q.push(e.v);
      }
      if (l[t] < 0) return flow;
      for (int u = 0; u < V; ++u) its[u] = edges[u].begin();
      while ((f = augment(s, 2e9)) > 0) flow += f;///take care of inf
    }
  }
};

/*For a given weighted graph the Gomory-Hu tree has the following properties:
The vertex set of the tree and the graph is the same.
The maximum flow between vertices u and v in the tree(i.e. minimum edge from u to v)
is equal to the maximum flow in the graph.
0-indexed
O(n*maxflow+n^2)*/
struct edge {
  int u, v;
  long long w;
};
struct GomoryHuTree {
  int V;
  vector<int> p, w, c;
  vector<edge> tree;
  void dfs(int u, const FlowGraph &fg) {
    c[u] = 1;
    for (const auto &e : fg[u])
      if (!c[e.v] && e.res())
        dfs(e.v, fg);
  }
  GomoryHuTree(int n, const vector<edge> &ed) : V(n), p(V), w(V), c(V) {
    FlowGraph fg(V);
    for (const edge &e : ed) fg.add_edge(e.u, e.v, e.w);
    p[0] = -1, std::fill(p.begin() + 1, p.end(), 0);
    for (int i = 1; i < V; ++i) {
      w[i] = Dinic(fg, i, p[i]).run();
      std::fill(c.begin(), c.end(), 0);
      dfs(i, fg);
      for (int j = i + 1; j < V; ++j)
        if (c[j] && p[j] == p[i]) p[j] = i;
      if (p[p[i]] >= 0 && c[p[p[i]]]) {
        int pi = p[i];
        swap(w[i], w[pi]);
        p[i] = p[pi];
        p[pi] = i;
      }
      fg.clear();
    }
  }
  const vector<edge> &get_tree() {
    if (tree.empty())
      for (int i = 0; i < V; ++i) {
        if (p[i] >= 0)
          tree.push_back(edge{i, (int)p[i], w[i]});
      }
    return tree;
  }
};

struct dsu {
  vector<int> par, rank, size;
  int c;
  dsu(int n) : par(n), rank(n, 0), size(n, 1), c(n) {
    for (int i = 0; i < n; ++i) par[i] = i;
  }
  int find(int i) {
    return (par[i] == i ? i : (par[i] = find(par[i])));
  }
  bool same(int i, int j) {
    return find(i) == find(j);
  }
  int get_size(int i) {
    return size[find(i)];
  }
  int count() {
    return c;
  }
  int merge(int i, int j) {
    if ((i = find(i)) == (j = find(j))) return -1;
    else --c;
    if (rank[i] > rank[j]) swap(i, j);
    par[i] = j;
    size[j] += size[i];
    if (rank[i] == rank[j]) rank[j]++;
    return j;
  }
};

//find a permutation such that sum of max flow(p[i],p[i+1]) is maximum
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m;
  cin >> n >> m;
  vector<edge> ed(m);
  for (edge &e : ed) cin >> e.u >> e.v >> e.w, --e.u, --e.v;
  GomoryHuTree gt(n, ed);
  vector<edge> t = gt.get_tree();
  sort(t.begin(), t.end(), [](const edge & l, const edge & r) {
    return l.w > r.w;
  });
  dsu d(n);
  vector<vector<int>> perm(n);
  long long ans = 0LL;
  for (int i = 0; i < n; ++i) perm[i].push_back(i);
  for (const auto &e : t) {
    int l = d.find(e.u), r = d.find(e.v);
    if (l != d.merge(l, r)) swap(l, r);
    ans += e.w;
    for (int j : perm[r]) perm[l].push_back(j);
    perm[r].clear();
  }
  cout << ans << endl;
  for (int i = 0; i < n; ++i) cout << perm[d.find(0)][i] + 1 << " \n"[i + 1 == n];
  return 0;
}
