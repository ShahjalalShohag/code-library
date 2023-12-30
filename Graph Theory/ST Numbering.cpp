#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

/*An st-numbering of G with two designated vertices s and t is an assignment of
the integers from 1 to n to the vertices of G, such that
    1. each vertex is assigned a distinct number,
    2. s is assigned the number 1,
    3. t is assigned the number n, and
    4. if a vertex v is assigned the number i with 1 < i < n, then at least one neighbor
       of v is assigned a smaller number than i and at least one neighbor of v
       is assigned a larger number than i
To form the DAG add edges from lower numbered vertex to higher numbered vertex*/

vector<int> g[N];
int low[N], dis[N], T = 0, par[N], sign[N];
vector<int> preorder;

bool dfs1(int u, int pre = -1) { // check if there is no articulation point
  low[u] = dis[u] = ++T;
  int child = 0;
  for(int v : g[u]) if(v != pre) {
      if(dis[v] == -1) {
        child++;
        if(!dfs1(v, u)) return 0;
        low[u] = min(low[u], low[v]);
        if(pre != -1 && low[v] >= dis[u]) return 0;
      } else low[u] = min(low[u], dis[v]);
    }
  if(pre == -1 && child > 1) return 0;
  return 1;
}

void dfs2(int u, int pre = -1) { //construct the dfs tree and preorder treversal
  low[u] = dis[u] = ++T;
  for(int v : g[u]) if(v != pre) {
      if(dis[v] == -1) {
        preorder.push_back(v);
        dfs2(v, u);
        low[u] = min(low[u], low[v]);
        par[v] = u;
      } else low[u] = min(low[u], dis[v]);
    }
}

list<int> lst;
list<int>::iterator it[N];
vector<int> st_numbering(int n, int s, int t) {
  // additional edge
  g[s].push_back(t);
  g[t].push_back(s);
  T = 0;
  preorder.clear();
  for(int i = 1; i <= n; i++) dis[i] = low[i] = -1, sign[i] = 0;
  if(!dfs1(t)) return vector<int>(); // no bipolar orientation if it is not 2-vertex connected after adding the s-t edge
  for(int i = 1; i <= n; i++) if(dis[i] == -1) return vector<int>(); // no bipolar orientation
  for(int i = 1; i <= n; i++) dis[i] = low[i] = -1, sign[i] = 0;
  T = 0;
  preorder.clear();
  dis[s] = low[s] = ++T;
  sign[dis[s]] = -1;
  dfs2(t);
  lst.clear();
  lst.push_back(s);
  lst.push_back(t);
  it[dis[s]] = lst.begin();
  it[dis[t]] = next(lst.begin());
  for(int v : preorder) {
    if(sign[low[v]] == -1) it[dis[v]] = lst.insert(it[dis[par[v]]], v);
    else it[dis[v]] = lst.insert(next(it[dis[par[v]]]), v);
    sign[dis[par[v]]] = -sign[low[v]];
  }
  vector<int> ret(lst.begin(), lst.end());
  return ret;
}

int ans[N];
int32_t main() {
  int n, m, s, t;
  cin >> n >> m >> s >> t;
  for(int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  vector<int> ord = st_numbering(n, s, t);
  if(ord.size() == 0) return cout << -1 << '\n', 0;
  for(int i = 0; i < n; i++) ans[ord[i]] = i + 1;
  for(int i = 1; i <= n; i++) cout << ans[i] << ' ';
  cout << endl;
  return 0;
}
// one problem: https://codeforces.com/contest/1916/problem/F
