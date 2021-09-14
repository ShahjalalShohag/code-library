#include<bits/stdc++.h>
using namespace std;

#define pii pair<int,int>
#define pll pair<ll,ll>
#define eb emplace_back
#define ll long long
#define nl '\n'
#define deb(x) cerr<<#x" = "<<x<<nl
#define in() ( { int a ; scanf("%d",&a); a; } )

const int N = 3e5 + 9;
int mod = 1e9 + 7;

struct DST {
  vector<vector<int>> left, right;
  int k, n;
  DST(vector<int> & a) {
    n = (int)a.size();
    k = log2(n) + 2;
    left.assign(k + 1, vector<int>(n));
    right.assign(k + 1, vector<int>(n));
    for(int j = 0; (1 << j) <= n; ++j) {
      int mask = (1 << j) - 1;
      int nw = 1; //neutral
      for(int i = 0; i < n; ++i) {
        nw = 1LL * nw * a[i] % mod;//prefix value
        left[j][i] = nw;
        if((i & mask) == mask) nw = 1; //neutral
      }
      nw = 1; //neutral
      for(int i = n - 1; i >= 0; --i) {
        nw = 1LL * nw * a[i] % mod;//prefix value
        right[j][i] = nw;
        if((i & mask) == 0) nw = 1; //neutral
      }
    }
  }
  int query(int l, int r) {
    if(l == r) return left[0][l];
    int i = 31 - __builtin_clz(l ^ r);
    int uno = left[i][r];
    int dos = right[i][l];
    return 1LL * uno * dos % mod;
  }
};

int32_t main() {
  int tc = in();
  while(tc--) {
    int n = in(), p = in(), q = in();
    mod = p;
    vector<int> a(n);
    for(int i = 0; i < n; i++) a[i] = in();
    DST t(a);
    vector<int> b((q >> 6) + 2);
    for(int i = 0; i < (int)b.size(); i++) b[i] = in();
    int x = 0, l = 1, r = 1;
    for(int i = 0; i < q; i++) {
      if(i % 64 == 0) {
        l = (b[i / 64] + x) % n;
        r = (b[(i / 64) + 1] + x) % n;
      } else {
        l = (l + x) % n;
        r = (r + x) % n;
      }
      if(l > r) swap(l, r);
      int ans = t.query(l, r);
      x = ans;
      x++;
      x %= mod;
    }
    printf("%d\n", x);
  }
  return 0;
}
// https://www.codechef.com/problems/SEGPROD
