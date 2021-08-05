/*
We are given a CNF, e.g. phi(x) = (x_1 or ~x_2) and (x_3 or ~x_4 or ~x_5) and ... .
SAT finds an assignment x for phi(x) = true.
Davis-Putnum-Logemann-Loveland Algorithm
Complexity: O(2^n) in worst case.
This implementation is practical for n <= 1000 or more. lmao.
*/

#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

// positive literal  x in [0,n),
// negative literal ~x in [-n,0)
// 0 indexed
struct SAT_GOD {
  int n;
  vector<int> occ, pos, neg;
  vector<vector<int>> g, lit;
  SAT_GOD(int n) : n(n), g(2*n), occ(2*n) { }
  void add_clause(const vector<int> &c) {
    for(auto u: c) {
      g[u+n].push_back(lit.size());
      occ[u+n] += 1;
    }
    lit.push_back(c);
  }
  //(!u | v | !w) -> (u, 0, v, 1, w, 0)
  void add(int u, int af, int v = 1e9, int bf = 0, int w = 1e9, int cf = 0) {
    vector<int> a;
    if(!af) u = ~u;
    a.push_back(u);
    if(v != 1e9) {
      if(!bf) v = ~v;
      a.push_back(v);
    }
    if(w != 1e9) {
      if(!cf) w = ~w;
      a.push_back(w);
    }
    add_clause(a);
  }
  vector<bool> x;
  vector<vector<int>> decision_stack;
  vector<int> unit_stack, pure_stack;
  void push(int u) {
    x[u + n] = 1;
    decision_stack.back().push_back(u);
    for (auto i: g[u + n]) if (pos[i]++ == 0) {
        for (auto u: lit[i]) --occ[u+n];
      }
    for (auto i: g[~u + n]) {
      ++neg[i];
      if (pos[i] == 0) unit_stack.push_back(i);
    }
  }
  void pop() {
    int u = decision_stack.back().back();
    decision_stack.back().pop_back();
    x[u + n] = 0;
    for (auto i: g[u + n]) if (--pos[i] == 0) {
        for (auto u: lit[i]) ++occ[u + n];
      }
    for (auto i: g[~u+n]) --neg[i];
  }
  bool reduction() {
    while(!unit_stack.empty() || !pure_stack.empty()) {
      if(!pure_stack.empty()) {  // pure literal elimination
        int u = pure_stack.back();
        pure_stack.pop_back();
        if (occ[u + n] == 1 && occ[~u + n] == 0) push(u);
      } else {                   // unit propagation
        int i = unit_stack.back();
        unit_stack.pop_back();
        if(pos[i] > 0) continue;
        if(neg[i]     == lit[i].size()) return false;
        if(neg[i] + 1 == lit[i].size()) {
          int w = n;
          for (int u: lit[i]) if (!x[u + n] && !x[~u + n]) w = u;
          if (x[~w + n]) return false;
          push(w);
        }
      }
    }
    return true;
  }
  bool ok() {
    x.assign(2*n,0);
    pos = neg = vector<int>(lit.size());
    decision_stack.assign(1, {});
    while(1) {
      if(reduction()) {
        int s = 0;
        for(int u = 0; u < n; ++u) if(occ[s + n] + occ[~s + n] < occ[u + n] + occ[~u + n]) s = u;
        if(occ[s + n] + occ[~s + n] == 0) return true;
        decision_stack.push_back({});
        push(s);
      } else {
        int s = decision_stack.back()[0];
        while(!decision_stack.back().empty()) pop();
        decision_stack.pop_back();
        if (decision_stack.empty()) return false;
        push(~s);
      }
    }
  }
};

int32_t main() {
  int n = 9;
  SAT_GOD t(n);
  t.add(0, 0, 1, 1);
  t.add(1, 0);
  t.add(1, 0, 3, 1, 5, 1);
  cout << t.ok() << endl;
}
