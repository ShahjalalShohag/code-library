#include<bits/stdc++.h>
using namespace std;

int g (int n) {
    return n ^ (n >> 1);
}
int rev_g (int g) {
  int n = 0;
  for (; g; g >>= 1)
    n ^= g;
  return n;
}

// property: g(i) = g(i - 1) ^ (1 << lsb(i))
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  
  return 0;
}
