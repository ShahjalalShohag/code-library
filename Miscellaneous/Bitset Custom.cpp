#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,avx2,fma")
#pragma GCC optimize("unroll-loops")

#include<bits/stdc++.h>
using namespace std;

const int N = 2e5 + 9;

const unsigned long long all = ULLONG_MAX;
const int sz = N / 64 + 2;
unsigned long long o[64][64], z[64][64];
void init()
{
    for(int i = 0; i < 64; i++) {
        for(int j = i; j < 64; j++) {
            if(j == i) o[i][j] = 1ull << j;
            else o[i][j] = o[i][j-1]| (1ull << j);
            z[i][j] = all ^ o[i][j];
        }
    }
}
template<const int S>
struct Bitset
{
    const static int sz = (S + 63) / 64, extra = 64 - sz * 64 + S;
    unsigned long long a[sz];
    Bitset() { memset(a, 0, sizeof a); }
    bool operator [] (const int i) {
        return (bool)(a[i / 64] & (1ull << (i & 63)));
    }
    void set(int i) {
        a[i / 64] |= (1ull << (i & 63));
    }
    void set() {
        for(int i = 0; i < sz; i++) a[i] = all;
    }
    void reset(int i) {
        a[i / 64] &= (all ^ (1ull << (i & 63)));
    }
    void reset() {
        memset(a, 0, sizeof a);
    }
    void flip(int i) {
        a[i / 64] ^= (1ull << (i & 63));
    }
    int count() {
        int ans = 0; for(int i = 0; i < sz; i++) ans += __builtin_popcountll(a[i]);
        return ans;
    }
    void set(int l, int r) {
        if(l / 64 == r / 64) {
            a[l / 64] |= o[l & 63][r & 63]; return;
        }
        a[l / 64] |= o[l & 63][63];
        a[r / 64] |= o[0][r & 63];
        for(int i = l / 64 + 1; i < r / 64; i++) a[i] = all;
    }
    void reset(int l, int r) {
        if(l / 64 == r / 64) {
            a[l / 64] &= z[l & 63][r & 63]; return;
        }
        a[l / 64] &= z[l & 63][63];
        a[r / 64] &= z[0][r & 63];
        for(int i = l / 64 + 1; i < r / 64; i++) a[i] = 0;
    }
    void flip(int l, int r) {
        if(l / 64 == r / 64) {
            a[l / 64] ^= o[l & 63][r & 63]; return;
        }
        a[l / 64] ^= o[l & 63][63];
        a[r / 64] ^= o[0][r & 63];
        for(int i = l / 64 + 1; i < r / 64; i++) a[i] = ~a[i];
    }
    inline Bitset operator & (const Bitset &x) const {
        Bitset ans; for(int i = 0; i < sz; i++) ans.a[i] = a[i] & x.a[i];
        return ans;
    }
    inline Bitset operator | (const Bitset &x) const {
        Bitset ans; for(int i = 0; i < sz; i++) ans.a[i] = a[i] | x.a[i];
        return ans;
    }
    inline Bitset operator ^ (const Bitset &x) const {
        Bitset ans; for(int i = 0; i < sz; i++) ans.a[i] = a[i] ^ x.a[i];
        return ans;
    }
    inline Bitset operator << (const int k) const {
        if(k == 0) return *this;
        assert(k >= 0);
        Bitset b;
        int s1 = k & 63, s2 = 64 - s1;
        for (int i = sz - 1, j = i - (k + 63) / 64; j > -2; i--, j--){
            b.a[i] = a[j+1] << s1;
            if (j > -1) b.a[i] |= a[j] >> s2;
        }
        if (extra != 64) b.a[sz - 1] ^= (b.a[sz - 1] >> extra) << extra;
        return b;
    }
    inline Bitset operator >> (const int k) const {
        if(k == 0) return *this;
        assert(k >= 0);
        Bitset b;
        int s1 = k & 63, s2 = 64 - s1;
        for (int i = 0, j = (k + 63) / 64; j <= sz; i++, j++){
            b.a[i] = a[j - 1] >> s1;
            if (j < sz ) b.a[i] |= a[j] << s2;
        }
        return b;
    }
    inline bool operator == (const Bitset &x) const {
        for(int i = 0; i < sz; i++) if(a[i] != x.a[i]) return false;
        return true;
    }
    inline bool operator > (const Bitset &x) const {
        for(int i = sz - 1; i >= 0; i--) if(a[i] != x.a[i]) return a[i] > x.a[i];
        return false;
    }
    inline bool operator < (const Bitset &x) const {
        for(int i = sz - 1; i >= 0; i--) if(a[i] != x.a[i]) return a[i] < x.a[i];
        return false;
    }
    friend Bitset operator ~ (const Bitset &x) {
        Bitset ans; for(int i = 0; i < sz; i++) ans.a[i] = ~x.a[i];
        return ans;
    }
    int _Find_first() {
        for(int i = 0, ans = 0; i < sz; i++, ans += 64){
            if(a[i]) return ans + __builtin_ctzll(a[i]);
        }
        return S;
    }
    int _Find_next(int k) {
        int id = k / 64; for(int i = (k & 63)+ 1; i < 64; i++) if((a[id] >> i) & 1) return id * 64 + i;
        for(int i = id + 1, ans = (id + 1) * 64; i < sz; i++, ans += 64){
            if(a[i]) return ans + __builtin_ctzll(a[i]);
        }
        return S;
    }
};

int a[N], ans[N];
Bitset<N> bs[101], nw, t;
int32_t main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    init();
    int n; cin >> n;
    for(int i = 1; i <= n; i++){
        int k; cin >> k; bs[k].set(i);
    }
    int q; cin >> q;
    while(q--){
        int l, r, x, y; cin >> l >> r >> x >> y;
        if(x == y) continue;
        nw.reset();
        nw.set(l, r);
        t = bs[x] & nw;
        bs[x].reset(l, r);
        bs[y] = bs[y] | t;
    }
    for(int i = 1; i <= 100; i++){
        int p = bs[i]._Find_first();
        while(p < N){
            ans[p] = i;
            p = bs[i]._Find_next(p);
        }
    }
    for(int i = 1; i <= n; i++) cout << ans[i] << " \n"[i == n];
    return 0;
}
//https://codeforces.com/contest/911/problem/G
