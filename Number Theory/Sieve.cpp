#include<bits/stdc++.h>
using namespace std;

const int N = 1e6 + 9;

vector<int> primes;
bool is_prime[N]; 
// use bitset<N> is_prime; to have O(N/64) memory complexity
// using bitset you can solve upto around N = 10^8 in 1s
void sieve_v0() {
  for (int i = 2; i < N; i++) {
    is_prime[i] = true;
  }
  for (int i = 2; i * i < N; i++) {
    if (is_prime[i]) {
      for (int j = i * i; j < N; j += i) {
        is_prime[j] = false;
      }
    }
  }
  for (int i = 2; i < N; i++) {
    if (is_prime[i]) {
      primes.push_back(i);
    }
  }
}

// sieve with smallest prime factors (spf)
int spf[N];
void sieve() {
  for (int i = 2; i < N; i++) {
    spf[i] = i;
  }
  for (int i = 2; i * i < N; i++) {
    if (spf[i] == i) {
      for (int j = i * i; j < N; j += i) {
        spf[j] = min(spf[j], i);
      }
    }
  }
  for (int i = 2; i < N; i++) {
    if (spf[i] == i) {
      primes.push_back(i);
    }
  }
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  sieve_v0();
  cout << primes.size() << '\n';
  primes.clear();
  sieve();
  cout << primes.size() << '\n';
  return 0;
}
