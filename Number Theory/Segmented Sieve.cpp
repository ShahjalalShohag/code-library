#include <bits/stdc++.h>
using namespace std;

const int S = 10000;

// Count primes up to n
int count_primes(int n) {
    vector<int> primes;
    int nsqrt = sqrt(n);
    vector<char> is_prime(nsqrt + 2, true);
    for (int i = 2; i <= nsqrt; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
            for (int j = i * i; j <= nsqrt; j += i)
                is_prime[j] = false;
        }
    }

    int result = 0;
    vector<char> block(S);
    for (int k = 0; k * S <= n; k++) {
        fill(block.begin(), block.end(), true);
        int start = k * S;
        for (int p : primes) {
            int start_idx = (start + p - 1) / p;
            int j = max(start_idx, p) * p - start;
            for (; j < S; j += p)
                block[j] = false;
        }
        if (k == 0)
            block[0] = block[1] = false;
        for (int i = 0; i < S && start + i <= n; i++) {
            if (block[i])
                result++;
        }
    }
    return result;
}

// Primes in range
vector<char> segmentedSieve(long long L, long long R) {
    long long lim = sqrt(R);
    vector<char> mark(lim + 1, false);
    vector<long long> primes;
    for (long long i = 2; i <= lim; ++i) {
        if (!mark[i]) {
            primes.emplace_back(i);
            for (long long j = i * i; j <= lim; j += i)
                mark[j] = true;
        }
    }

    vector<char> isPrime(R - L + 1, true);
    for (long long i : primes)
        for (long long j = max(i * i, (L + i - 1) / i * i); j <= R; j += i)
            isPrime[j - L] = false;
    if (L == 1)
        isPrime[0] = false;
    return isPrime;
}

int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n = 1000000; // Example value, modify as needed
    cout << "Count of primes up to " << n << ": " << count_primes(n) << '\n';

    long long L = 10, R = 100; // Example range
    vector<char> primesInRange = segmentedSieve(L, R);
    cout << "Primes between " << L << " and " << R << ": ";
    for (long long i = L; i <= R; i++) {
        if (primesInRange[i - L])
            cout << i << " ";
    }
    cout << '\n';

    return 0;
}
