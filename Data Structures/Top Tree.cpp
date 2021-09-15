#include <bits/stdc++.h>
using namespace std;

// credit: ecnerwala
struct top_tree_node {
  mutable top_tree_node* p = nullptr;
  top_tree_node* c[3] = {nullptr, nullptr, nullptr};

  int d() const {
    assert(p);
    if (this == p->c[0]) {
      return 0;
    } else if (this == p->c[1]) {
      return 1;
    } else if (this == p->c[2]) {
      return 2;
    } else assert(false);
  }
  top_tree_node*& p_c() const { return p->c[d()]; } // p->c which points to you

  // 3 types of verts: path edges, path verts, non-path edges
  bool is_path;
  bool is_vert;

  bool r() const { return !p || p->is_path != is_path; }

  int __id;

  bool flip_path = false;
  int64_t subtree_size = 0;
  int64_t path_size = 0;
  int64_t subtree_lazy = 0;
  int64_t path_lazy = 0;
  int64_t tot_A = 0;
  int64_t own_A = 0;
  std::array<int64_t, 2> tot_sub_d = {0,0};
  std::array<int64_t, 2> tot_ans = {0,0};

  void do_flip_path() {
    assert(is_path);
    flip_path ^= 1;
    std::swap(tot_sub_d[0], tot_sub_d[1]);
    std::swap(tot_ans[0], tot_ans[1]);
  }

  void do_subtree_increment(int64_t v = 1) {
    //cerr << "doing subtree increment " << __id << ' ' << v << '\n';
    subtree_lazy += v;
    if (is_vert) own_A += v;
    tot_A += subtree_size * v;
    tot_ans[0] += tot_sub_d[0] * v;
    tot_ans[1] += tot_sub_d[1] * v;
  }

  void do_path_increment(int64_t v = 1) {
    //cerr << "doing path increment " << __id << ' ' << v << '\n';
    assert(is_path);
    path_lazy += v;
    if (is_vert) own_A += v;
    tot_A += path_size * v;
    tot_ans[0] += (path_size * (path_size-1) / 2) * v;
    tot_ans[1] += (path_size * (path_size-1) / 2) * v;
  }

  void downdate() {
    // fortunately, this doesn't interact with our ops
    if (flip_path) {
      assert(is_path);
      if (!is_vert) {
        if (c[0]) c[0]->do_flip_path();
        if (c[1]) c[1]->do_flip_path();
      }
      std::swap(c[0], c[1]);
      flip_path = false;
    }

    if (subtree_lazy) {
      if (c[0]) c[0]->do_subtree_increment(subtree_lazy);
      if (c[1]) c[1]->do_subtree_increment(subtree_lazy);
      if (c[2]) c[2]->do_subtree_increment(subtree_lazy);
      subtree_lazy = 0;
    }

    if (path_lazy) {
      assert(is_path);
      if (!is_vert) {
        //std::cerr << "downdating path lazy" << ' ' << __id << ' ' << path_lazy << '\n';
        assert(c[0] && c[1] && !c[2]);
        c[0]->do_path_increment(path_lazy);
        c[1]->do_path_increment(path_lazy);
      }
      path_lazy = 0;
    }
  }

  void update() {
    assert(!flip_path && !subtree_lazy && !path_lazy);
    if (is_vert) assert(is_path);

    subtree_size = is_vert;
    for (int z = 0; z <= 2; z++) {
      if (c[z]) subtree_size += c[z]->subtree_size;
    }

    if (!is_path) {
      path_size = 0;
    } else {
      path_size = is_vert;
      if (!is_vert) {
        for (int z = 0; z < 2; z++) {
          if (c[z]) path_size += c[z]->path_size;
        }
      }
      assert(path_size >= 1);
    }

    tot_A = is_vert ? own_A : 0;
    for (int z = 0; z <= 2; z++) {
      if (c[z]) tot_A += c[z]->tot_A;
    }

    if (is_path && !is_vert) {
      assert(c[0] && c[1]);
      assert(!c[2]);
      for (int z = 0; z < 2; z++) {
        tot_sub_d[z] = c[z]->tot_sub_d[z] + c[z]->path_size * c[!z]->subtree_size + c[!z]->tot_sub_d[z];
        tot_ans[z] = c[z]->tot_ans[z] + c[z]->path_size * c[!z]->tot_A + c[!z]->tot_ans[z];
      }
    } else {
      tot_sub_d[0] = 0;
      tot_ans[0] = 0;
      for (int z = 0; z < 2; z++) {
        if (c[z]) {
          tot_sub_d[0] += c[z]->tot_sub_d[0];
          tot_ans[0] += c[z]->tot_ans[0];
        }
      }

      if (c[2]) {
        assert(!is_path);
        tot_sub_d[0] += c[2]->subtree_size + c[2]->tot_sub_d[0];
        tot_ans[0] += c[2]->tot_A + c[2]->tot_ans[0];
      }

      tot_sub_d[1] = tot_sub_d[0];
      tot_ans[1] = tot_ans[0];
    }
  }

  void downdate_all() {
    if (p) p->downdate_all();
    downdate();
  }

  void update_all() {
    update();
    if (p) p->update_all();
  }

private:

  void rot() {
    assert(!is_vert);
    assert(!r());
    top_tree_node* pa = p;
    int x = d(); assert(x == 0 || x == 1);
    top_tree_node* ch = c[!x];

    if (pa->p) pa->p_c() = this;
    this->p = pa->p;

    pa->c[x] = ch;
    if (ch) ch->p = pa;

    this->c[!x] = pa;
    pa->p = this;

    pa->update();
  }

  void rot_2(int c_d) {
    assert(!is_vert);
    assert(!r());
    assert(c[c_d]);
    assert(!c[c_d]->is_vert);

    if (d() == c_d) {
      rot();
      return;
    }

    top_tree_node* pa = p;
    int x = d(); assert(x == 0 || x == 1);
    assert(c_d == !x);
    top_tree_node* ch = c[c_d]->c[!x];

    if (pa->p) pa->p_c() = this;
    this->p = pa->p;

    pa->c[x] = ch;
    if (ch) ch->p = pa;

    this->c[c_d]->c[!x] = pa;
    pa->p = this->c[c_d];

    pa->update();
  }

  void splay_dir(int x) {
    while (!r() && d() == x) {
      if (!p->r() && p->d() == x) {
        p->rot();
      }
      rot();
    }
  }

  void splay_2(int c_d) {
    assert(!is_vert && is_path);
    assert(c[c_d] && !c[c_d]->is_vert);
    while (!r()) {
      if (!p->r()) {
        if (p->d() == d()) {
          p->rot();
        } else {
          rot_2(c_d);
        }
      }
      rot_2(c_d);
    }
  }

  void splay_2() {
    assert(!is_vert && is_path);
    assert(!r());
    p->splay_2(d());
  }

  void splay_vert() {
    assert(is_vert);
    if (r()) {
      return;
    }
    p->splay_dir(d());
    if (p->r()) {
      return;
    }

    assert(p->d() != d());
    // we have a preference to be the left child
    if (d() == 1) {
      p->rot();
    }
    assert(d() == 0);

    p->splay_2();
    assert(d() == 0);
    assert(p->d() == 1);
    assert(p->p->r());
  }

  void splay() {
    assert(!is_vert);
    while (!r()) {
      if (!p->r()) {
        if (p->d() == d()) {
          p->rot();
        } else {
          rot();
        }
      }
      rot();
    }
  }

  top_tree_node* cut_right() {
    assert(is_vert && is_path);
    splay_vert();

    if (r() || d() == 1) {
      assert(r() || (d() == 1 && p->r()));
      assert(c[0] == nullptr);
      return nullptr;
    }

    top_tree_node* pa = p;
    assert(pa->r() || (pa->d() == 1 && pa->p->r()));
    assert(!pa->is_vert);
    assert(pa->is_path);
    assert(pa->c[0] == this);
    assert(pa->c[2] == nullptr);

    if (pa->p) pa->p_c() = this;
    this->p = pa->p;

    pa->is_path = false;
    pa->c[2] = pa->c[1]; // don't need to change the parent

    pa->c[0] = c[0]; if (c[0]) c[0]->p = pa;
    pa->c[1] = c[1]; if (c[1]) c[1]->p = pa;

    c[0] = nullptr;
    c[1] = pa; pa->p = this;
    assert(c[2] == nullptr);

    assert(c[0] == nullptr);

    pa->update();
    return pa;
  }

  top_tree_node* splice_non_path() {
    assert(!is_path);
    assert(!is_vert);

    splay();
    assert(p && p->is_vert && p->is_path);
    p->cut_right();

    if (!p->is_path) rot();
    assert(p && p->is_vert && p->is_path);
    assert(p->r() || (p->d() == 1 && p->p->r()));
    assert(p->c[d()] == this && p->c[!d()] == nullptr);

    top_tree_node* pa = p;

    if (pa->p) pa->p_c() = this;
    this->p = pa->p;

    pa->c[0] = c[0]; if (c[0]) c[0]->p = pa;
    pa->c[1] = c[1]; if (c[1]) c[1]->p = pa;

    assert(c[2] && c[2]->is_path);
    c[1] = c[2]; // don't need to change parent
    c[0] = pa; pa->p = this;
    c[2] = nullptr;

    is_path = true;

    pa->update();
    return pa;
  }

  void splice_all(top_tree_node*& res) {
    if (!is_path) {
      res = splice_non_path();
    }
    assert(is_path);
    if (!p) return;
    p->splice_all(res);
  }

public:
  top_tree_node* expose() {
    assert(is_vert);
    downdate_all();

    top_tree_node* res = nullptr;
    splice_all(res);

    cut_right();

    update_all();

    return res;
  }

  void meld_path_end() {
    assert(!p);
    top_tree_node* rt = this;
    while (true) {
      rt->downdate();
      if (rt->is_vert) break;
      rt = rt->c[1];
    }
    assert(rt->is_vert);
    rt->splay_vert();
    if (rt->c[0] && rt->c[1]) {
      top_tree_node* ch = rt->c[1];
      while (true) {
        ch->downdate();
        if (!ch->c[0]) break;
        ch = ch->c[0];
      }
      ch->splay();
      assert(ch->c[0] == nullptr);

      ch->c[0] = rt->c[0];
      ch->c[0]->p = ch;

      rt->c[0] = nullptr;

      ch->update();
    } else if (rt->c[0]) {
      rt->c[1] = rt->c[0];
      rt->c[0] = nullptr;
    }
    assert(rt->c[0] == nullptr);
    rt->update_all();
  }

  void make_root() {
    expose();

    top_tree_node* rt = this;
    while (rt->p) {
      assert(rt->d() == 1);
      rt = rt->p;
    }
    rt->do_flip_path();
    rt->meld_path_end();

    expose();

    assert(!p);
  }

  // link v2 as a child of v1 with edge e
  friend void link(top_tree_node* e, top_tree_node* v1, top_tree_node* v2) {
    assert(e && v1 && v2);
    assert(!e->c[0] && !e->c[1] && !e->c[2]);
    v1->expose(); while (v1->p) v1 = v1->p;
    v2->make_root();

    assert(!v1->p);
    assert(!v2->p);

    e->is_path = true, e->is_vert = false;
    e->c[0] = v1;
    v1->p = e;
    e->c[1] = v2;
    v2->p = e;
    e->update();
  }

  friend std::pair<top_tree_node*, top_tree_node*> cut(top_tree_node* e) {
    assert(!e->p);
    assert(e->is_path);
    assert(!e->is_vert);

    e->downdate();

    top_tree_node* l = e->c[0];
    top_tree_node* r = e->c[1];
    assert(l && r);

    e->c[0] = e->c[1] = nullptr;
    l->p = r->p = nullptr;

    assert(e->c[2] == nullptr);

    l->meld_path_end();

    return {l, r};
  }

  friend top_tree_node* get_path(top_tree_node* a, top_tree_node* b) {
    assert(a->is_vert && b->is_vert);
    a->make_root();
    b->expose();
    if (a == b) {
      assert(!b->p);
      return b;
    }
    assert(!b->p->p);
    return b->p;
  }

  friend top_tree_node* get_subtree(top_tree_node* rt, top_tree_node* n) {
    rt->make_root();
    n->expose();
    return n;
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  int N; cin >> N;
  vector<top_tree_node> nodes(2*N-1);
  for (int i = 0; i < 2 * N - 1; i++) {
    nodes[i].__id = i;
  }

  for (int i = 0; i < N; i++) {
    top_tree_node* n = &nodes[i];
    n->is_path = n->is_vert = true;
    n->update();
  }

  for (int e = 0; e < N-1; e++) {
    int u, v; cin >> u >> v; u--, v--;
    top_tree_node* a = &nodes[u];
    top_tree_node* b = &nodes[v];
    link(&nodes[N+e], a, b);
  }

  int Q; cin >> Q;
  while (Q--) {
    int op; cin >> op;
    if (op == 1) {
      int u, v; cin >> u >> v; u--, v--;
      auto sub = get_subtree(&nodes[u], &nodes[v]);
      sub->do_subtree_increment();
      sub->downdate();
      sub->update_all();
    } else if (op == 2) {
      int u, v; cin >> u >> v; u--, v--;
      auto pth = get_path(&nodes[u], &nodes[v]);
      pth->do_path_increment();
      pth->downdate();
      pth->update_all();
    } else if (op == 3) {
      int v; cin >> v; v--;
      nodes[v].make_root();
      cout << nodes[v].tot_ans[0] << '\n';
    } else assert(false);

    /*
    cerr << "dumping tree" << '\n';
    for (int z = 0; z < 2*N-1; z++) {
      cerr << "node " << z << '\n';
      cerr << "par: " << (nodes[z].p ? nodes[z].p->__id : -1) << '\n';
      cerr << "subtree_size: " << nodes[z].subtree_size << '\n';
      cerr << "path_size: " << nodes[z].path_size << '\n';
      cerr << "subtree_lazy: " << nodes[z].subtree_lazy << '\n';
      cerr << "path_lazy: " << nodes[z].path_lazy << '\n';
      cerr << "tot_A: " << nodes[z].tot_A << '\n';
      cerr << "own_A: " << nodes[z].own_A << '\n';
      cerr << "tot_sub_d[0]: " << nodes[z].tot_sub_d[0] << '\n';
      cerr << "tot_sub_d[1]: " << nodes[z].tot_sub_d[1] << '\n';
      cerr << "tot_ans[0]: " << nodes[z].tot_ans[0] << '\n';
      cerr << "tot_ans[1]: " << nodes[z].tot_ans[1] << '\n';
    }
    cerr << '\n';
    */
  }

  return 0;
}
// https://codeforces.com/gym/102979/problem/A
// https://github.com/ecnerwala/cp-book/blob/master/src/top_tree.hpp
