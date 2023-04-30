#include<bits/stdc++.h>
using namespace std;

struct trygub_num {
  // maintains a big number in the given base b
  const int K = 30;
  const int b = 2;
  const int base = 1 << K; // for base b use b^K base to make the computation faster
  map<int, long long> digs; // tracks digits in (-base, base)
  void clear() {
    digs.clear();
  }
  // add x * b^e to this number: x, e both can be negative
  // O(log_b(n)^2 / K) amortized
  void add(long long x, int e) {
    if (e >= 0) {
      x = x * (1 << (e % K)); // x * b ^ (e % K)
      e /= K;
    }
    else {
      int k = (e % K + K) % K;
      x = x * (1 << k);
      e = e / K - (e % K != 0);
    }
    digs[e] += x;
    long long t;
    do {
      t = digs[e] / base;
      digs[e + 1] += t;
      digs[e] -= t * base;
      if (digs[e] == 0) {
        digs.erase(e);
      }
      e++;
    } while(t);

    if(digs[e] == 0) {
      digs.erase(e);
    }
  }

  // assuming this number is >= 0, find the kth digit in the base b
  int kth_digit(int k) {
    int cur_k = k / K;
    auto it = digs.lower_bound(cur_k);
    int ans = it == digs.end() || it->first > cur_k ? 0 : it->second;
    if(it != digs.begin() && prev(it)->second < 0) {
      ans--;
    }
    ans = (ans + base) % base;
    return (ans >> (k % K)) & 1; // (k % K) th digit of ans in base b
  }

  int sign() { // if the number is >= 0
    if (digs.empty()) return 1; // number = 0
    auto it = prev(digs.end());
    return it -> second > 0;
  }

} big_num;
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int q, t; cin >> q >> t >> t >> t;
  while (q--) {
    int ty; cin >> ty;
    if (ty == 1) {
      int x, e; cin >> x >> e;
      big_num.add(x, e);
    }
    else {
      int k; cin >> k;
      cout << big_num.kth_digit(k) << '\n';
    }
  }
  return 0;
}
// https://codeforces.com/blog/entry/115626
// https://loj.ac/p/2302
