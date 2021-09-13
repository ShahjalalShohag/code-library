#include<bits/stdc++.h>
using namespace std;
using ll = long long;

const ll INF = 1e18;

template<typename T>
struct heap {
  struct node{
    node* ch[2] = {0, 0};
    int sz; T val;
    int from, to;
    node(T val, int from, int to): sz(1), val(val), from(from), to(to) {}
  };
  node* root = 0;
  heap(node* t = 0): root(t) {}
  node* meld(node* a, node* b) {
    if (!b) return a ? new node(*a) : 0;
    if (!a) return b ? new node(*b) : 0;
    a = new node(*a); b = new node(*b);
    if (a -> val > b -> val) swap(a, b);
    a -> ch[1] = meld(a -> ch[1], b);
    if (!a -> ch[0] || a -> ch[0] -> sz < a -> ch[1] -> sz) swap(a -> ch[0], a -> ch[1]);
    a -> sz = (a -> ch[1] ? a -> ch[1] -> sz : 0) + 1;
    return a;
  }
  heap meld(heap b) {
    return heap(meld(root, b.root));
  }
  heap insert(T x, int from, int to) { // from and to is needed only for this problem
    return heap(meld(root, new node(x, from, to)));
  }
  heap pop() {
    return heap(meld(root -> ch[0], root -> ch[1]));
  }
  T top() { // returns the smallest value of the heap
    return root ? root -> val : T(-1);
  }
  bool empty() {
    return !root;
  }
};
vector<vector<array<int, 3>>> g;
vector<vector<array<int, 3>>> rg;
vector<ll> Eppstein(int n, int src, int dest, int k) { // 0 indexed, directed graph
  vector<ll> d(n, INF);
  vector<int> par(n, -1);
  vector<int> idx(n, -1);
  vector<vector<int>> t(n);
  priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
  pq.emplace(0, dest);
  d[dest] = 0;
  while (!pq.empty()) {
    auto [w, u] = pq.top();
    pq.pop();
    for (auto [v, cost, id]: g[u]) {
      if (d[v] > w + cost) {
        d[v] = w + cost;
        par[v] = u;
        idx[v] = id;
        pq.emplace(d[v], v);
      }
    }
  }
  for (int i = 0; i < n; ++i) {
    if (par[i] != -1) t[par[i]].push_back(i);
  }
  vector<heap<ll>> h(n);
  for (int i = 0; i < n; ++i) h[i] = heap<ll>();
  queue<int> q;
  q.emplace(dest);
  while (!q.empty()) {
    auto u = q.front();
    q.pop();
    if (par[u] != -1) h[u] = h[u].meld(h[par[u]]);
    for (auto [v, cost, id]: rg[u]) {
      if (id != idx[u]) h[u] = h[u].insert(cost - d[u] + d[v], u, v);
    }
    for (auto v:t[u]) {
      q.emplace(v);
    }
  }
  auto comp = [](auto s, auto t) { return get<0>(s) > get<0>(t); };
  priority_queue<tuple<ll, heap<ll>::node*>, vector<tuple<ll, heap<ll>::node*>>, decltype(comp)> Q(comp);
  heap<ll> s;
  s = s.insert(d[src], -1, src);
  Q.emplace(d[src], s.root);
  vector<ll>ans;
  while (!Q.empty()) {
    auto [w, cur] = Q.top();
    Q.pop();
    if (w >= INF) break;
    ans.push_back(w);
    if ((ans.size()) == k) break;
    if (cur -> ch[0]) {
      Q.emplace(w + cur -> ch[0] -> val - cur -> val, cur -> ch[0]);
    }
    if (cur -> ch[1]) {
      Q.emplace(w + cur -> ch[1] -> val - cur -> val, cur -> ch[1]);
    }
    if (h[cur -> to].root) {
      Q.emplace(w + h[cur -> to].root -> val, h[cur -> to].root);
    }
  }
  while (ans.size() < k) {
    ans.push_back(-1);
  }
  return ans;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m, src, dest, k; cin >> n >> m >> src >> dest >> k;
  g.resize(n);
  rg.resize(n);
  for (int i = 0; i < m; ++i) {
    int u, v, w; cin >> u >> v >> w;
    g[v].push_back({u, w, i});
    rg[u].push_back({v, w, i});
  }
  auto ans = Eppstein(n, src, dest, k);
  for (auto x: ans) {
    cout << x << '\n';
  }
  return 0;
}
// https://qiita.com/hotman78/items/42534a01c4bd05ed5e1e
// https://judge.yosupo.jp/problem/k_shortest_walk
