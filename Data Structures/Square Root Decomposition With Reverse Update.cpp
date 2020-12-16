#include<bits/stdc++.h>
using namespace std;
  
const int N = 1e5 + 9, B = 330;

struct Block {
  vector<int> v, sorted_v; int sz = 0;
  int assigned = -1;
  bool rev = false;
  Block() {}
  Block(int s, int to_assign, bool to_rev = false) {
    sz = s; rev = to_rev;
    assigned = to_assign;
  }
  Block(vector<int> &a, int to_assign = -1, bool to_rev = false) {
    v = a; assigned = to_assign;
    sz = a.size();
    rev = to_rev;
    sorted_v = v;
    sort(sorted_v.begin(), sorted_v.end());
  }
  int get(int x) {
    if (assigned != -1) {
      return assigned >= x ? sz : 0;
    }
    return sorted_v.size() - (lower_bound(sorted_v.begin(), sorted_v.end(), x) - sorted_v.begin());
  }
  int size() {
    return sz;
  }
  void perform_reverse() {
    rev ^= true;
    if (assigned != -1) return;
    reverse(v.begin(), v.end());
  }
  void truncate(int k) {
    sz = k;
    if (rev) {
      perform_reverse();
    }
    if (assigned != -1) return;
    v.resize(k);
    sorted_v = v;
    sort(sorted_v.begin(), sorted_v.end());
  }
}t[2000];
vector<int> a, blocks;
int n, T;
inline void rebuild() {
  T = 0; blocks.clear();
  for (int i = 1; i <= n; i += B) {
    int l = i, r = min(i + B - 1, n);
    vector<int> cur(a.begin() + l, a.begin() + r + 1);
    t[++T] = Block(cur);
    blocks.push_back(T);
  }
}
// returns such 𝑗(0 indexed), that the 𝑖-the element is the beginning of the 𝑗-th segment
int split(int i) {
  if (i > n) return blocks.size();
  int cur = 1;
  for (auto it = blocks.begin(); it != blocks.end(); ++it) {
    int id = *it;
    int cl = cur, cr = cur + t[id].size() - 1;
    if (cl <= i && i <= cr) {
      if (i == cl) {
        return it - blocks.begin();
      }
      else {
        if (t[id].rev) {
          t[id].perform_reverse();
        }
        int k = i - cl;
        int cur = ++T;
        if (t[id].assigned != -1) {
          t[cur] = Block(cr - cl + 1 - k, t[id].assigned);
        }
        else {
          vector<int> tmp = vector<int> (t[id].v.begin() + k, t[id].v.end());
          t[cur] = Block(tmp);
        }
        t[id].truncate(k);
        ++it;
        it = blocks.insert(it, cur);
        return it - blocks.begin();
      }
    }
    cur = cr + 1;
  }
  return 0;
}
// insert x into the i-th index
void insert(int i, int x) {
  a[n++] = x;
  int j = split(i);
  int cur = ++T; vector<int> b({x});
  t[cur] = Block(b);
  blocks.insert(blocks.begin() + j, cur);
}
// erase the i-th element
void erase(int i) {
  int j = split(i);
  split(i + 1);
  blocks.erase(blocks.begin() + j);
}
int ty[N], l[N], r[N], x[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  cin >> n;
  a.resize(n + 1);
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
    a[i]++;
  }
  int q; cin >> q;
  for (int i = 1; i <= q; i++) {
    string p; cin >> p;
    if (p == "get") {
      ty[i] = 3;
      cin >> l[i] >> r[i] >> x[i];
      x[i]++;
    }
    else if (p == "set") {
      ty[i] = 1;
      cin >> l[i] >> r[i] >> x[i];
      x[i]++;
    }
    else {
      ty[i] = 2;
      cin >> l[i] >> r[i];
    }
  }
  rebuild();
  for (int i = 1; i <= q; i++) {
    if (i % 600 == 0) {
      int cur = 1;
      a.resize(1);
      for (auto x: blocks) {
        if (t[x].rev) {
          t[x].perform_reverse();
        }
        if (t[x].assigned == -1) {
          for (auto y: t[x].v) {
            a.push_back(y);
          }
        }
        else {
          for (int j = 0; j < t[x].sz; j++) {
            a.push_back(t[x].assigned);
          }
        }
      }
      rebuild();
    }
    if (ty[i] == 1) { // for all a_i: l <= i <= r, set a_i = x
      int p = split(l[i]), q = split(r[i] + 1);
      while (p < q) {
        t[blocks[p]].assigned = x[i];
        ++p;
      }
    }
    else if (ty[i] == 3) { // find the number of elements in [l, r] s.t. a_i >= x
      int ans = 0;
      int p = split(l[i]), q = split(r[i] + 1);
      while (p < q) {
        ans += t[blocks[p]].get(x[i]);
        ++p;
      }
      cout << ans << '\n';
    }
    else { // reverse a subarray
      int p = split(l[i]), q = split(r[i] + 1);
      for (int j = p; j < q; j++) {
        t[blocks[j]].rev ^= true;
      }
      reverse(blocks.begin() + p, blocks.begin() + q);
    }
  }
  return 0;
}
// https://www.e-olymp.com/en/problems/2961
