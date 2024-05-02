#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

/*
-> diff(v) = len(v) - len(link(v))
-> series link will lead from the vertex v to the vertex u corresponding
   to the maximum suffix palindrome of v which satisfies diff(v) != diff(u)
-> path within series links to the root contains only O(log n) vertices
-> cnt contains the number of palindromic suffixes of the node
*/
struct PalindromicTree {
  struct node {
    int nxt[26], len, st, en, link, diff, slink, cnt, oc;
  };
  string s;
  vector<node> t;
  int sz, last;
  PalindromicTree() {}
  PalindromicTree(string _s) {
    s = _s;
    int n = s.size();
    t.clear();
    t.resize(n + 9);
    sz = 2, last = 2;
    t[1].len = -1, t[1].link = 1;
    t[2].len = 0, t[2].link = 1;
    t[1].diff = t[2].diff = 0;
    t[1].slink = 1;
    t[2].slink = 2;
  }
  int extend(int pos) { // returns 1 if it creates a new palindrome
    int cur = last, curlen = 0;
    int ch = s[pos] - 'a';
    while (1) {
      curlen = t[cur].len;
      if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == s[pos]) break;
      cur = t[cur].link;
    }
    if (t[cur].nxt[ch]) {
      last = t[cur].nxt[ch];
      t[last].oc++;
      return 0;
    }
    sz++;
    last = sz;
    t[sz].oc = 1;
    t[sz].len = t[cur].len + 2;
    t[cur].nxt[ch] = sz;
    t[sz].en = pos;
    t[sz].st = pos - t[sz].len + 1;
    if (t[sz].len == 1) {
      t[sz].link = 2;
      t[sz].cnt = 1;
      t[sz].diff = 1;
      t[sz].slink = 2;
      return 1;
    }
    while (1) {
      cur = t[cur].link;
      curlen = t[cur].len;
      if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == s[pos]) {
        t[sz].link = t[cur].nxt[ch];
        break;
      }
    }
    t[sz].cnt = 1 + t[t[sz].link].cnt;
    t[sz].diff = t[sz].len - t[t[sz].link].len;
    if (t[sz].diff == t[t[sz].link].diff) t[sz].slink = t[t[sz].link].slink;
    else t[sz].slink = t[sz].link;
    return 1;
  }
  void calc_occurrences() {
    for (int i = sz; i >= 3; i--) t[t[i].link].oc += t[i].oc;
  }
  vector<array<int, 2>> minimum_partition() { //(even, odd), 1 indexed
    int n = s.size();
    vector<array<int, 2>> ans(n + 1, {0, 0}), series_ans(n + 5, {0, 0});
    ans[0][1] = series_ans[2][1] = 1e9;
    for (int i = 1; i <= n; i++) {
      extend(i - 1);
      for (int k = 0; k < 2; k++) {
        ans[i][k] = 1e9;
        for (int v = last; t[v].len > 0; v = t[v].slink) {
          series_ans[v][!k] = ans[i - (t[t[v].slink].len + t[v].diff)][!k];
          if (t[v].diff == t[t[v].link].diff) series_ans[v][!k] = min(series_ans[v][!k], series_ans[t[v].link][!k]);
          ans[i][k] = min(ans[i][k], series_ans[v][!k] + 1);
        }
      }
    }
    return ans;
  }
} t;

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  string s;
  cin >> s;
  PalindromicTree t(s);
  for (int i = 0; i < s.size(); i++) t.extend(i);
  t.calc_occurrences();
  long long ans = 0;
  for (int i = 3; i <= t.sz; i++) ans += t.t[i].oc;
  cout << ans << '\n';
  return 0;
  //auto ans = t.minimum_partition();
  // for (int i = 1; i <= s.size(); i++) {
  //     cout << (ans[i][1] == 1e9 ? -1 : ans[i][1]) << ' ';
  //     cout << (ans[i][0] == 1e9 ? -2 : ans[i][0]) << '\n';
  // }
  return 0;
}
// https://acm.timus.ru/problem.aspx?space=1&num=2058
