#include<bits/stdc++.h>
using namespace std;

const int N = 1e7 + 9;

// f is multiplicative with f(p^k) = k
int spf[N], f[N], cnt[N]; // cnt[i] = power of spf[i] in i
vector<int> primes;
void sieve() {
  f[1] = 1;
  for(int i = 2; i < N; i++) {
    if (spf[i] == 0) { // i is prime
      spf[i] = i, primes.push_back(i);
      f[i] = 1; cnt[i] = 1;
    }
    int sz = primes.size();
    for (int j = 0; j < sz && i * primes[j] < N && primes[j] <= spf[i]; j++) {
      int p = i * primes[j];
      spf[p] = primes[j];
      if (primes[j] == spf[i]) { // primes[j] divides i
        f[p] = f[i] / cnt[i] * (cnt[i] + 1); // f(i * primes[j]) = f(i / (primes[j]^cnt[i])) * f(primes[j]^(cnt[i] + 1))
        cnt[p] = cnt[i] + 1;
      }
      else { // primes[j] does not divide i
        f[p] = f[i] * f[primes[j]];
        cnt[p] = 1;
      }
    }
  }
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  sieve();
  for (int i = 1; i <= 10; i++) {
    cout << f[i] << '\n';
  }
  return 0;
}
// https://codeforces.com/blog/entry/54090