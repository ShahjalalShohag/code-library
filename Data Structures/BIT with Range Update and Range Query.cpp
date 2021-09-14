#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

struct BIT {
  long long M[N], A[N];
  BIT() {
    memset(M, 0, sizeof M);
    memset(A, 0, sizeof A);
  }
  void update(int i, long long mul, long long add) {
    while (i < N) {
      M[i] += mul;
      A[i] += add;
      i |= (i + 1);
    }
  }
  void upd(int l, int r, long long x) {
    update(l, x, -x * (l - 1));
    update(r, -x, x * r);
  }
  long long query(int i) {
    long long mul = 0, add = 0;
    int st = i;
    while (i >= 0) {
      mul += M[i];
      add += A[i];
      i = (i & (i + 1)) - 1;
    }
    return (mul * st + add);
  }
  long long query(int l, int r) {
    return query(r) - query(l - 1);
  }
} t;

int32_t main() {

  return 0;
}
