#include<bits/stdc++.h>
using namespace std;

const int N = 2e5 + 9;
// there is a directed edge from bracket i to bracket j if j is directly inside i
vector<int> g[N]; 
int sz[N];
void dfs(int u) {
  sz[u] = 1;
  for (auto v: g[u]) {
    dfs(v);
    sz[u] += sz[v];
  }
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t; cin >> t;
  while (t--) {
    int k; cin >> k;
    string s; cin >> s;
    int bracket_id = 0;
    vector<int> roots;
    vector<int> st;
    for (auto c: s) {
      if (c == '(') {
        st.push_back(++bracket_id);
      }
      else {
        int cur_bracket_id = st.back();
        st.pop_back();
        if (!st.empty()) {
          int par_bracket_id = st.back();
          g[par_bracket_id].push_back(cur_bracket_id);
        }
        else {
          roots.push_back(cur_bracket_id);
        }
      }
    }
    for (int r: roots) {
      dfs(r);
    }
    sort(sz + 1, sz + bracket_id + 1);
    long long ans = 0;
    for (int i = 1; i <= bracket_id - k; i++) {
      ans += sz[i] - 1;
    }
    cout << ans << '\n';

    for (int i = 1; i <= bracket_id; i++) {
      g[i].clear();
    }
  }
  return 0;
}
// https://codeforces.com/contest/1821/problem/E
