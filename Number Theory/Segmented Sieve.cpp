#include <bits/stdc++.h>
using namespace std;

vector<int> generatePrimesUpTo(int limit) {
    vector<bool> isPrime(limit + 1, true);
    isPrime[0] = isPrime[1] = false;
    for (int p = 2; p * p <= limit; ++p) {
        if (isPrime[p]) {
            for (int i = p * p; i <= limit; i += p) {
                isPrime[i] = false;
            }
        }
    }
    vector<int> primes;
    for (int p = 2; p <= limit; ++p) {
        if (isPrime[p]) {
            primes.push_back(p);
        }
    }
    return primes;
}
// Generate all primes from m to n
void segmentedSieve(int m, int n) {
    if (m == 1) {
        m++;
    }
    int limit = sqrt(n);
    vector<int> primes = generatePrimesUpTo(limit);
    vector<bool> isPrime(n - m + 1, true);
    for (int p : primes) {
        int start = max(p * p, m + ((p - m % p) % p));
        for (int j = start; j <= n; j += p) {
            isPrime[j - m] = false;
        }
    }
    for (int i = m; i <= n; ++i) {
        if (isPrime[i - m]) {
            cout << i << endl;
        }
    }
}

int main() {
    int t;
    cin >> t;
    while (t--) {
        int m, n;
        cin >> m >> n;
        segmentedSieve(m, n);
        if (t > 0) cout << endl;
    }
    return 0;
}

// https://www.spoj.com/problems/PRIME1/
