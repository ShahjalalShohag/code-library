#include<bits/stdc++.h>
using namespace std;
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());

const int N = 3e5 + 9;

struct node {
  node *l, *r;
  int pos, key, mn, mx;
  long long val, g;
  node(int position, long long value) {
    l = r = nullptr;
    mn = mx = pos = position;
    key = rnd();
    val = g = value;
  }
  void pull() {
    g = val;
    if(l) g = __gcd(g, l->g);
    if(r) g = __gcd(g, r->g);
    mn = (l ? l->mn : pos);
    mx = (r ? r->mx : pos);
  }
};
//memory O(n)
struct treap {
  node *root;
  treap() {
    root = nullptr;
  }
  void split(node *t, int pos, node *&l, node *&r) {
    if (t == nullptr) {
      l = r = nullptr;
      return;
    }
    if (t->pos < pos) {
      split(t->r, pos, l, r);
      t->r = l;
      l = t;
    } else {
      split(t->l, pos, l, r);
      t->l = r;
      r = t;
    }
    t->pull();
  }
  node* merge(node *l, node *r) {
    if (!l || !r) return l ? l : r;
    if (l->key < r->key) {
      l->r = merge(l->r, r);
      l->pull();
      return l;
    }
    r->l = merge(l, r->l);
    r->pull();
    return r;
  }
  bool find(int pos) {
    node *t = root;
    while (t) {
      if (t->pos == pos) return true;
      if (t->pos > pos) t = t->l;
      else t = t->r;
    }
    return false;
  }
  void upd(node *t, int pos, long long val) {
    if (t->pos == pos) {
      t->val = val;
      t->pull();
      return;
    }
    if (t->pos > pos) upd(t->l, pos, val);
    else upd(t->r, pos, val);
    t->pull();
  }
  void insert(int pos, long long val) { //set a_pos = val
    if (find(pos)) upd(root, pos, val);
    else {
      node *l, *r;
      split(root, pos, l, r);
      root = merge(merge(l, new node(pos, val)), r);
    }
  }
  long long query(node *t, int st, int en) {
    if (t->mx < st || en < t->mn) return 0;
    if (st <= t->mn && t->mx <= en) return t->g;
    long long ans = (st <= t->pos && t->pos <= en ? t->val : 0);
    if (t->l) ans = __gcd(ans, query(t->l, st, en));
    if (t->r) ans = __gcd(ans, query(t->r, st, en));
    return ans;
  }
  long long query(int l, int r) { //gcd of a_i such that l <= i <= r
    if (!root) return 0;
    return query(root, l, r);
  }
  void print(node *t) {
    if (!t) return;
    print(t->l);
    cout << t->val << " ";
    print(t->r);
  }
};
//total memory along with treap = nlogn
struct ST {
  ST *l, *r;
  treap t;
  int b, e;
  ST() {
    l = r = nullptr;
  }
  ST(int st, int en) {
    l = r = nullptr;
    b = st, e = en;
  }
  void fix(int pos) {
    long long val = 0;
    if (l) val = __gcd(val, l->t.query(pos, pos));
    if (r) val = __gcd(val, r->t.query(pos, pos));
    t.insert(pos, val);
  }
  void upd(int x, int y, long long val) { //set a[x][y] = val
    if (e < x || x < b) return;
    if (b == e) {
      t.insert(y, val);
      return;
    }
    if (b != e) {
      if (x <= (b + e) / 2) {
        if (!l) l = new ST(b, (b + e) / 2);
        l->upd(x, y, val);
      } else {
        if (!r) r = new ST((b + e) / 2 + 1, e);
        r->upd(x, y, val);
      }
    }
    fix(y);
  }
  long long query(int i, int j, int st, int en) { //gcd of a[x][y] such that i <= x <= j && st <= y <= en
    if (e < i || j < b) return 0;
    if (i <= b && e <= j) return t.query(st, en);
    long long ans = 0;
    if (l) ans = __gcd(ans, l->query(i, j, st, en));
    if (r) ans = __gcd(ans, r->query(i, j, st, en));
    return ans;
  }
};

int32_t main() {
  int n, m;
  cin >> n >> m;
  ST t(0, n - 1);
  int q;
  cin >> q;
  while(q--) {
    int ty;
    cin >> ty;
    if(ty == 1) {
      int x, y, v;
      cin >> x >> y >> v;
      t.upd(x, y, v);
    } else {
      int x1, y1, x2, y2;
      cin >> x1 >> y1 >> x2 >> y2;
      cout << t.query(x1, x2, y1, y2) << '\n';
    }
  }
  return 0;
}
//https://oj.uz/problem/view/IOI13_game
