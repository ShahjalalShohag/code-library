#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

struct persistent_dsu {
  struct state {
    int u, v, rnku, rnkv;
    state() {
      u = -1;
      v = -1;
      rnkv = -1;
      rnku = -1;
    }
    state(int _u, int _rnku, int _v, int _rnkv) {
      u = _u;
      rnku = _rnku;
      v = _v;
      rnkv = _rnkv;
    }
  };

  stack<state> st;
  int par[N], depth[N];
  int comp;
  persistent_dsu() {
    comp = 0;
    memset(par, -1, sizeof(par));
    memset(depth, 0, sizeof(depth));
  }

  int root(int x) {
    if(x == par[x]) return x;
    return root(par[x]);
  }

  void init(int n) {
    comp = n;
    for(int i = 0; i <= n; i++) {
      par[i] = i;
      depth[i] = 1;
    }
  }

  bool connected(int x, int y) {
    return root(x) == root(y);
  }

  void unite(int x, int y) {
    int rx = root(x), ry = root(y);
    if(rx == ry) {
      st.push(state());
      return;
    }
    if(depth[rx] < depth[ry])
      par[rx] = ry;
    else if(depth[ry] < depth[rx])
      par[ry] = rx;
    else {
      par[rx] = ry;
      depth[rx]++;
    }
    comp--;
    st.push(state(rx, depth[rx], ry, depth[ry]));
  }
  ///how many last added edges you want to erase
  void backtrack(int c) {
    while(!st.empty() && c) {
      if(st.top().u == -1) {
        st.pop();
        c--;
        continue;
      }
      par[st.top().u] = st.top().u;
      par[st.top().v] = st.top().v;
      depth[st.top().u] = st.top().rnku;
      depth[st.top().v] = st.top().rnkv;
      st.pop();
      c--;
      comp++;
    }
  }
};
persistent_dsu d;
vector<pair<int, int>> alive[4 * N];
void upd(int n, int b, int e, int i, int j, pair<int, int> &p) {
  if(b > j || e < i) return;
  if(b >= i && e <= j) {
    alive[n].push_back(p);///this edge was alive in this time range
    return;
  }
  int l = 2 * n, r = l + 1, mid = b + e >> 1;
  upd(l, b, mid, i, j, p);
  upd(r, mid + 1, e, i, j, p);
}
int ans[N];
void query(int n, int b, int e) {
  if(b > e) return;
  int prevsz = d.st.size();
  ///add edges which were alive in this range
  for(auto p : alive[n]) d.unite(p.first, p.second);
  if(b == e) {
    ans[b] = d.comp;
    d.backtrack(d.st.size() - prevsz);
    return;
  }
  int l = 2 * n, r = l + 1, mid = b + e >> 1;
  query(l, b, mid);
  query(r, mid + 1, e);
  d.backtrack(d.st.size() - prevsz);
}
struct HASH {
  size_t operator()(const pair<int, int>&x)const {
    return hash<long long>()(((long long)x.first) ^ (((long long)x.second) << 32));
  }
};
set<pair<int, int>>se;
bool isquery[N];
unordered_map<pair<int, int>, int, HASH>st;
int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  freopen("connect.in", "r", stdin);
  freopen("connect.out", "w", stdout);
  st.reserve(1 << 20);
  int i, j, k, n, m, u, v;
  cin >> n >> m;
  d.init(n);
  for(i = 1; i <= m; i++) {
    string ty;
    cin >> ty;
    if(ty == "?") {
      isquery[i] = 1;
    } else if(ty == "+") {
      cin >> u >> v;
      if(u > v) swap(u, v);
      pair<int, int> p = {u, v};
      se.insert(p);
      st[p] = i;
    } else {
      cin >> u >> v;
      if(u > v) swap(u, v);
      pair<int, int> p = {u, v};
      se.erase(p);
      upd(1, 1, m, st[p], i - 1, p); ///in this time range this edge was in the DSU
    }
  }
  for(auto p : se) upd(1, 1, m, st[p], m, p); ///update rest of the edges
  se.clear();
  query(1, 1, m);
  for(i = 1; i <= m; i++) if(isquery[i]) cout << ans[i] << endl;
  return 0;
}
// https://codeforces.com/gym/100551/problem/A
