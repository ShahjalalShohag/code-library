#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 1, B = 4500, C = N / B + 1;

struct MEX {
  int cnt[N * 2], f[N * 2];
  MEX() {
    memset(cnt, 0, sizeof cnt);
    memset(f, 0, sizeof f);
  }
  void add(int x) {
    f[cnt[x]]--, ++cnt[x], f[cnt[x]]++;
  }
  void del(int x) {
    f[cnt[x]]--, --cnt[x], f[cnt[x]]++;
  }
  int get() {
    int ans = 1;
    while(f[ans] > 0) ans++;
    return ans;
  }
} t[C * (C + 1) / 2 + 10], ds;


int st[C], en[C], BC = 0;
int a[N], I[N];
int query(int l, int r) {
  int L = l / B, R = r / B;
  if(r != en[R]) R--;
  if(l != st[L]) L++;
  if(R < L) {
    for(int i = l; i <= r; i++) ds.add(a[i]);
    int ans = ds.get();
    for(int i = l; i <= r; i++) ds.del(a[i]);
    return ans;
  }
  int id = I[L * BC + R];
  for(int i = l; i < st[L]; i++) t[id].add(a[i]);
  for(int i = en[R] + 1; i <= r; i++) t[id].add(a[i]);
  int ans = t[id].get();
  for(int i = l; i < st[L]; i++) t[id].del(a[i]);
  for(int i = en[R] + 1; i <= r; i++) t[id].del(a[i]);
  return ans;
}
inline void upd(int id, int pos, int val) {
  t[id].del(a[pos]);
  t[id].add(val);
}
map <int, int> mp;
int nxt = 0;
int get(int x) {
  return mp.count(x) ? mp[x] : mp[x] = ++nxt;
}
int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  int n, q;
  cin >> n >> q;
  for (int i = 0; i < n; i++) {
    cin >> a[i];
    a[i] = get(a[i]);
  }
  for(int i = 0; i < n; i++) {
    if(i % B == 0) st[i / B] = i, BC++;
    if(i % B == B - 1 || i == n - 1) en[i / B] = i;
  }
  int nw = 0;
  for(int i = 0; i < BC; i++) {
    for(int j = i; j < BC; j++) {
      int id = nw;
      I[i * BC + j] = nw++;
      for(int p = st[i]; p <= en[j]; p++) t[id].add(a[p]);
    }
  }
  while(q--) {
    int ty;
    cin >> ty;
    if(ty == 1) {
      int l, r;
      cin >> l >> r;
      --l;
      --r;
      cout << query(l, r) << '\n';
    } else {
      int pos, val;
      cin >> pos >> val;
      --pos;
      val = get(val);
      for(int i = 0; i < BC; i++) {
        for(int j = i; j < BC; j++) {
          if(st[i] <= pos && pos <= en[j]) upd(I[i * BC + j], pos, val);
        }
      }
      a[pos] = val;
    }
  }
  return 0;
}
//https://codeforces.com/contest/940/problem/F
