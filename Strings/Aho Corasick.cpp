#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;

//credit: Alpha_Q
struct AC {
  int N, P;
  const int A = 26;
  vector <vector <int>> next;
  vector <int> link, out_link; // out_link[v] = nearest ancestor of v where an input pattern ended which is also a suffix link of v.
  vector <vector <int>> out;
  AC(): N(0), P(0) {node();}
  int node() {
    next.emplace_back(A, 0);
    link.emplace_back(0);
    out_link.emplace_back(0);
    out.emplace_back(0);
    return N++;
  }
  inline int get (char c) {
    return c - 'a';
  }
  int add_pattern (const string T) {
    int u = 0;
    for (auto c : T) {
      if (!next[u][get(c)]) next[u][get(c)] = node();
      u = next[u][get(c)];
    }
    out[u].push_back(P);
    return P++;
  }
  void compute() {
    queue <int> q;
    for (q.push(0); !q.empty();) {
      int u = q.front(); q.pop();
      for (int c = 0; c < A; ++c) {
        int v = next[u][c];
        if (!v) next[u][c] = next[link[u]][c];
        else {
          link[v] = u ? next[link[u]][c] : 0;
          out_link[v] = out[link[v]].empty() ? out_link[link[v]] : link[v];
          q.push(v);
        }
      }
    }
  }
  int advance (int u, char c) {
    while (u && !next[u][get(c)]) u = link[u];
    u = next[u][get(c)];
    return u;
  }
};

// Problem:  You are given n patterns (sum of the lengths of which is sum <= 10^5), and also a text string s. 
// Find the minimum number of substring partitions that we can have of the text string such that each partition exists in the given patterns.
// This code solves it in O(len(s) * sqrt(sum)) by utilizing the fact that at most O(sqrt(sum)) distinct patterns can end at a specific index of s
// And using out_link it becomes trivial to brute force over all such patterns
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  auto st = clock();
  int t, cs = 0; cin >> t;
  while (t--) {
    int n; cin >> n;
    vector<string> v;
    for (int i = 0; i < n; i++) {
      string s; cin >> s;
      v.push_back(s);
    }
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    AC aho;
    vector<int> len(n + 3, 0);
    for (auto s: v) {
      len[aho.add_pattern(s)] = s.size();
    }
    aho.compute();
    string s; cin >> s;
    n = s.size();
    vector<int> dp(n, n + 10);
    int u = 0;
    for (int i = 0; i < n; i++) {
      char c = s[i];
      u = aho.advance(u, c);
      for (int v = u; v; v = aho.out_link[v]) {
        for (auto p : aho.out[v]) { 
          dp[i] = min(dp[i], (i - len[p] >= 0 ? dp[i - len[p]] : 0) + 1);
        }
      }
    }
    cout << "Case " << ++cs << ": ";
    if (dp[n - 1] == n + 10) {
      cout << "impossible\n";
    }
    else {
      cout << dp[n - 1] << '\n';
    }
  }
  cout << 1.0 * (clock() - st) / 1000 << '\n';
  return 0;
}
