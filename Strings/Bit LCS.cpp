#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,avx2,fma")
#pragma GCC optimize("unroll-loops")

#include <bits/stdc++.h>
using namespace std;
/* Bit-String Longest Common Subsequence Algorithm
O(nm/w)*/
const int N = 5e4 + 9, SIGMA = 26;
const int W = 62;
int M;
struct Bitset {
    long long u[N / W + 5];
    void clear() {
        memset(u, 0, sizeof(u));
    }
    void set(int x) {
        u[x / W] |= 1ll << (x % W);
    }
    Bitset operator | (const Bitset &r) const {
        Bitset s;
        for (int i = 0; i < M; ++ i) {
            s.u[i] = u[i] | r.u[i];
        }
        return s;
    }
    void yo(const Bitset &r) {
        for (int i = 0; i < M; ++ i) {
            u[i] = (u[i] ^ r.u[i]) & r.u[i];
        }
    }
    void sub(const Bitset &r) {
        for (int i = 0; i < M; ++ i) u[i] = r.u[i] - u[i];
        for (int i = 0; i < M; ++ i) if (u[i] < 0) {
                u[i] += 1ll << W;
                u[i + 1] --;
            }
    }
    void shl() {
        long long c = 1;
        for (long i = 0; i < M; ++ i) {
            u[i] <<= 1;
            u[i] |= c;
            c = u[i] >> W & 1;
            u[i] ^= c << W;
        }
    }
    int count() const {
        int c = 0;
        for (int i = 0; i < M; ++ i) {
            c += __builtin_popcountll(u[i]);
        }
        return c;
    }
} row, bs[SIGMA], x;
int main() {
    for (int i = 0; i < SIGMA; ++i) bs[i].clear();
    string s, t;
    cin >> s >> t;
    int n = s.size(), m = t.size();
    for (int i = 0; i < n; ++ i) {
        bs[s[i] - 'a'].set(i);
    }
    M = n / W + (n % W != 0);
    row.clear();
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < M; ++j) {
            x.u[j] = row.u[j] | bs[t[i] - 'a'].u[j];
        }
        row.shl();
        row.sub(x);
        row.yo(x);
        //printf("%d\n", row.count()); //lcs of prefix of t and whole s
    }
    cout << row.count() << '\n';
    return 0;
}
//https://www.spoj.com/problems/LCS0/
