#include<bits/stdc++.h>
using namespace std;

const int N = 2e5 + 9, M = N * 2 + N * 19 * 2;

// Define a graph using an adjacency list
vector<pair<int, int>> g[N * 2], G[N];

// Function to add an edge to the graph
inline void add_edge(int u, int v, int w) {
  g[u].push_back({v, w});
}

int T;

// Function to binarize the tree
void binarize(int u, int p = 0) {
  int last = 0, tmp = 0;
  for (auto e : G[u]) {
    int v = e.first, w = e.second;
    if (v == p) continue;
    ++tmp;
    if (tmp == 1) {
      add_edge(u, v, w);
      add_edge(v, u, w);
      last = u;
    }
    else if (tmp == ((int) G[u].size()) - (u != 1)) {
      add_edge(last, v, w);
      add_edge(v, last, w);
    }
    else {
      T++;
      add_edge(last, T, 0);
      add_edge(T, last, 0);
      last = T;
      add_edge(T, v, w);
      add_edge(v, T, w);
    }
  }
  for (auto e : G[u]) {
    int v = e.first;
    if (v == p) continue;
    binarize(v, u);
  }
}

int sz[N * 2];
int tot, done[N * 2], cenpar[N * 2];

// Calculate the subtree sizes
void calc_sz(int u, int p) {
  tot ++; // Increment the total count of nodes in the subtree
  sz[u] = 1; // Initialize the size of the subtree rooted at u to 1
  for (auto e: g[u]) {
    int v = e.first;
    if(v == p || done[v]) continue; // Skip nodes that have been visited or are the parent
    calc_sz(v, u); // Recursively calculate subtree sizes
    sz[u] += sz[v]; // Update the size of the current node's subtree
  }
}

// Find the centroid of the subtree
int find_cen(int u, int p) {
  for (auto e: g[u]) {
    int v = e.first;
    if(v == p || done[v]) continue; // Skip nodes that have been visited or are the parent
    else if(sz[v] > tot / 2) return find_cen(v, u); // Recursively search for the centroid
  }
  return u; // Return the centroid when found
}

long long d[20][N * 2];

// Calculate distances for centroid decomposition
void yo(int u, int p, long long nw, int l) {
  d[l][u] = nw; // Store the distance in the d array
  for(auto e : g[u]) {
    int v = e.first; long long w = e.second;
    if (v == p || done[v]) continue; // Skip visited nodes and the parent
    yo(v, u, nw + w, l); // Recursively calculate distances
  }
}

int st[N * 2], en[N * 2], DT;

struct node {
  vector<int> ct; // Stores adjacent edges in centroid tree
  int level = 0, id = 0, cnt = 0;
  long long sum = 0, parsum = 0;
}t[M];

// Decompose the tree into a centroid tree
int decompose(int u, int p = 0, int l = 0) {
  tot = 0;
  calc_sz(u, p);
  int cen = find_cen(u, p); // Find the centroid of the current subtree
  cenpar[cen] = p; done[cen] = 1; // Mark the centroid as visited

  // Add your centroid tree construction logic here
  // The logic to build the centroid tree should go in this function.
  // This part is currently missing from the code.
}

