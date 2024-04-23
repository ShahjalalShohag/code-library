template<typename T>
struct heap {
  struct node{
    node* ch[2] = {0, 0};
    int sz; T val;
    int from, to;
    node(T val, int from, int to): sz(1), val(val), from(from), to(to) {}
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
  heap meld(heap b) {
    return heap(meld(root, b.root));
  }
  heap insert(T x, int from, int to) { // from and to is needed only for this k_shortest_walk problem
    return heap(meld(root, new node(x, from, to)));
  }
  heap pop() {
    return heap(meld(root -> ch[0], root -> ch[1]));
  }
  T top() { // returns the smallest value of the heap
    return root ? root -> val : T(-1);
  }
  bool empty() {
    return !root;
  }
};

// https://en.wikipedia.org/wiki/Leftist_tree
// Tehttps://judge.yosupo.jp/problem/k_shortest_walk
