#include<bits/stdc++.h>
using namespace std;

using T = __int128;
T extended_euclid(T a, T b, T &x, T &y) {
  if (b == 0) {
    x = 1; y = 0;
    return a;
  }
  T x1, y1;
  T d = extended_euclid(b, a % b, x1, y1);
  x = y1;
  y = x1 - y1 * (a / b);
  return d;
}
T inverse(T a, T m) {
  T x, y;
  T g = extended_euclid(a, m, x, y);
  if (g != 1) return -1;
  return (x % m + m) % m;
}
T sumsq(T n) {
  return n / 2 * ((n - 1) | 1);
}
// \sum_{i = 0}^{n - 1}{(a + d * i) / m}, O(log m)
T floor_sum(T a, T d, T m, T n) {
  T res = d / m * sumsq(n) + a / m * n;
  d %= m; a %= m;
  if (!d) return res;
  T to = (n * d + a) / m;
  return res + (n - 1) * to - floor_sum(m - 1 - a, m, d, to);
}
// number of solutions to ax + by + cz = n s.t. x, y, z >= 0
// a, b, c >= 1, a, b and c are pairwise coprime
// O(log(max(a, b, c)));
T count(T a, T b, T c, T n) {
  assert(__gcd(a, b) == 1);
  assert(__gcd(b, c) == 1);
  assert(__gcd(a, c) == 1);
  T b1p = (-n % a * inverse(b, a) % a + a) % a;
  if (b1p == 0) b1p = a;  
  T c1p = (b % a * inverse(c, a) % a + a) % a;
  if (c1p == 0) c1p = a;  
  T c2p = (-n % b * inverse(c, b) % b + b) % b;
  if (c2p == 0) c2p = b;  
  T a2p = (c % b * inverse(a, b) % b + b) % b;
  if (a2p == 0) a2p = b;  
  T a3p = (-n % c * inverse(a, c) % c + c) % c;
  if (a3p == 0) a3p = c;  
  T b3p = (a % c * inverse(b, c) % c + c) % c;
  if (b3p == 0) b3p = c;
  T N1 = n * (n + a + b + c);
  N1 += c * b * b1p * (a + 1 - c1p * (b1p - 1));
  N1 += a * c * c2p * (b + 1 - a2p * (c2p - 1));
  N1 += b * a * a3p * (c + 1 - b3p * (a3p - 1));
  T ans = N1 / (2 * a * b * c);
  ans += floor_sum(0, c1p, a, b1p);
  ans += floor_sum(0, a2p, b, c2p);
  ans += floor_sum(0, b3p, c, a3p);
  ans -= 2;
  return ans;
}
// number of solutions to ax + by + cz = n s.t. x, y, z >= 0
// a, b, c >= 1
// O(log(max(a, b, c)));
// paper: https://cs.uwaterloo.ca/journals/JIS/VOL23/Binner/binner4.pdf
T yo(T a, T b, T c, T n) {
  T g = __gcd(__gcd(a, b), c);
  if (n % g != 0) return 0;
  a /= g; b /= g; c /= g; n /= g;
  T g1 = __gcd(b, c), g2 = __gcd(c, a), g3 = __gcd(a, b);
  T a1 = inverse(a, g1), b2 = inverse(b, g2), c3 = inverse(c, g3);
  T n1 = n * a1 % g1, n2 = n * b2 % g2, n3 = n * c3 % g3;
  T A = a / g2 / g3, B = b / g3 / g1, C = c / g1 / g2;
  T N = (n - a * n1 - b * n2 - c * n3) / g1 / g2 / g3;
  return count(A, B, C, N);
}
T brute(T a, T b, T c, T n) {
  T ans = 0;
  for (int i = 0; i <= n; i++) {
    for (int j = 0; j <= n; j++) {
      for (int k = 0; k <= n; k++) {
        if (i * a + j * b + k * c == n) ++ans;
      }
    }
  }
  return ans;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  for (int a = 1; a <= 5; a++) {
    for (int b = 1; b <= 5; b++) {
      for (int c = 1; c <= 5; c++) {
        for (int n = 1; n <= 50; n++) {
          T x = yo(a, b, c, n), y = brute(a, b, c, n);
          assert(x == y);
        }
      }
    }
  }
  return 0;
}
