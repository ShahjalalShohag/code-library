#include <bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;
int a[N];

// Segment Tree with point update and range query
// Replace 'T' with appropriate type (int, long long, etc.)
// 1 based indexing
template <typename T>
struct ST {
  T t[4 * N];
  static const T inf = numeric_limits<T>::max(); // Maximum limit for type T
  
  ST() {}

  T merge(T a, T b) {
    return max(a, b); // Change this as per your use case
  }

  // Build function for segment tree.
  // n: node index, [b, e]: range covered by the node.
  void build(int n, int b, int e) {
    if (b == e) {
      t[n] = a[b];
      return;
    }
    int mid = (b + e) >> 1, l = n << 1, r = l | 1;
    build(l, b, mid);
    build(r, mid + 1, e);
    t[n] = merge(t[l], t[r]);
  }

  // Update function to update a point value
  // n: node index, [b, e]: range covered by node, i: index to update, x: new value
  void upd(int n, int b, int e, int i, T x) {
    if (b > i || e < i) return;
    if (b == e && b == i) {
      t[n] = x;
      return;
    }
    int mid = (b + e) >> 1, l = n << 1, r = l | 1;
    upd(l, b, mid, i, x);
    upd(r, mid + 1, e, i, x);
    t[n] = merge(t[l], t[r]);
  }

  // Query function to query in a range [i, j]
  // n: node index, [b, e]: range covered by node, [i, j]: query range
  T query(int n, int b, int e, int i, int j) {
    if (b > j || e < i) return -inf;
    if (b >= i && e <= j) return t[n];
    int mid = (b + e) >> 1, l = n << 1, r = l | 1;
    return merge(query(l, b, mid, i, j), query(r, mid + 1, e, i, j));
  }
};

int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  // Example usage
  int n = 5;
  for (int i = 1; i <= n; i++) {
    a[i] = i;
  }

  ST<int> t;
  t.build(1, 1, n);
  cout << t.query(1, 1, n, 1, 3) << '\n';
  t.upd(1, 1, n, 2, 10);
  cout << t.query(1, 1, n, 1, 3) << '\n';

  return 0;
}
