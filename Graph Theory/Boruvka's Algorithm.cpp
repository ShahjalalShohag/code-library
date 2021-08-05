#include<bits/stdc++.h>
using namespace std;

const int N = 2e5 + 9;

int t[N * 30][2], sz;
int el[N * 30], tot[N * 30];
void add(int x, int id) {
  int cur = 0;
  tot[cur]++;
  for (int i=30; i>=0; i--) {
    bool b = x&(1<<i);
    if (t[cur][b] == 0) t[cur][b] = ++sz;
    cur = t[cur][b];
    tot[cur]++;
  }
  el[cur] = id;
}

void del(int x, int id) {
  int cur = 0;
  tot[cur]--;
  for (int i=30; i>=0; i--) {
    bool b = x&(1<<i);
    cur = t[cur][b];
    tot[cur]--;
  }
  assert(el[cur] == id);
  el[cur] = 0;
}

int minxor(int x) {
  int cur = 0;
  for (int i=30; i>=0; i--) {
    bool b = x&(1<<i);
    if (t[cur][b] && tot[t[cur][b]] > 0)  cur = t[cur][b];
    else cur = t[cur][!b];
  }
  assert(el[cur]);
  return el[cur];
}

struct dsu {
  vector<int> par, rnk, size; int c;
  dsu(int n) : par(n+1), rnk(n+1,0), size(n+1,1), c(n) {
    for (int i = 1; i <= n; ++i) par[i] = i;
  }
  int find(int i) { return (par[i] == i ? i : (par[i] = find(par[i]))); }
  bool same(int i, int j) { return find(i) == find(j); }
  int get_size(int i) { return size[find(i)]; }
  int count() { return c; } //connected components
  int merge(int i, int j) {
    if ((i = find(i)) == (j = find(j))) return -1; else --c;
    if (rnk[i] > rnk[j]) swap(i, j);
    par[i] = j; size[j] += size[i];
    if (rnk[i] == rnk[j]) rnk[j]++;
    return j;
  }
};
//take care the non - unique edges
int a[N];
vector<int> vec[N];
int32_t main() {
  int n; cin >> n;
  for(int i = 1; i <= n; i++) cin >> a[i];
  sort(a + 1, a + n + 1);
  n = unique(a + 1, a + n + 1) - a - 1;
  dsu d(n);
  int cnt = n;
  long long ans = 0;
  while(cnt > 1){
    for(int i = 1; i <= n; i++) vec[d.find(i)].push_back(i), add(a[i], i);
    vector<array<int, 3>> edges;
    for(int i = 1; i <= n; i++){
      if(vec[i].empty()) continue;
      for(auto id: vec[i]) del(a[id], id);
      pair<int, int> nw = {2e9, 2e9};
      for(auto id: vec[i]){
        int k = minxor(a[id]);
        nw = min(nw, {a[id] ^ a[k], k});
      }
      edges.push_back({nw.first, i, nw.second});
      for(auto id: vec[i]) add(a[id], id);
    }
    sort(edges.begin(), edges.end());
    for(auto e: edges){
      int x = d.find(e[1]);
      int y = d.find(e[2]);
      if(x != y){
        cnt--;
        ans += e[0];
        d.merge(x, y);
      }
    }
    for(int i = 1; i <= n; i++) vec[i].clear();
    for(int i = 0; i <= sz; i++)  t[i][0] = t[i][1] = 0, tot[i] = 0;
    sz = 0;
  }
  cout << ans << '\n';
  return 0;
}
// https://codeforces.com/contest/888/problem/G
