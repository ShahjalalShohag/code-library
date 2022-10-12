#include<bits/stdc++.h>
using namespace std;

const int N = 105, MAX_INDEPENDENT_SET_SIZE = 105;
using T = pair<int, int>;

struct GroundSetElement {
  int color;
  T edge;
  bool in_independent_set; // if this element is in the IS
  int independent_set_position; // the index of this element in the IS
};

vector<GroundSetElement> elements;
vector<int> independent_set; // stores the indices of the ground set elements

struct GraphicOracle {
  struct GraphBasis {
    vector<int> par, rnk, sz;
    int c;
    GraphBasis(){}
    GraphBasis(int n) : par(n + 1), rnk(n + 1, 0), sz(n + 1, 1), c(n) {
      for (int i = 1; i <= n; ++i) par[i] = i;
    }
    int find(int i) {
      return (par[i] == i ? i : (par[i] = find(par[i])));
    }
    bool same(int i, int j) {
      return find(i) == find(j);
    }
    int get_size(int i) {
      return sz[find(i)];
    }
    int count() {
      return c;    //connected components
    }
    int add_edge(T edge) { // add an independent edge
      auto [i, j] = edge;
      if ((i = find(i)) == (j = find(j))) return -1;
      else --c;
      if (rnk[i] > rnk[j]) swap(i, j);
      par[i] = j;
      sz[j] += sz[i];
      if (rnk[i] == rnk[j]) rnk[j]++;
      return j;
    }
    bool independent_with(T edge) {
      return !same(edge.first, edge.second);
    }
  };
  int n;
  GraphBasis basis; // of independent set
  GraphBasis basis_without[MAX_INDEPENDENT_SET_SIZE + 1];

  GraphicOracle(){}
  GraphicOracle(int n) : n(n) {}

  // can we insert elements[id] without breaking independence?
  bool can_insert(int id) {
    return basis.independent_with(elements[id].edge);
  }

  // can we insert elements[inserted_id] and remove elements[removed_id] 
  // without breaking independence?
  bool can_exchange(int inserted_id, int removed_id) {
    int pos = elements[removed_id].independent_set_position;
    return basis_without[pos].independent_with(elements[inserted_id].edge);
  }

  // prepare the oracle for the current independent set
  void prepare() {
    basis = GraphBasis(n);

    for (int i = 0; i < independent_set.size(); i++) {
      basis_without[i] = GraphBasis(n);
    }

    for (int i = 0; i < independent_set.size(); i++) {
      basis.add_edge(elements[independent_set[i]].edge);
      for (int  j = 0; j < independent_set.size(); j++) {
        if (i != j) basis_without[i].add_edge(elements[independent_set[j]].edge);
      }
    }
  }
}graphic_oracle;


struct ColorfulOracle {
  int color_count;
  vector<bool> color_used;

  ColorfulOracle(int _color_count = 0) {
    color_count = _color_count;
    color_used = vector<bool>(color_count + 1, false);
  }

  // can we insert elements[id] without breaking independence?
  bool can_insert(int id) {
    int inserted_color = elements[id].color;
    return !color_used[inserted_color];
  }

  // can we insert elements[inserted_id] and remove elements[removed_id] 
  // without breaking independence?
  bool can_exchange(int inserted_id, int removed_id) {
    int inserted_color = elements[inserted_id].color;
    int removed_color = elements[removed_id].color;
    if (!color_used[inserted_color]) return true;
    return inserted_color == removed_color;
  }

  // prepare the oracle for the current independent set
  void prepare() {
    for (int c = 0; c < color_count; c++) {
      color_used[c] = false;
    }
    for (auto idx : independent_set) {
      color_used[elements[idx].color] = true;
    }
  }
}colorful_oracle;

// try to increment the size of the independent set
// implementation details: https://codeforces.com/blog/entry/69287#:~:text=Implementation%20and%20complexity
bool augment() {
  // swapping the oracles might run faster
  auto oracle1 = colorful_oracle;
  auto oracle2 = graphic_oracle;
  oracle1.prepare();
  oracle2.prepare();

  const int SOURCE = -1;
  const int NOT_VISITED = -2;
  const int NOT_FOUND = -3;

  int sz = elements.size();
  vector<int> par(sz, NOT_VISITED);
  int endpoint = NOT_FOUND;
  queue<int> q;
  for (int i = 0; i < sz; i++) {
    if (oracle1.can_insert(i)) {
      par[i] = SOURCE;
      q.push(i);
    }
  }
  while (q.size()) {
    int cur = q.front();
    q.pop();
    if (elements[cur].in_independent_set) {
      for (int to = 0; to < sz; to++) {
        if (par[to] != NOT_VISITED) continue;
        if (!oracle1.can_exchange(to, cur)) continue;
        par[to] = cur;
        q.push(to);
      }
    } else {
      if (oracle2.can_insert(cur)) {
        endpoint = cur;
        break;
      }
      for (auto to : independent_set) {
        if (par[to] != NOT_VISITED) continue;
        if (!oracle2.can_exchange(cur, to)) continue;
        par[to] = cur;
        q.push(to);
      }
    }
  }
  if (endpoint == NOT_FOUND) return false;
  do {
    elements[endpoint].in_independent_set ^= true;
    endpoint = par[endpoint];
  } while (endpoint != SOURCE);
  independent_set.clear();
  for (int i = 0; i < sz; i++) {
    if (elements[i].in_independent_set) {
      elements[i].independent_set_position = independent_set.size();
      independent_set.push_back(i);
    }
  }
  return true;
}

// returns the largest subset of elements that is independent for both matroids
// formed by the elements (colorful matroid and graphic matroid in this case)
// Time Complexity:
// let r1 = rank (max size of IS) of first matroid, r2 = rank of second matroid
// n = size of the ground set elements, r = min(r1, r2)
// c1_exch_insert = running of can_exchange() and can_insert() functions of 1st oracle
// c1_prep = running time of prepare() of 1st oracle
// Total Complexity: O(r^1.5 * n * (c1_exch_insert + c2_exch_insert) + r * (c1_prep + c2_prep))
vector<int> matriod_intersection(int node_count, vector<GroundSetElement> _elements) {
  elements = _elements;
  independent_set.clear();
  int color_count = 0;
  for (auto &element: elements) {
    element.in_independent_set = false;
    color_count = max(color_count, element.color);
  }
  graphic_oracle = GraphicOracle(node_count);
  colorful_oracle = ColorfulOracle(color_count);
  while (augment()); // keep increasing independent set if possible
  return independent_set;
}


int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m, k, cs = 0; 
  while (cin >> n >> m >> k) {
    vector<GroundSetElement> elements;
    for (int i = 1; i <= m; i++) {
      int u, v, c; cin >> u >> v >> c;
      elements.emplace_back();
      elements.back().color = c;
      elements.back().edge = T(u, v);
    }

    auto independent_set = matriod_intersection(n, elements);
    // for (auto i: independent_set) {
    //   cout << elements[i].edge.first << ' ' << elements[i].edge.second << ' ' << elements[i].color << '\n';
    // }
    cout << "Instancia " << ++cs << "\n";
    if (independent_set.size() < n - 1) {
      cout << "nao\n";
    }
    else {
      assert(independent_set.size() == n - 1);
      cout << "sim\n";
    }
    cout << '\n';
  }
  return 0;
}
// https://codeforces.com/blog/entry/69287
// https://www.beecrowd.com.br/judge/en/problems/view/2128
