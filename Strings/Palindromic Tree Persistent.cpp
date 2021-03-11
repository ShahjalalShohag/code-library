#include<bits/stdc++.h>
using namespace std;

const int N = 1e6 + 9;

// cnt contains the number of palindromic suffixes of the node
// t[cur].smart_link[c] -> link to the maximum length of a palindromic suffix of t[cur] 
// s.t. ch is the immediate previous char of that suffix
struct PalindromicTree {
  static const int A = 2;
  struct node {
    int nxt[A], len, st, en, link, cnt, oc, smart_link[A];
    node() {
      memset(nxt, 0, sizeof nxt);
      for (int i = 0; i < A; i++) {
        smart_link[i] = 1;
      }
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
    if (pos - t[cur].len - 1 < 0 || s[pos - t[cur].len - 1] != c) {
      cur = t[cur].smart_link[ch];
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
      t[sz].smart_link[ch] = 2;
      return 1;
    }
    else {
      t[sz].link = t[t[cur].smart_link[ch]].nxt[ch];
      for (int i = 0; i < A; i++) {
        int x = t[sz].link;
        if (pos - t[x].len >= 0 && s[pos - t[x].len] - 'a' == i) {
          t[sz].smart_link[i] = x;
        }
        else {
          t[sz].smart_link[i] = t[x].smart_link[i];
        }
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
