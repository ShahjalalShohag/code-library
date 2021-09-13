#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, mod = 1e9 + 97;
template <const int32_t MOD>
struct modint {
  int32_t value;
  modint() = default;
  modint(int32_t value_) : value(value_) {}
  inline modint<MOD> operator + (modint<MOD> other) const { int32_t c = this->value + other.value; return modint<MOD>(c >= MOD ? c - MOD : c); }
  inline modint<MOD> operator - (modint<MOD> other) const { int32_t c = this->value - other.value; return modint<MOD>(c <    0 ? c + MOD : c); }
  inline modint<MOD> operator * (modint<MOD> other) const { int32_t c = (int64_t)this->value * other.value % MOD; return modint<MOD>(c < 0 ? c + MOD : c); }
  inline modint<MOD> & operator += (modint<MOD> other) { this->value += other.value; if (this->value >= MOD) this->value -= MOD; return *this; }
  inline modint<MOD> & operator -= (modint<MOD> other) { this->value -= other.value; if (this->value <    0) this->value += MOD; return *this; }
  inline modint<MOD> & operator *= (modint<MOD> other) { this->value = (int64_t)this->value * other.value % MOD; if (this->value < 0) this->value += MOD; return *this; }
  inline modint<MOD> operator - () const { return modint<MOD>(this->value ? MOD - this->value : 0); }
  modint<MOD> pow(uint64_t k) const { modint<MOD> x = *this, y = 1; for (; k; k >>= 1) { if (k & 1) y *= x; x *= x; } return y; }
  modint<MOD> inv() const { return pow(MOD - 2); }  // MOD must be a prime
  inline modint<MOD> operator /  (modint<MOD> other) const { return *this *  other.inv(); }
  inline modint<MOD> operator /= (modint<MOD> other)       { return *this *= other.inv(); }
  inline bool operator == (modint<MOD> other) const { return value == other.value; }
  inline bool operator != (modint<MOD> other) const { return value != other.value; }
  inline bool operator < (modint<MOD> other) const { return value < other.value; }
  inline bool operator > (modint<MOD> other) const { return value > other.value; }
};
template <int32_t MOD> modint<MOD> operator * (int64_t value, modint<MOD> n) { return modint<MOD>(value) * n; }
template <int32_t MOD> modint<MOD> operator * (int32_t value, modint<MOD> n) { return modint<MOD>(value % MOD) * n; }
template <int32_t MOD> istream & operator >> (istream & in, modint<MOD> &n) { return in >> n.value; }
template <int32_t MOD> ostream & operator << (ostream & out, modint<MOD> n) { return out << n.value; }

using mint = modint<mod>;

mint pw[N];
const mint P = 998244353, Q = 1e9 + 33, R = 99999989;
const int base = 10;
struct Tree {
  int n;
  vector<vector<int>> g;
  Tree() {}
  Tree(int _n) : n(_n) {
    g.resize(n + 1);
  }
  void add_edge(int u, int v) {
    g[u].push_back(v);
    g[v].push_back(u);
  }
  vector<int> bfs(int s) {
    queue<int> q;
    vector<int> d(n + 1, n * 2);
    d[0] = -1;
    q.push(s);
    d[s] = 0;
    while(!q.empty()) {
      int u = q.front();
      q.pop();
      for(auto v : g[u]) if(d[u] + 1 < d[v]) {
          d[v] = d[u] + 1;
          q.push(v);
        }
    }
    return d;
  }
  vector<int> get_centers() {
    auto du = bfs(1);
    int v = max_element(du.begin(), du.end()) - du.begin();
    auto dv = bfs(v);
    int u = max_element(dv.begin(), dv.end()) - dv.begin();
    du = bfs(u);
    vector<int> ans;
    for(int i = 1; i <= n; i++) if(du[i] + dv[i] == du[v] && du[i] >= du[v] / 2 && dv[i] >= du[v] / 2) {
        ans.push_back(i);
      }
    return ans;
  }
  mint yo(int u, int pre = 0) {
    vector<mint> nw;
    for(auto v : g[u]) if(v != pre) nw.push_back(yo(v, u));
    mint ans = 0;
    for(auto x : nw) {
      ans = ans + P.pow(x.value);
    }
    ans = ans * Q + R;
    return ans;
  }
  bool iso(Tree &t) {
    auto a = get_centers();
    auto b = t.get_centers();
    for(auto x : a) for(auto y : b) if(yo(x) == t.yo(y)) return 1;
    return 0;
  }
};
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  int t;
  cin >> t;
  while(t--) {
    int n;
    cin >> n;
    Tree t1(n);
    for(int i = 1; i < n; i++) {
      int u, v;
      cin >> u >> v;
      t1.add_edge(u, v);
    }
    Tree t2(n);
    for(int i = 1; i < n; i++) {
      int u, v;
      cin >> u >> v;
      t2.add_edge(u, v);
    }
    if(t1.iso(t2)) cout << "YES\n";
    else cout << "NO\n";
  }
  return 0;
}
// https://www.spoj.com/problems/TREEISO/
