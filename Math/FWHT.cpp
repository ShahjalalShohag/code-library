#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, mod = 1e9 + 7;

int POW(long long n, long long k) {
  int ans = 1 % mod; n %= mod; if (n < 0) n += mod;
  while (k) {
    if (k & 1) ans = (long long) ans * n % mod;
    n = (long long) n * n % mod;
    k >>= 1;
  }
  return ans;
}
const int inv2 = (mod + 1) >> 1;
#define M (1 << 20)
#define OR 0
#define AND 1
#define XOR 2
struct FWHT{
  int P1[M], P2[M];
  void wt(int *a, int n, int flag = XOR) {
    if (n == 0) return;
    int m = n / 2;
    wt(a, m, flag); wt(a + m, m, flag);
    for (int i = 0; i < m; i++){
      int x = a[i], y = a[i + m];
      if (flag == OR) a[i] = x, a[i + m] = (x + y) % mod;
      if (flag == AND) a[i] = (x + y) % mod, a[i + m] = y;
      if (flag == XOR) a[i] = (x + y) % mod, a[i + m] = (x - y + mod) % mod;
    }
  }
  void iwt(int* a, int n, int flag = XOR) {
    if (n == 0) return;
    int m = n / 2;
    iwt(a, m, flag); iwt(a + m, m, flag);
    for (int i = 0; i < m; i++){
      int x = a[i], y = a[i + m];
      if (flag == OR) a[i] = x, a[i + m] = (y - x + mod) % mod;
      if (flag == AND) a[i] = (x - y + mod) % mod, a[i + m] = y;
      if (flag == XOR) a[i] = 1LL * (x + y) * inv2 % mod, a[i + m] = 1LL * (x - y + mod) * inv2 % mod; // replace inv2 by >>1 if not required
    }
  }
 vector<int> multiply(int n, vector<int> A, vector<int> B, int flag = XOR) {
    assert(__builtin_popcount(n) == 1);
    A.resize(n); B.resize(n);
    for (int i = 0; i < n; i++) P1[i] = A[i];
    for (int i = 0; i < n; i++) P2[i] = B[i];
    wt(P1, n, flag); wt(P2, n, flag);
    for (int i = 0; i < n; i++) P1[i] = 1LL * P1[i] * P2[i] % mod;
    iwt(P1, n, flag);
    return vector<int> (P1, P1 + n);
  }
  vector<int> pow(int n, vector<int> A, long long k, int flag = XOR) {
    assert(__builtin_popcount(n) == 1);
    A.resize(n);
    for (int i = 0; i < n; i++) P1[i] = A[i];
    wt(P1, n, flag);
    for(int i = 0; i < n; i++) P1[i] = POW(P1[i], k);
    iwt(P1, n, flag);
    return vector<int> (P1, P1 + n);
  }
}t;
int32_t main() {
  int n; cin >> n;
  vector<int> a(M, 0);
  for(int i = 0; i < n; i++) {
    int k; cin >> k; a[k]++;
  }
  vector<int> v = t.pow(M, a, n, AND);
  int ans = 1;
  for(int i = 1; i < M; i++) ans += v[i] > 0;
  cout << ans << '\n';;
  return 0;
}
// https://csacademy.com/contest/archive/task/and-closure/statement/
