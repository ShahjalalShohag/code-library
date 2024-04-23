#include<bits/stdc++.h>
using namespace std;
// https://en.wikipedia.org/wiki/Leftist_tree
// Tested on: https://judge.yosupo.jp/problem/k_shortest_walk

template<typename T>
struct heap {
  struct node{
    node* ch[2] = {0, 0};
    int sz; T val;
    node(T val): sz(1), val(val) {}

  };
  node* root = 0;
  heap(node* t = 0): root(t) {}
  node* meld(node* a, node* b) {
    if (!b) return a ? new node(*a) : 0;
    if (!a) return b ? new node(*b) : 0;
    a = new node(*a); b = new node(*b);
    if (a -> val > b -> val) swap(a, b);
    a -> ch[1] = meld(a -> ch[1], b);
    if (!a -> ch[0] || a -> ch[0] -> sz < a -> ch[1] -> sz) swap(a -> ch[0], a -> ch[1]);
    a -> sz = (a -> ch[1] ? a -> ch[1] -> sz : 0) + 1;
    return a;
  }
  heap meld(heap b) { // merging two heaps
    return heap(meld(root, b.root));
  }
  heap insert(T x) { // inserting and returing the new heap, the previous heap doesn't change
    return heap(meld(root, new node(x)));
  }
  heap pop() { // pops and returns the new heap without modifying the previous version
    return heap(meld(root -> ch[0], root -> ch[1]));
  }
  T top() { // returns the smallest value of the heap
    return root ? root -> val : T(-1);
  }
  bool empty() {
    return !root;
  }
};

heap<int> h[100];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  for (int i = 0; i < 100; i++) {
    h[i] = heap<int>();
  }
  h[1] = h[0].insert(20);
  h[2] = h[1].insert(10);
  h[3] = h[2].insert(30);

  h[4] = h[1].insert(5);
  h[4] = h[4].insert(15);
  h[5] = h[4].meld(h[3]);
  for (int i = 1; i <= 5; i++) {
    cout << h[i].top() << '\n';
  }
  cout << "h[5]:\n";
  while (h[5].top() != -1) {
    cout << h[5].top() << '\n';
    h[5] = h[5].pop();
  }
  return 0;
}
