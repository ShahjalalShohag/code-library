#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

/*
prufer code is a sequence of length n-2 to uniquely determine a labeled tree with n vertices
Each time take the leaf with the lowest number and add the node number the leaf is connected to
the sequence and remove the leaf. Then break the algo after n-2 iterations
*/
//0-indexed
int n;
vector<int> g[N];
int parent[N], degree[N];

void dfs (int v) {
  for (size_t i = 0; i < g[v].size(); ++i) {
    int to = g[v][i];
    if (to != parent[v]) {
      parent[to] = v;
      dfs (to);
    }
  }
}

vector<int> prufer_code() {
  parent[n - 1] = -1;
  dfs (n - 1);
  int ptr = -1;
  for (int i = 0; i < n; ++i) {
    degree[i] = (int) g[i].size();
    if (degree[i] == 1 && ptr == -1) ptr = i;
  }
  vector<int> result;
  int leaf = ptr;
  for (int iter = 0; iter < n - 2; ++iter) {
    int next = parent[leaf];
    result.push_back (next);
    --degree[next];
    if (degree[next] == 1 && next < ptr) leaf = next;
    else {
      ++ptr;
      while (ptr < n && degree[ptr] != 1) ++ptr;
      leaf = ptr;
    }
  }
  return result;
}
vector < pair<int, int> > prufer_to_tree(const vector<int> & prufer_code) {
  int n = (int) prufer_code.size() + 2;
  vector<int> degree (n, 1);
  for (int i = 0; i < n - 2; ++i) ++degree[prufer_code[i]];

  int ptr = 0;
  while (ptr < n && degree[ptr] != 1) ++ptr;
  int leaf = ptr;
  vector < pair<int, int> > result;
  for (int i = 0; i < n - 2; ++i) {
    int v = prufer_code[i];
    result.push_back (make_pair (leaf, v));
    --degree[leaf];
    if (--degree[v] == 1 && v < ptr) leaf = v;
    else {
      ++ptr;
      while (ptr < n && degree[ptr] != 1) ++ptr;
      leaf = ptr;
    }
  }
  for (int v = 0; v < n - 1; ++v) if (degree[v] == 1) result.push_back (make_pair (v, n - 1));
  return result;
}

int32_t main() {

  return 0;
}

