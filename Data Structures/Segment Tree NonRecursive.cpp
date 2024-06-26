#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

int n, a[N];
struct Node {
    int sum;
 
    Node() { sum = 0; }
    Node(int val) { sum = val; }
};

struct ST {
  int n;
  Node *t;
  ST(int _n) { n = _n; t = new Node[2 * n]; }

  inline Node combine(Node l, Node r) {
    Node res;
    res.sum = l.sum + r.sum;
    return res;
  }

  void build() {
    for(int i = 0; i < n; i++) t[i + n] = Node(a[i+1]);
    for(int i = n - 1; i > 0; --i) t[i] = combine(t[i << 1], t[i << 1 | 1]);
  }

  void upd(int p, int v) {
    p--;
    for (t[p += n] = Node(v); p >>= 1; ) t[p] = combine(t[p << 1], t[p << 1 | 1]);
  }

  Node query(int l, int r) {
    --l;
    bool f1 = 1, f2 = 1;
    Node resl, resr;
    for(l += n, r += n; l < r; l >>= 1, r >>= 1) {
      if(l & 1) resl = f1 ? t[l++] : combine(resl, t[l++]), f1 = 0;
      if(r & 1) resr = f2 ? t[--r] : combine(t[--r], resr), f2 = 0;
    }
    if(f2) return resl;
    if(f1) return resr;
    return combine(resl, resr);
  }
};

int32_t main() {
  n = 3;
  a[1] = 1;
  a[2] = 2;
  a[3] = 3;
  ST t = ST(n);
  t.build();
  cout << t.query(1, 2).sum << '\n';
  t.upd(2, 10);
  cout << t.query(1, 3).sum << '\n';
  return 0;
}
