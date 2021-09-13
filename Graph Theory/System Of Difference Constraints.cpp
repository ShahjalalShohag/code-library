#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

/*
Given some inequality on some variables (x_1, x_2, ...x_n)
in the form x_i-x_j <= w_ij
we need to determine whether we can assign values to the variables so that
all the given inequalities are satisfiable or not. If satisfiable, then output a solution.
Solution:
* For each variable we create a vertex.
* For each inequality, x_i-x_j <= w, we give a directed edge (v_i, v_j ) with cost -w.
* Create a source vertex S and give an edge (S, v_i) for all vertices with cost 0.

#If there is NO negative cycle in the constraint graph,
 then there is a solution for the system.
#x_i=shortest path from source to v_i i.e. dist[i]-dist[src]
#Let x_1, x_2, ..., x_n be a solution to a system of difference constraints
 and let d be any constant. Then
x_1 + d, x_2 + d, ..., x_n + d is a solution as well.
#it minimizes max{x_i}-min{x_j}

There can be two types of solution
T=0 ->sum_{x_i}=maximum. It extracts any solution maintaining this
T=1 ->sum_{x_i}=minimum. It extracts the lexicographically smallest solution

complexity : O(n+constraints) on average
try to add negative edges for better complexity
*/

bool vis[N];
int d[N];
vector<pair<int, int>> g[N];
bool spfa(int u) {
  vis[u] = 1;
  for(auto e : g[u]) {
    int w = e.second, v = e.first;
    if(d[u] + w > d[v]) { //d[u] + w < d[v] for T=0
      if(vis[v]) return 0;
      d[v] = d[u] + w;
      if(!spfa(v)) return 0;
    }
  }
  vis[u] = 0;
  return 1;
}

/*
add an edge of type (x_u - x_v OP w)
here ty = 0 means OP is <=
ty = 1 means OP is >=
ty = 2 means OP is ==
*/
void add(int u, int v, int w, int ty) {
  if(ty == 1) add(v, u, -w, 0);
  else if(ty == 2) {
    add(u, v, w, 0);
    add(v, u, -w, 0);
  } else {
    g[u].emplace_back(v, -w); //for T=1
    //g[v].emplace_back(u, w); //for T=0
  }
}

int a[N], ans[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  int n, l, r, k;
  cin >> n >> l >> r >> k;
  for(int i = 1; i <= n; i++) cin >> a[i];
  for(int i = 1; i < n; i++) {
    if(a[i] < a[i + 1]) add(i + 1, i, k, 0), add(i, i + 1, -1, 0);
    else if(a[i] > a[i + 1]) add(i, i + 1, k, 0), add(i + 1, i, -1, 0);
    else add(i + 1, i, 0, 0), add(i, i + 1, 0, 0);
  }
  int s = n + 1;
  for(int i = 1; i <= n; i++) add(i, s, r, 0), add(s, i, -l, 0);   //adding the constraint l <= a_i <= r
  memset(vis, 0, sizeof vis);
  for(int i = 1; i <= s ; i++) d[i] = -1e9 - 9; //1e9 + 9 for T = 0,  try to make INF as close to 0 as possible
  d[s] = 0;
  if(!spfa(s)) puts("-1");
  else {
    for(int i = 1; i <= n; i++) ans[i] = d[i] - d[s];
    for(int i = 1; i <= n; i++) {
      if(i > 1) cout << ' ';
      cout << ans[i];
    }
    cout << '\n';
  }
  return 0;
}
