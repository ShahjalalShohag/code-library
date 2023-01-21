#include<bits/stdc++.h>
using namespace std;

/**
 * Credit: kactl
 * Given a graph as a symmetric bitset matrix (without any self edges)
 * Finds the maximum matching
 * Can be used to find the maximum independent set by finding a clique of the complement graph.
 * Runs in about 1s for n=155, and faster for sparse graphs
 * 0 indexed
 */
const int N = 105;
typedef vector<bitset<N>> graph;
#define sz(v) (int)v.size()
struct Maxclique {
  double limit = 0.025, pk = 0;
  struct Vertex {
    int i, d = 0;
  };
  typedef vector<Vertex> vv;
  graph e;
  vv V;
  vector<vector<int>> C;
  vector<int> qmax, q, S, old;
  void init(vv& r) {
    for (auto& v : r) v.d = 0;
    for (auto& v : r) for (auto j : r) v.d += e[v.i][j.i];
    sort(r.begin(), r.end(), [](auto a, auto b) {
      return a.d > b.d;
    });
    int mxD = r[0].d;
    for (int i = 0; i < sz(r); i++) r[i].d = min(i, mxD) + 1;
  }
  void expand(vv& R, int lev = 1) {
    S[lev] += S[lev - 1] - old[lev];
    old[lev] = S[lev - 1];
    while (sz(R)) {
      if (sz(q) + R.back().d <= sz(qmax)) return;
      q.push_back(R.back().i);
      vv T;
      for(auto v : R) if (e[R.back().i][v.i]) T.push_back({v.i});
      if (sz(T)) {
        if (S[lev]++ / ++pk < limit) init(T);
        int j = 0, mxk = 1, mnk = max(sz(qmax) - sz(q) + 1, 1);
        C[1].clear(), C[2].clear();
        for (auto v : T) {
          int k = 1;
          auto f = [&](int i) {
            return e[v.i][i];
          };
          while (any_of(C[k].begin(), C[k].end(), f)) k++;
          if (k > mxk) mxk = k, C[mxk + 1].clear();
          if (k < mnk) T[j++].i = v.i;
          C[k].push_back(v.i);
        }
        if (j > 0) T[j - 1].d = 0;
        for (int k = mnk; k <= mxk; k++) for (int i : C[k])
          T[j].i = i, T[j++].d = k;
        expand(T, lev + 1);
      } else if (sz(q) > sz(qmax)) qmax = q;
      q.pop_back(), R.pop_back();
    }
  }
  Maxclique(graph g) : e(g), C(sz(e) + 1), S(sz(C)), old(S) {
    for (int i = 0; i < sz(e); i++) V.push_back({i});
  }
  vector<int> solve() { // returns the clique
    init(V), expand(V);
    return qmax;
  }
};
array<int, 2> p[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, d; cin >> n >> d;
  for (int i = 0; i < n; i++) {
    cin >> p[i][0] >> p[i][1];
  }
  graph g(n);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i == j) continue;
      int dis = (p[i][0] - p[j][0]) * (p[i][0] - p[j][0]) + (p[i][1] - p[j][1]) * (p[i][1] - p[j][1]);
      if (dis <= d * d) {
        g[i][j] = 1;
      }
    }
  }
  Maxclique M(g);
  vector<int> clique = M.solve();
  cout << clique.size() << '\n';
  for (auto i: clique) {
    cout << i + 1 << ' ';
  }
  cout << '\n';
  return 0;
}
// https://vjudge.net/problem/Kattis-sensor
