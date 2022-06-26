#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;
// Given number of nodes n and q queries
// add edge (u,v)
// output the bridges in current graph
struct OnlineBridge {
  // 1 indexed
  vector<int> par, dsu_2ecc, dsu_cc, dsu_cc_size;
  int bridges;
  int lca_iteration;
  vector<int> last_visit;
  OnlineBridge() {}
  OnlineBridge(int n) {
    dsu_2ecc.resize(n + 1); // contains info of the forest of 2-edge-connected components
    dsu_cc.resize(n + 1); // contains the info of the forest of bridge trees
    par.resize(n + 1); // stores the correct parent of the root of each 2-edge-connected component in the bridge tree
    dsu_cc_size.resize(n + 1);
    lca_iteration = 0;
    last_visit.assign(n + 1, 0);
    for (int i=0; i<= n; ++i) {
        dsu_2ecc[i] = i;
        dsu_cc[i] = i;
        dsu_cc_size[i] = 1;
        par[i] = -1;
    }
    bridges = 0;
  }
  // returns representative of the 2-edge-connected component u is in
  int find_2ecc(int u) {
    if (u == -1) return -1;
    return dsu_2ecc[u] == u ? u : dsu_2ecc[u] = find_2ecc(dsu_2ecc[u]);
  }
  // returns representative of the bridge tree u is in
  int find_cc(int u) {
    u = find_2ecc(u);
    return dsu_cc[u] == u ? u : dsu_cc[u] = find_cc(dsu_cc[u]);
  }
  // let v = the root of the 2-edge-connected component u is in
  // now make v the root of the bridge tree v is in
  void make_root(int u) {
    u = find_2ecc(u);
    int root = u;
    int child = -1;
    while (u != -1) {
      int p = find_2ecc(par[u]);
      par[u] = child;
      dsu_cc[u] = root;
      child = u;
      u = p;
    }
    // at this moment child = the previous root of the tree
    int previous_root = child;
    dsu_cc_size[root] = dsu_cc_size[previous_root];
  }
  // contract everything from u to v into a single node, to be precise, their LCA
  void merge_path (int u, int v) {
    ++lca_iteration;
    vector<int> path_u, path_v;
    int lca = -1;
    while (lca == -1) {
      if (u != -1) {
        u = find_2ecc(u);
        path_u.push_back(u);
        if (last_visit[u] == lca_iteration){
          lca = u;
          break;
        }
        last_visit[u] = lca_iteration;
        u = par[u];
      }
      if (v != -1) {
        v = find_2ecc(v);
        path_v.push_back(v);
        if (last_visit[v] == lca_iteration){
          lca = v;
          break;
        }
        last_visit[v] = lca_iteration;
        v = par[v];
      }
    }
    // all the vertices of the cycle get compressed by attaching them to the LCA
    for (int x : path_u) {
      dsu_2ecc[x] = lca;
      if (x == lca) break;
      --bridges;
    }
    for (int x : path_v) {
      dsu_2ecc[x] = lca;
      if (x == lca) break;
      --bridges;
    }
  }
  void add_edge(int u, int v) {
    int x = u, y = v;
    u = find_2ecc(u);
    v = find_2ecc(v);
    // if they are already in the same 2-edge-connected component, then return immediately
    if (u == v) return;

    // now check if they are in the same bridge tree
    int comp_u = find_cc(u);
    int comp_v = find_cc(v);

    if (comp_u != comp_v) {
      ++bridges;
      if (dsu_cc_size[comp_u] > dsu_cc_size[comp_v]) {
        swap(u, v);
        swap(comp_u, comp_v);
      }
      make_root(u);
      par[u] = dsu_cc[u] = v;
      dsu_cc_size[comp_v] += dsu_cc_size[u];
    }
    else {
      merge_path(u, v);
    }
  }
}online_bridge;

int main() {
  int n, q; cin >> n >> q;
  online_bridge = OnlineBridge(n);
  while (q--) {
    int u, v; cin >> u >> v;
    online_bridge.add_edge(u, v);
    cout << online_bridge.bridges << '\n';
  }
  return 0;
}
// https://cp-algorithms.com/graph/bridge-searching-online.html
