#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

/*A graph is chordal if any cycle C with |C| >= 4 has a chord i.e. an edge (u, v) where u and v is in the cycle but (u, v) is not
A perfect elimination ordering in a graph is an ordering of the vertices of the graph such that,
for each vertex v, v and the neighbors of v that occur after v in the order form a clique.
A graph is chordal if and only if it has a perfect elimination ordering.

To find PEO we will use maximum cardinality search (MCS), which
selects u which has the largest indeg with the nodes that are already selected.

notice that if we add a directed edge from u to v if(pos[u] < pos[v])  the we will find a DAG.
optimal vertex coloring of the graph: col[i] = smallest color that is not used by any of the parents of i in the DAG
chromatic number = max clique
chromatic polynomial = (x - d_1) * (x - d_2) * ... * (x - d_n) where d_i = number of neighbors of i that come after i in PEO
*/

vector<int> g[N]; //each node should be unique in the adjacency list
vector<int> PEO(int n) { // O(n + m)
  int top = 0;
  vector<int> ord, vis(n + 1, 0), indeg(n + 1, 0);
  vector<vector<int>> bucket(n); // bucket dijkstra
  for(int i = 1; i <= n; ++i) bucket[0].push_back(i);
  for(int i = 1; i <= n; ) {
    while(bucket[top].empty()) --top;
    int u = bucket[top].back();
    bucket[top].pop_back();
    if(vis[u]) continue;
    ord.push_back(u);
    vis[u] = 1;
    ++i;
    for(int v : g[u]) {
      if(vis[v]) continue;
      bucket[++indeg[v]].push_back(v);
      top = max(top, indeg[v]);
    }
  }
  reverse(ord.begin(), ord.end());
  return ord;
}
int max_clique(int n, vector<int> &ord) {
  int ans = 0;
  vector<int> pos(n + 1);
  for(int i = 0; i < n; i++) pos[ord[i]] = i;
  for(int i = 0; i < n; i++) {
    int u = ord[i];
    int nw = 1;
    for (int v : g[u]) if (pos[u] < pos[v]) nw++;
    ans = max(ans, nw);
  }
  return ans;
}
bool is_chordal(int n, vector<int> & ord, set<pair<int, int>> &edges) {
  vector<int> pos(n + 1);
  for(int i = 0; i < n; i++) pos[ord[i]] = i;
  for(int u = 1; u <= n; u++) {
    int mn = n + 1;
    for(auto v : g[u]) if(pos[u] < pos[v]) mn = min(mn, pos[v]);
    if(mn != n + 1) {
      int p = ord[mn];
      for(auto v : g[u]) if(pos[v] > pos[u] && v != p && edges.find({v, p}) == edges.end()) return 0;
    }
  }
  return 1;
}
vector<int> vertext_coloring(int n, vector<int>& ord) {
  vector<int> pos(n + 1), col(n + 1, 0);
  for(int i = 0; i < n; i++) pos[ord[i]] = i;
  for(int i = 0; i < n; i++) {
    int u = ord[i];
    set<int> vis;
    for(auto v : g[u]) if(pos[u] > pos[v]) {
        vis.insert(col[v]);
      }
    int mex = 0;
    while(vis.find(mex) != vis.end()) mex++;
    col[u] = mex;
  }
  return col;
}
int a[N], b[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  int t;
  cin >> t;
  while(t--) {
    int n, m;
    cin >> n >> m;
    set<pair<int, int>> edges;
    for(int i = 1; i <= m; i++) {
      int u, v;
      cin >> u >> v;
      g[u].push_back(v);
      g[v].push_back(u);
      a[i] = u, b[i] = v;
      edges.insert({u, v});
      edges.insert({v, u});
    }
    auto ord = PEO(n);
    vector<int> pos(n + 1);
    for(int i = 0; i < n; i++) pos[ord[i]] = i;
    int ok = is_chordal(n, ord, edges);
    if(!ok) cout << "No solution\n";
    else {
      for(int i = 1; i <= m; i++) {
        if(pos[a[i]] < pos[b[i]]) cout << '^';
        else cout << 'v';
      }
      cout << '\n';
    }
    for(int i = 1; i <= n; i++) g[i].clear();
  }
  return 0;
}

//https://github.com/spaghetti-source/algorithm/blob/master/graph/is_chordal.cc
//https://www.codechef.com/problems/EGGFREE
