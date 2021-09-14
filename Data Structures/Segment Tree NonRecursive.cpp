#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

int n, a[N];
struct ST {
  int t[2 * N];
  ST() {
    memset(t, 0, sizeof t);
  }
  inline int combine(int l, int r) {
    return l + r;
  }
  void build() {
    for(int i = 0; i < n; i++) t[i + n] = a[i + 1];
    for(int i = n - 1; i > 0; --i) t[i] = combine(t[i << 1], t[i << 1 | 1]);
  }
  void upd(int p, int v) {
    p--;
    for (t[p += n] = v; p >>= 1; ) t[p] = combine(t[p << 1], t[p << 1 | 1]);
  }
  int query(int l, int r) {
    int resl = 0, resr = 0;
    --l;
    for(l += n, r += n; l < r; l >>= 1, r >>= 1) {
      if(l & 1) resl = combine(resl, t[l++]);
      if(r & 1) resr = combine(t[--r], resr);
    }
    return combine(resl, resr);
  }
} t;

int32_t main() {
  n = 3;
  a[1] = 1;
  a[2] = 2;
  a[3] = 3;
  t.build();
  cout << t.query(1, 2) << '\n';
  t.upd(2, 10);
  cout << t.query(1, 3) << '\n';
  return 0;
}
