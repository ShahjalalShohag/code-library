#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9, LG = 16;

vector<pair<int, int>> g[N];
int sz[N];
int tot, done[N], cenpar[N];
void calc_sz(int u, int p) {
    tot ++; sz[u] = 1;
    for (auto e: g[u]) {
    	int v = e.first;
        if(v == p || done[v]) continue;
        calc_sz(v, u);
        sz[u] += sz[v];
    }
}
int find_cen(int u, int p) {
    for (auto e: g[u]) {
        int v = e.first;
        if(v == p || done[v]) continue;
        else if(sz[v] > tot / 2) return find_cen(v, u);
    }
    return u;
}
pair<long long, long long> extendedEuclid(long long a, long long b) { // returns x, y | ax + by = gcd(a,b)
    if(b == 0) return pair<long long, long long>(a>=0?1LL:-1LL , 0LL);
    else {
        pair<long long, long long> d = extendedEuclid(b, a % b);
        return pair<long long, long long>(d.second, d.first - d.second * (a / b));
    }

}
long long inv(long long a, long long n) {///a and n is coprime
    pair<long long, long long> ret = extendedEuclid(a, n);
    return ((ret.first % n) + n) % n;
}
int pw[N], invpw[N], mod;
map<int, int> mp;
void add(int u, int p, int dep, int down, int up) {
    mp[up]++;
    for (auto e: g[u]) {
        int v = e.first, w = e.second;
        if(v == p || done[v]) continue;
        int D = (1LL * down * 10 % mod + w) % mod;
        int U = (1LL * w * pw[dep] % mod + up) % mod;
        add(v, u, dep + 1, D, U);
    }
}
long long ans, extra;
void yo(int u, int p, int dep, int down, int up) {
    int x = (-1LL * down * invpw[dep] % mod + mod) % mod;
    if(mp.find(x) != mp.end()) ans += mp[x];
    extra += up == 0;
    extra += down == 0;
    for (auto e: g[u]) {
        int v = e.first, w = e.second;
        if(v == p || done[v]) continue;
        int D = (1LL * down * 10 % mod + w) % mod;
        int U = (1LL * w * pw[dep] % mod + up) % mod;
        yo(v, u, dep + 1, D, U);
    }
}
void decompose(int u, int p  = 0) {
    tot = 0;
    calc_sz(u, p);
    int cen = find_cen(u, p);
    cenpar[cen] = p;
    done[cen] = 1;
    mp.clear();
    extra = 0;
    for (auto e: g[cen]) {
        int v = e.first, w = e.second;
        if(v == p || done[v]) continue;
        yo(v, cen, 1, w, w);
        add(v, cen, 1, w, w);
    } 
    ans += extra;
    mp.clear();
    reverse(g[cen].begin(), g[cen].end()); 
    for (auto e: g[cen]) {
        int v = e.first, w = e.second;
        if(v == p || done[v]) continue;
        yo(v, cen, 1, w, w);
        add(v, cen, 1, w, w);
    }
    reverse(g[cen].begin(), g[cen].end());
    for (auto e: g[cen]) {
        int v = e.first;
        if(v == p || done[v]) continue;
        decompose(v, cen);
    }
}
int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n; cin >> n >> mod;
    pw[0] = 1;
    for (int i = 1; i < N; i++) pw[i] = 1LL * pw[i - 1] * 10 % mod;
    for (int i = 0; i < N; i++) invpw[i] = inv(pw[i], mod);
    for (int i = 1; i < n; i++) {
        int u, v, w; cin >> u >> v >> w; u++, v++;
        g[u].push_back({v, w % mod});
        g[v].push_back({u, w % mod});
    }
    decompose(1);
    cout << ans << '\n';
    return 0;
}
//https://codeforces.com/problemset/problem/715/C