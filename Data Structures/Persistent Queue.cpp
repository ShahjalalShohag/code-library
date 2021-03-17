#include<bits/stdc++.h>
using namespace std;

template<typename T>
struct PersistentArray { // 0-indexed
  struct node {
    node* l, * r; T x;
  };
  int n = 1;
  vector<node*> root;
  int build(vector<T> v) {
    while (n < v.size()) n <<= 1;
    root.push_back(build(0, n - 1, v));
    return root.size() - 1;
  }
  node* build(int l, int r, vector<T>& v) {
    node* cur = new node();
    if (l == r) {
      if (l < v.size()) cur -> x = v[l];
      else cur -> x = 0;
    }
    else {
      cur -> l = build(l, (l + r) >> 1, v);
      cur -> r = build(((l + r) >> 1) + 1, r, v);
    }
    return cur;
  }
  // get the ith value of the rth array
  T get_val(int r, int i) {
    return get_val(root[r], i, 0, n - 1);
  }
  T get_val(node* cur, int i, int l, int r) {
    if (l == r) return cur -> x;
    if (i <= ((l + r) >> 1)) return get_val(cur -> l, i, l, (l + r) >> 1);
    else return get_val(cur -> r, i, ((l + r) >> 1) + 1, r);
  }
  // update the ith value if the rth array by x and return the new array root
  int upd(int r, int i, T x) {
    root.push_back(upd(root[r], i, x, 0, n - 1));
    return root.size() - 1;
  }
  node* upd(node* pre, int i, T x, int l, int r) {
    node* cur = new node();
    if (l == r){
      cur -> x = x;
    }
    else {
      if (i <= ((l + r) >> 1)) {
        cur -> l = upd(pre -> l, i, x, l, (l + r) >> 1);
        cur -> r = pre -> r;
      }
      else {
        cur -> l = pre -> l;
        cur -> r = upd(pre -> r, i, x, ((l + r) >> 1) + 1, r);
      }
    }
    return cur;
  }
};

template<typename T>
struct PersistentQueue {
  int cur = 0; // queue ids
  vector<int> L, R;
  PersistentArray<T> st;
  PersistentQueue() {}
  PersistentQueue(int q) { // max size of a queue
    L.assign(q, 0);
    R.assign(q, 0);
    st.build(vector<T>(q, 0));
    cur++;
  }
  void push(int r, T x) { // push x at the end of the rth queue
    st.upd(r, R[r], x);
    L[cur] = L[r];
    R[cur] = R[r] + 1;
    cur++;
  }
  T pop(int r) { // pops the top element of the rth queue and returns the popped element
    st.upd(r, L[r], 0);
    L[cur] = L[r] + 1;
    R[cur] = R[r];
    cur++;
    return st.get_val(r, L[r]);
  }
};

int32_t main(){
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int q; cin >> q;
  PersistentQueue<int> Q(q + 1);
  while (q--) {
    int ty; cin >> ty;
    if (ty == 0) {
      int id, x; cin >> id >> x;
      Q.push(id + 1, x);
    }
    else {
      int id; cin >> id;
      cout << Q.pop(id + 1) << '\n';
    }
  }
}
// https://judge.yosupo.jp/problem/persistent_queue