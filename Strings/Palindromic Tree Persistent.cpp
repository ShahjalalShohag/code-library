#include<bits/stdc++.h>
using namespace std;

const int N = 1e6 + 9;

// cnt contains the number of palindromic suffixes of the node
struct PalindromicTree {
  struct node {
    int nxt[2], len, st, en, link, cnt, oc;
    node() {
      memset(nxt, 0, sizeof nxt);
    }
  };
  string s; vector<node> t;
  vector<array<int, 4>> changes;
  int sz, last;
  vector<int> pref;
  PalindromicTree() {
    s = ""; t.resize(3);
    sz = 2, last = 2;
    t[1].len = -1, t[1].link = 1;
    t[2].len = 0, t[2].link = 1;
    changes.clear();
  }
  int extend(char c) {
    int cur = last, curlen = 0, pos = s.size();
    pref.push_back(0);
    if (pos) {
      pref[pos] = pref[pos - 1];
    }
    s += c; int ch = c - 'a';
    while (1) {
      curlen = t[cur].len;
      if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == s[pos]) break;
      cur = t[cur].link;
    }
    if (t[cur].nxt[ch]) {
      changes.push_back({last, t[cur].nxt[ch], -1, -1});
      last = t[cur].nxt[ch];
      t[last].oc++;
      return 0;
    }
    changes.push_back({last, -1, cur, ch});
    sz++; last = sz;
    t.emplace_back();
    t[sz].oc = 1; t[sz].len = t[cur].len + 2;
    pref[pos] = max(pref[pos], t[sz].len);
    t[cur].nxt[ch] = sz;
    t[sz].en = pos; t[sz].st = pos - t[sz].len + 1;
    if (t[sz].len == 1) {
      t[sz].link = 2; t[sz].cnt = 1;
      return 1;
    }
    while (1) {
      cur = t[cur].link; curlen = t[cur].len;
      if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == s[pos]) {
        t[sz].link = t[cur].nxt[ch];
        break;
      }
    }
    t[sz].cnt = 1 + t[t[sz].link].cnt;
    return 1;
  }
  void rollback() {
    if (s.size() == 0) return;
    s.pop_back();
    pref.pop_back();
    auto x = changes.back();
    int prvlast = x[0], oc = x[1], c = x[2], ch = x[3]; 
    changes.pop_back();
    last = prvlast;
    if (oc == -1) {
      t[c].nxt[ch] = 0;
      t.pop_back();
      sz--;
    }
    else {
      t[oc].oc--;
    }
  }
}t;
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int q; cin >> q;
  while (q--) {
    int ty; cin >> ty;
    if (ty == 1) {
      int k; cin >> k;
      t.extend(char(k + 'a'));
    }
    else {
      t.rollback();
    }
    cout << (t.s.empty() ? 0 : t.pref[t.s.size() - 1]) << '\n';
  }
  return 0;
}
// https://www.codechef.com/problems/BINPALIN