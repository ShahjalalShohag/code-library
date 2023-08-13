#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;
using ll = long long;
const ll inf = 1e18;

mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
struct node {
  node *l, *r;
  int prior;
  int sz;
  ll val;
  ll lazy_a, lazy_d;
  node(ll id) {
    l = r = nullptr;
    sz = 1;
    val = id;
    prior = rnd();
    lazy_a = lazy_d = 0;
  }
};

// this treap maintains a list of values sorted in non-increasing order
// so the BST is from large to small
// And the heap is max heap
// Also, it supports adding an arithmetic progression to all nodes in the treap
// And, it can merge two treaps: so basically merging two non-increasing sequences
// into one non-increasing sequence while maintaing all the BST and Heap properties
struct treap {
  node *root;
  treap() {
    root = nullptr;
  }
  void pull(node * t) {
    if (!t) return;
    t -> sz = (t -> l ? t -> l -> sz : 0) + 1 + (t -> r ? t -> r -> sz : 0);
  }
  void add(node *t, ll a, ll d) {
    if (!t) return;
    t -> lazy_a += a;
    t -> lazy_d += d;
    t -> val += a + d * (t -> l ? t -> l -> sz : 0);
  }
  void push(node * t) {
    add(t -> l, t -> lazy_a, t -> lazy_d);
    add(t -> r, t -> lazy_a + ((t -> l ? t -> l -> sz : 0) + 1) * t -> lazy_d, t -> lazy_d);
    t -> lazy_a = t -> lazy_d = 0;
  }
  void split(node *t, ll x, node *&l, node *&r) {
    if (t == nullptr) {
      l = r = nullptr;
      return;
    }
    push(t);
    if (t->val >= x) {
      split(t->r, x, l, r);
      t->r = l;
      l = t;
    } else {
      split(t->l, x, l, r);
      t->l = r;
      r = t;
    }
    pull(t);
  }
  node* merge(node *l, node *r) { // assuming all l-> val >= r -> val
    if (!l || !r) return l ? l : r;
    push(l); push(r);
    if (l->prior > r->prior) {
      l->r = merge(l->r, r);
      pull(l);
      return l;
    }
    r->l = merge(l, r->l);
    pull(r);
    return r;
  }
  node* merge_treap(node *l, node *r) { // not necessarily all l -> val >= r -> val
    if(!l) return r;
    if(!r) return l;
    push(l); push(r);
    if(l->prior < r->prior) swap(l, r);
    node *L, *R;
    split(r, l->val, L, R);
    l->r = merge_treap(l->r, R);
    l->l = merge_treap(L, l->l);
    pull(l);
    return l;
  }
  void insert(ll id) {
    node *l, *r;
    split(root, id, l, r);
    root = merge(merge(l, new node(id)), r);
  }
  void combine(node *x) { //combine with another treap
    root = merge_treap(root, x);
  }
  void add_arithmetic_progression(ll a, ll d) {
    add(root, a, d);
  }
  vector<ll> ans;
  void dfs(node* cur) {
    if(!cur) return;
    push(cur);
    dfs(cur -> l);
    ans.push_back(cur -> val);
    dfs(cur -> r);
  }
  vector<ll> get() {
    ans.clear();
    dfs(root);
    return ans;
  }
}dp[N];
// let f[u][i] = answer when we take i nodes from the subtree of u
// f[u] is convex and its difference array is non-increasing
// Why? the transition is following
// merged_f[u][i + j] = max(merged_f[u][i + j], f[u][i] + f[v][j] + w * j * (k - j)) over 0 <= i, j
// here j * (k - j) is convex, adding multiple convex function is convex
// also, (max, +) convolution is also convex (basically minkowski sum)

// Now, dp[u] contains the difference array of f[u]
// so dp[u][i] = (f[u][i] - f[u][i - 1]) for 1 <= i <= sz[u]
// And this sequence is non-increasing (which makes f[u] convex)
int n, k;
vector<pair<int, int>> g[N];
void dfs(int u, int p = 0) {
  dp[u].insert(0); // f[u][0] = f[u][1] = 0, so dp[u][1] = 0
  for (auto [v, w]: g[u]) {
    if (v ^ p) {
      dfs(v, u);
      // add w * j * (k - j) to f[v]
      // so in the difference array its just adding w * (k - 1), w * (k - 3), w * (k - 5), ...
      // so its an arithmetic progression of a = w * (k - 1) and d = -2 * w
      dp[v].add_arithmetic_progression(1LL * w * (k - 1), -2LL * w);
      
      // merge f[u] and f[v]
      // as they both are convex, the difference array of merge(f[u], f[v])
      // is just the sorted (in non-decreasing) order of dp[u] and dp[v]
      dp[u].combine(dp[v].root); 
    }
  }
}


// Given a tree with n nodes, find the maximum of sum of all pair distances of k nodes
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  cin >> n >> k;
  for (int i = 1; i < n; i++) {
    int u, v, w; cin >> u >> v >> w;
    g[u].push_back({v, w});
    g[v].push_back({u, w});
  }
  dfs(1);
  // f[u][k] = sum of dp[u][i] such that i <= k
  auto v = dp[1].get();
  ll ans = 0;
  for (int i = 0; i < k; i++) {
    ans += v[i];
  }
  cout << ans << '\n';
  return 0;
}

// https://codeforces.com/gym/104128/problem/H
// https://qoj.ac/files/ucup/tutorials/tutorials-1-0.pdf
// bruteforces of the same solution: https://codeforces.com/gym/104128/submission/218661168
