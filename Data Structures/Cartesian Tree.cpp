#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9, mod = 1e9 + 7;
using ll = long long;
/**
A Cartesian tree is a tree data structure created from a set of data that obeys
the  following structural invariants:
    1. The tree obeys in the min (or max) heap property – each node is less (or greater)
    than its children.
    2. An inorder traversal of the nodes yields the values in the same order
    in which they appear in the initial sequence.
in this tree lca(i,j)=rmq(i,i+1,....j-1,j)
l[i]=left child of i, i.e. nearest value <i such that a[l[i]]<a[i] (for min heap)
r[i]=right child of i, i.e. nearest value >i such that a[r[i]]<a[i] (for min heap)

**/

int n, tot, st[N], l[N], r[N], vis[N];
ll inv[N], ans;
pair<int, int> a[N];

int dfs(int u) {
  int sz = 1;
  //cout<<u<<": "<<l[u]<<' '<<r[u]<<nl;
  if (l[u]) sz += dfs(l[u]);
  if (r[u]) sz += dfs(r[u]);
  //debug(sz);
  ans = ans * inv[sz] % mod;
  return sz;
}

//O(n)
//returns root of the tree
//has max heap property
int cartesian_tree() {
  tot = 0;
  for (int i = 1; i <= n; i++) l[i] = r[i] = vis[i] = 0;
  for (int i = 1; i <= n; i++) {
    int k = tot;
    while (k > 0 && a[st[k - 1]] < a[i]) k--; //use > for min heap
    if (k) r[st[k - 1]] = i;
    if (k < tot) l[i] = st[k];
    st[k++] = i;
    tot = k;
  }
  for (int i = 1; i <= n; i++) vis[l[i]] = vis[r[i]] = 1;
  int rt = 0;
  for (int i = 1; i <= n; i++) {
    if (vis[i] == 0) rt = i;
  }
  return rt;
}


//given an array find the probability of any array having each element in [0,1](real numbers)
//and isomorphic to the array
//two array is isomorphic if index set(before sorting) of first array is equal to index set(before sorting) of
//second array after sorting the numbers
//(1,2,1) and (2,3,2) is isomorphic, (1,2,1) and (3,2,3) is not
int main() {
  inv[1] = 1;
  for (int i = 2; i < N; i++) inv[i] = inv[mod % i] * (mod - mod / i) % mod;
  int t;
  scanf("%d", &t);
  while (t--) {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
      int x;
      scanf("%d", &x);
      a[i] = make_pair(x, -i);//for making the array elements distinct,for min heap use (+i)
    }
    ans = 1LL * n * inv[2] % mod;//n*expected value in [0,1]
    int rt = cartesian_tree();
    dfs(rt);
    printf("%I64d\n", ans);
  }
  return 0;
}
