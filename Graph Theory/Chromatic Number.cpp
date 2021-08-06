#include<bits/stdc++.h>
using namespace std;

vector<int> g[20];
//O(n*2^n)
int chromatic_number(int n) {
  const int N = 1 << n;
  vector<int> adj(n);
  for (int u = 0; u < n; ++u)
    for (int v : g[u])
      adj[u] |= (1 << v);

  int ans = n;
  for (int d : {7}) { //,11,21,33,87,93}) {
    long long mod = 1e9 + d;
    vector<long long> ind(N), aux(N, 1);
    ind[0] = 1;
    for (int S = 1; S < N; ++S) {
      int u = __builtin_ctz(S);
      ind[S] = ind[S ^ (1 << u)] + ind[(S ^ (1 << u)) & ~adj[u]];
    }
    for (int k = 1; k < ans; ++k) {
      long long w = 0;
      for (int i = 0; i < N; ++i) {
        int S = i ^ (i >> 1); // gray-code
        aux[S] = (aux[S] * ind[S]) % mod;
        w += (i & 1) ? aux[S] : -aux[S];
      }
      if (w % mod) ans = min(ans, k);
    }
  }
  return ans;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n; cin >> n;
  cin.ignore();
  for (int u = 0; u < n; u++) {
    string s, x; getline(cin, s);
    stringstream ss(s); 
    while (ss >> x) {
      int v = atoi(x.c_str());
      g[u].push_back(v);
      g[v].push_back(u);
    }
  }
  cout << chromatic_number(n) << '\n';
  return 0;
}
// https://open.kattis.com/problems/coloring