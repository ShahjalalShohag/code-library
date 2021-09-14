#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;
//works for multiple values too
struct STM {
#define lc t[cur].l
#define rc t[cur].r
  struct node {
    int l, r, sz;
  } t[N * 30];
  int T;
  int id[N * 30];
  STM() {
    T = 0;
    for (int i = 1; i < N * 30; i++) id[i] = i;
  }
  inline int new_node() {
    int cur = id[++T];
    t[cur].l = t[cur].r = t[cur].sz = 0;
    return cur;
  }
  inline void save_memory(int x) {
    id[T--] = x;
  }
  //create a segment tree with only node i
  int build(int b, int e, int i) {
    int cur = new_node();
    if (b == e) {
      t[cur].sz = 1;
      return cur;
    }
    int mid = b + e >> 1;
    if(i <= mid) lc = build(b, mid, i);
    else rc = build(mid + 1, e, i);
    t[cur].sz = t[lc].sz + t[rc].sz;
    return cur;
  }
  //merge segment tree a and b
  int merge(int a, int b) {
    if(!a || !b) return a ^ b;
    t[a].l = merge(t[a].l, t[b].l);
    t[a].r = merge(t[a].r, t[b].r);
    t[a].sz += t[b].sz;
    save_memory(b);
    return a;
  }
  //split cur into cur and b so that cur contains k smallest elements
  void split(int cur, int &b, int k) {
    if (t[cur].sz < k) return;
    b = new_node();
    int lsz = t[lc].sz;
    if(k > lsz) split(rc, t[b].r, k - lsz);
    else swap(rc, t[b].r);
    if(k < lsz) split(lc, t[b].l, k);
    t[b].sz = t[cur].sz - k;
    t[cur].sz = k;
  }
  //k -th sorted element in cur segment tree
  int query(int cur, int b, int e, int k) {
    if(b == e) return b;
    int lsz = t[lc].sz;
    int mid = b + e >> 1;
    if(k > lsz) return query(rc, mid + 1, e, k - lsz);
    return query(lc, b, mid, k);
  }
} t;
bool ty[N]; //0 - inc; 1 - dec
//i-th pos store a substr (l, r) such that l = i (if exists)
//roots[i] stores the root, rb[i] stores the right border
int root[N], rb[N];
set<int> cur; //maintains the left borders of ordered substrs
int n, q, a[N];
//split (l, ...) so that rb[l] = r
void split(int l, int r) {
  if (r >= rb[l] || r < l) return;
  if (!ty[l]) t.split(root[l], root[r + 1], r - l + 1);
  else {
    root[r + 1] = root[l];
    t.split(root[r + 1], root[l], rb[l] - r); //checkmate
  }
  rb[r + 1] = rb[l];
  rb[l] = r;
  cur.insert(r + 1);
  ty[r + 1] = ty[l];
}
//merge adjacent substr a and b to a (ty[a] should be edited manually)
void merge(int a, int b) {
  if(a == b) return;
  cur.erase(b);
  root[a] = t.merge(root[a], root[b]);
  rb[a] = rb[b];
}
//query for a[k] on substr (l, ...)
int query(int l, int k) {
  k -= l - 1;
  if(!ty[l]) return t.query(root[l], 1, n, k);
  return t.query(root[l], 1, n, rb[l] - l + 1 - k + 1);
}
int main() {
  scanf("%d %d", &n, &q);
  for(int i = 1; i <= n; i++) {
    scanf("%d",  &a[i]);
    root[i] = t.build(1, n, a[i]);
    cur.insert(i);
    rb[i] = i;
  }

  while (q--) {
    //sort range [l, r]
    //type: 0 - inc  1 - dec
    int type, l, r;
    scanf("%d %d %d", &type, &l, &r);
    //split l and r
    set<int>::iterator it = cur.upper_bound(l);
    split(*(--it), l - 1);
    it = cur.upper_bound(r);
    split(*(--it), r);
    //get the range
    set<int>::iterator L = cur.lower_bound(l), R = cur.upper_bound(r);
    --R;
    int nw = *L;
    vector<int> v;
    while (L != R) {
      ++L;
      int k = *L;
      v.push_back(k);
      if (L == R) break;
    }
    for (int i = 0; i < v.size(); i++) merge(nw, v[i]);
    ty[nw] = type;
  }
  int i;
  scanf("%d", &i); //query for the i-th position of the sequence
  set<int>::iterator it = cur.upper_bound(i);
  int l = *(--it);
  printf("%d\n", query(l, i));
  return 0;
}
// https://codeforces.com/blog/entry/49446
// http://www.lydsy.com:808/JudgeOnline/problem.php?id=4552
/*
The problem is maintaing a permutation of 1~n,
supporting range sorting (increasing or decreasing),
query the value of **one** position **after all sortings**.
*/
