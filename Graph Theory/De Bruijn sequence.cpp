// Generates a De Bruijn sequence B(k, n): a cyclic sequence over a k-symbol
// alphabet such that every possible string of length n appears exactly once
// as a contiguous substring.
// Built via a Hierholzer-style Eulerian circuit on the De Bruijn graph,
// where each node is an (n-1)-length string and each edge appends one symbol.
// Time: O(k^n), Space: O(k^n)
#include <bits/stdc++.h>
using namespace std;

string de_bruijn(int k, int n) {
  const string alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  assert(k > 0 && n > 0 && k <= (int)alphabet.size());
  if (n == 1) return alphabet.substr(0, k); // every symbol appears once

  long long nodes = 1;
  for (int i = 0; i < n - 1; i++) nodes *= k; // k^(n-1) distinct (n-1)-length nodes

  vector<int> nxt(nodes, 0);        // nxt[v] = next unused outgoing edge label from node v
  vector<pair<int, int>> stk;       // {node, edge label used to reach it}, -1 = root
  stk.push_back({0, -1});
  vector<int> circuit;              // edge labels in reverse Eulerian circuit order

  while (!stk.empty()) {
    int v = stk.back().first;
    if (nxt[v] < k) {
      int d = nxt[v]++;
      int u = (v * k + d) % nodes;  // shift v left by one symbol d (mod k^(n-1))
      stk.push_back({u, d});
    } else {
      // no more outgoing edges from v, backtrack and record the edge used to reach it
      auto p = stk.back(); stk.pop_back();
      if (p.second != -1) circuit.push_back(p.second);
    }
  }
  reverse(circuit.begin(), circuit.end());

  // the sequence starts with (n-1) copies of the first symbol (the starting node)
  string res(n - 1, alphabet[0]);
  for (int d : circuit) res.push_back(alphabet[d]);
  return res;
}

int32_t main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int k, n; cin >> k >> n;
  cout << de_bruijn(k, n) << '\n';
  return 0;
}
