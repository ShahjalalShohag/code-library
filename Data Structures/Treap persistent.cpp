#include<bits/stdc++.h>
using namespace std;

const int N = 5e4 + 9;

//Implicit Persistent Treap
//0-indexed
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
struct node {
  node *l, *r;
  int val, prior, sz;
  node() {}
  node(int id) {
    l = r = nullptr;
    val = id;
    prior = rnd();
    sz = 1;
  }
};
namespace PersistentTreap {
inline void copy(node* &a, node* b) {
  if (!b) a = nullptr;
  else {
    a = new node();
    *a = *b;
  }
}
int size(node* t) {
  return t ? t->sz : 0;
}
void update_size(node* &t) {
  if(t) t->sz = size(t->l) + size(t->r) + 1;
}
void split(node *cur, int pos, node *&l, node *&r, int add = 0) {
  node* t;
  copy(t, cur);
  if (t == nullptr) {
    l = r = nullptr;
    return;
  }
  int id = add + size(t -> l);
  if (id <= pos) {
    split(t->r, pos, l, r, id + 1);
    t->r = l;
    l = t;
  } else {
    split(t->l, pos, l, r, add);
    t->l = r;
    r = t;
  }
  update_size(t);
}
node* merge(node *l, node *r) {
  node* t;
  if (!l || !r) copy(t, l ? l : r);
  else if (l->prior < r->prior) {
    copy(t, l);
    t->r = merge(t->r, r);
  } else {
    copy(t, r);
    t->l = merge(l, t->l);
  }
  update_size(t);
  return t;
}
node* insert(node* t, int pos, int val) {
  node *l, *r;
  split(t, pos - 1, l, r);
  return merge(merge(l, new node(val)), r);
}
node* insert(node* t, int pos, node* x) {
  node *l, *r;
  split(t, pos - 1, l, r);
  return merge(merge(l, x), r);
}
node* erase(node* t, int L, int R) {
  node *l, *r, *mid, *mr;
  split(t, L - 1, l, r);
  split(r, R - L, mid, mr);
  return merge(l, mr);
}
//creating a treap for string s in O(n)
node* build(int l, int r, string &s) {
  int mid = l + r >> 1;
  node* cur = new node(s[mid]);
  if (l < mid) cur -> l = build(l, mid - 1, s);
  if (mid < r) cur -> r = build(mid + 1, r, s);
  update_size(cur);
  return cur;
}
int c = 0;
void print_nodes(node* t) {
  if (!t) return;
  print_nodes(t -> l);
  char ch = t -> val;
  c += ch == 'c';
  cout << ch;
  print_nodes(t -> r);
}
void print(node* t, int L, int R) {
  node *l, *r, *mid, *mr;
  split(t, L - 1, l, r);
  split(r, R - L, mid, mr);
  print_nodes(mid);
}
}
using namespace PersistentTreap;
node* root[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int version = 0;
  int q;
  cin >> q;
  while (q --) {
    int ty;
    cin >> ty;
    if (ty == 1) {
      int k;
      string s;
      cin >> k >> s;
      k -= c;
      node* t = build(0, (int)s.size() - 1, s);
      root[version + 1] = insert(root[version], k, t);
      version++;
    } else if (ty == 2) {
      int l, len;
      cin >> l >> len;
      l -= c, len -= c;
      --l;
      root[version + 1] = erase(root[version], l, l + len - 1);
      version++;
    } else {
      int v, l, len;
      cin >> v >> l >> len;
      v -= c, l -= c, len -= c;
      --l;
      print(root[v], l, l + len - 1);
      cout << '\n';
    }
  }
  return 0;
}
//https://vjudge.net/problem/UVA-12538
