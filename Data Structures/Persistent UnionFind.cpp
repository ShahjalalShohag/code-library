#include<bits/stdc++.h>
using namespace std;

const int N = 2e5 + 9;
template<typename T>
struct PersistentArray { // 0-indexed
  struct node {
    node* l, * r; T x;
  };
  int n = 1;
  vector<node*> root;
  int build(vector<T> v) {
    while (n < v.size()) n <<= 1;
    root.push_back(build(0, n - 1, v));
    return root.size() - 1;
  }
  node* build(int l, int r, vector<T>& v) {
    node* cur = new node();
    if (l == r) {
      if (l < v.size()) cur -> x = v[l];
      else cur -> x = 0;
    }
    else {
      cur -> l = build(l, (l + r) >> 1, v);
      cur -> r = build(((l + r) >> 1) + 1, r, v);
    }
    return cur;
  }
  // get the ith value of the rth array
  T get_val(int r, int i) {
    return get_val(root[r], i, 0, n - 1);
  }
  T get_val(node* cur, int i, int l, int r) {
    if (l == r) return cur -> x;
    if (i <= ((l + r) >> 1)) return get_val(cur -> l, i, l, (l + r) >> 1);
    else return get_val(cur -> r, i, ((l + r) >> 1) + 1, r);
  }
  // update the ith value if the rth array by x and return the new root of the array
  int upd(int r, int i, T x) {
    root.push_back(upd(root[r], i, x, 0, n - 1));
    return root.size() - 1;
  }
  void set(int r, int i, T x) {
    int k = upd(r, i, x);
    root[r] = root[k];
    root.pop_back();
  }
  node* upd(node* pre, int i, T x, int l, int r) {
    node* cur = new node();
    if (l == r){
      cur -> x = x;
    }
    else {
      if (i <= ((l + r) >> 1)) {
        cur -> l = upd(pre -> l, i, x, l, (l + r) >> 1);
        cur -> r = pre -> r;
      }
      else {
        cur -> l = pre -> l;
        cur -> r = upd(pre -> r, i, x, ((l + r) >> 1) + 1, r);
      }
    }
    return cur;
  }
};
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
struct PersistentDSU {
  PersistentArray<int> par, sz;
  vector<int> c; int cur = 0;
  PersistentDSU() {}
  PersistentDSU(int n, int q) { // q -> maximum instances of DSU
    vector<int> p(n + 1);
    for (int i = 1; i <= n; i++) {
      p[i] = i;
    }
    par.build(p);
    sz.build(vector<int> (n + 1, 1));
    c.resize(q + 1, n); cur = 0; // initial DSU is the 0th one
  }
  int find(int r, int u) {
    int p = par.get_val(r, u);
    if (p == u) return u;
    int cur = find(r, p);
    par.set(r, u, cur);
    return cur;
  }
  bool same(int r, int u, int v) { return find(r, u) == find(r, v); }
  int get_size(int r, int u) { return sz.get_val(r, find(r, u)); }
  int count(int r) { return c[r]; } //connected components
  int merge(int r, int u, int v) { // returns the updated root
    cur++;
    c[cur] = c[r];
    if ((u = find(r, u)) == (v = find(r, v))) {
      par.upd(r, 0, 0);
      sz.upd(r, 0, 0);
      // assert(cur == par.root.size() - 1);
      return cur; 
    }
    else c[cur]--;
    if (rnd() % 2) swap(u, v);
    int x = sz.get_val(r, v) + sz.get_val(r, u);
    par.upd(r, u, v); 
    sz.upd(r, v, x);
    // assert(cur == par.root.size() - 1);
    return cur;
  }
};
int r[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, q; cin >> n >> q;
  PersistentDSU d(n, q + 1);
  for (int i = 1; i <= q; i++) {
    int ty, id, u, v; cin >> ty >> id >> u >> v;
    ++id, ++u; ++v;
    if (ty == 0) {
      r[i] = d.merge(r[id], u, v);
    }
    else {
      r[i] = r[i - 1];
      cout << d.same(r[id], u, v) << '\n';
    }
  }
  return 0;
}
// https://judge.yosupo.jp/problem/persistent_unionfind