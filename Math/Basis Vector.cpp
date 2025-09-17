#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;
using ll = long long;

template<typename T = int, int B = 31>
struct Basis {
  T basis[B]; int sz;
  void clear() {
    memset(basis, 0, sizeof basis);
    sz = 0;
  }
  Basis() {
    clear();
  }
  void insert(T x) {
    for (int i = B - 1; i >= 0; i--) {
      if (x >> i & 1) {
        if (basis[i]) x ^= basis[i];
        else {
          basis[i] = x;
          sz++;
          break;
        }
      }
    }
  }
  // is there any subset having xor = x
  bool can(T x) {
    for(int i = B - 1; i >= 0; i--) {
      x = min(x, x ^ basis[i]);
    }
    return x == 0;
  }
  // max subset xor after xoring it with x
  T max_xor(T x = 0) { 
    for(int i = B - 1; i >= 0; i--) {
      x = max(x, x ^ basis[i]);
    }
    return x;
  }
  // kth smallest subset xor, 1st is 0
  T kth(T k) {
    if (k < 1 || k > ((T)1 << sz)) {
      return -1;
    }
    T x = 0;
    T cnt = ((T)1 << sz);
    for (int i = B - 1; i >= 0; i--) {
      if (basis[i]) {
        if (k > (cnt >> 1)) {
          // set the ith bit
          if (!(x >> i & 1)) {
            x ^= basis[i];
          }
          k -= (cnt >> 1);
        } else {
          // unset the ith bit
          if (x >> i & 1) {
            x ^= basis[i];
          }
        }
        cnt >>= 1;
      }
    }
    return x;
  }
  // number of subsets having xor < x
  T count_lt(T x) {
    if (x < 0) {
      return 0;
    }
    T ans = 0;
    T cnt = ((T)1 << sz);
    T mask = 0;
    for (int i = B - 1; i >= 0; i--) {
      // at this stage, all prev > i th bits in mask and x are the same
      if (basis[i]) {
        if (x >> i & 1) {
          ans += (cnt >> 1);
          if (!(mask >> i & 1)) {
            mask ^= basis[i];
          }
        } else {
          if (mask >> i & 1) {
            mask ^= basis[i];
          }
        }
        cnt >>= 1;
      } else {
        if ((x >> i & 1) != (mask >> i & 1)) {
          if (x >> i & 1) {
            return ans + cnt;
          } else {
            return ans; 
          }
        }
      }
    }
    return ans;
  }
};

void solve() {
  int q; cin >> q;
  Basis B;
  while (q--) {
    int ty, k; cin >> ty >> k;
    if (ty == 1) B.insert(k);
    else cout << B.kth(k) << '\n';
  }
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t = 1;
  // cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}
// https://codeforces.com/group/qcIqFPYhVr/contest/203881/problem/S
