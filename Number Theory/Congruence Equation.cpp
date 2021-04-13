#include<bits/stdc++.h>
using namespace std;

// return x, where ax = 1 (mod mod)
long long mod_inv(long long a, long long mod) {
  if (gcd(a, mod) != 1) return -1;
  long long b = mod, s = 1, t = 0;
  while (b) {
    long long q = a / b;
    swap(a -= q * b, b);
    swap(s -= q * t, t);
  }
  return s < 0 ? s + mod : s;
}

// ax = b (mod m)
vector<long long> congruence_equation(long long a, long long b, long long m) {
  vector<long long> ret;
  long long g = gcd(a, m), x;
  if (b % g != 0) return ret;
  a /= g, b /= g;
  x = mod_inv(a, m / g);
  for (int k = 0; k < g; ++k) {
    ret.push_back((x * b + m / g * k) % m);
  }
  return ret;
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  
  return 0;
}