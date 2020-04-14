#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9, mod = 1e9 + 9;

template <const int32_t MOD>
struct modint {
    int32_t value;
    modint() = default;
    modint(int32_t value_) : value(value_) {}
    inline modint<MOD> operator + (modint<MOD> other) const { int32_t c = this->value + other.value; return modint<MOD>(c >= MOD ? c - MOD : c); }
    inline modint<MOD> operator - (modint<MOD> other) const { int32_t c = this->value - other.value; return modint<MOD>(c <    0 ? c + MOD : c); }
    inline modint<MOD> operator * (modint<MOD> other) const { int32_t c = (int64_t)this->value * other.value % MOD; return modint<MOD>(c < 0 ? c + MOD : c); }
    inline modint<MOD> & operator += (modint<MOD> other) { this->value += other.value; if (this->value >= MOD) this->value -= MOD; return *this; }
    inline modint<MOD> & operator -= (modint<MOD> other) { this->value -= other.value; if (this->value <    0) this->value += MOD; return *this; }
    inline modint<MOD> & operator *= (modint<MOD> other) { this->value = (int64_t)this->value * other.value % MOD; if (this->value < 0) this->value += MOD; return *this; }
    inline modint<MOD> operator - () const { return modint<MOD>(this->value ? MOD - this->value : 0); }
    modint<MOD> pow(uint64_t k) const {
        modint<MOD> x = *this, y = 1;
        for (; k; k >>= 1) {
            if (k & 1) y *= x;
            x *= x;
        }
        return y;
    }
    modint<MOD> inv() const { return pow(MOD - 2); }  // MOD must be a prime
    inline modint<MOD> operator /  (modint<MOD> other) const { return *this *  other.inv(); }
    inline modint<MOD> operator /= (modint<MOD> other)       { return *this *= other.inv(); }
    inline bool operator == (modint<MOD> other) const { return value == other.value; }
    inline bool operator != (modint<MOD> other) const { return value != other.value; }
    inline bool operator < (modint<MOD> other) const { return value < other.value; }
    inline bool operator > (modint<MOD> other) const { return value > other.value; }
};
template <int32_t MOD> modint<MOD> operator * (int64_t value, modint<MOD> n) { return modint<MOD>(value) * n; }
template <int32_t MOD> modint<MOD> operator * (int32_t value, modint<MOD> n) { return modint<MOD>(value % MOD) * n; }
template <int32_t MOD> ostream & operator << (ostream & out, modint<MOD> n) { return out << n.value; }

using mint = modint<mod>;


mint f[N];
mint get_kth(mint a, mint b, int k) {
    if(k == 1) return a;
    if(k == 2) return b;
    return a * f[k-2] + b * f[k-1];
}
template<const int ty> //ty = 1 : normal tree, ty = 2: reverse tree
struct PST
{
    #define lc t[cur].l
    #define rc t[cur].r
    struct node
    {
        int l = 0, r = 0; mint lf = 0, ls = 0; mint val = 0;
    }t[400 * N];
    int T = 0;
    int build(int b, int e) {
        int cur = ++T;
        if(b == e) return cur;
        int mid = b + e >> 1;
        lc = build(b, mid);
        rc = build(mid + 1, e);
        return cur;
    }
    inline int push(int pre, int b, int e, mint af = 0, mint as = 0) {
        if(ty == 2){
            int cur = ++T; t[cur] = t[pre];
            t[cur].lf += af; t[cur].ls += as;
            if(b == e) t[cur].val += t[cur].lf;
            else {
                if(lc < pre) lc = ++T; if(rc < pre) rc = ++T;
                t[lc] = t[t[pre].l];
                t[rc] = t[t[pre].r];
                int len = e - b + 1;
                t[cur].val = t[cur].val + get_kth(t[cur].lf, t[cur].ls, len + 2) - t[cur].ls;
                t[rc].lf = t[rc].lf + t[cur].lf;
                t[rc].ls = t[rc].ls + t[cur].ls;
                int mid = (b + e) >> 1;
                t[lc].lf = t[lc].lf + get_kth(t[cur].lf, t[cur].ls, e - mid + 1);
                t[lc].ls = t[lc].ls + get_kth(t[cur].lf, t[cur].ls, e - mid + 2);
            }
            t[cur].lf = t[cur].ls = 0;
            return cur;
        }
        else{
            int cur = ++T; t[cur] = t[pre];
            t[cur].lf += af; t[cur].ls += as;
            if(b == e) t[cur].val += t[cur].lf;
            else {
                if(lc < pre) lc = ++T; if(rc < pre) rc = ++T;
                t[lc] = t[t[pre].l];
                t[rc] = t[t[pre].r];
                int len = e - b + 1;
                t[cur].val = t[cur].val + get_kth(t[cur].lf, t[cur].ls, len + 2) - t[cur].ls;
                t[lc].lf = t[lc].lf + t[cur].lf;
                t[lc].ls = t[lc].ls + t[cur].ls;
                int mid = (b + e) >> 1;
                t[rc].lf = t[rc].lf + get_kth(t[cur].lf, t[cur].ls, mid - b + 1 + 1);
                t[rc].ls = t[rc].ls + get_kth(t[cur].lf, t[cur].ls, mid - b + 1 + 2);
            }
            t[cur].lf = t[cur].ls = 0;
            return cur;
        }
    }
    int upd(int pre, int b, int e, int i, int j, int x) {
        int cur = push(pre, b, e);
        if(b > j || e < i) return cur;
        if(i <= b && e <= j){
            if(ty == 2) cur = push(cur, b, e, f[j - e + 1 + x], f[j - e + 2 + x]);
            else cur = push(cur, b, e, f[b - i + 1 + x], f[b - i + 2 + x]);
            return cur;
        }
        int mid = b + e >> 1;
        lc = upd(lc, b, mid, i, j, x);
        rc = upd(rc, mid + 1, e, i, j, x);
        t[cur].val = t[lc].val + t[rc].val;
        return cur;
    }
    mint query(int cur, int b, int e, int i, int j) {
        cur = push(cur, b, e);
        if (e < i || j < b) return 0;
        if (i <= b && e <= j) return t[cur].val;
        int mid = b + e >> 1;
        return query(lc, b, mid, i , j) + query(rc, mid + 1, e, i, j);
    }
};
PST<1> t;
PST<2> r;
const int LG = 17;
vector<int> g[N];
int par[N][LG + 1], dep[N], sz[N];
void dfs(int u, int p = 0) {
    par[u][0] = p;
    dep[u] = dep[p] + 1;
    sz[u] = 1;
    for (int i = 1; i <= LG; i++) par[u][i] = par[par[u][i - 1]][i - 1];
    if (p) g[u].erase(find(g[u].begin(), g[u].end(), p));
    for (auto &v: g[u]) if (v != p) {
        dfs(v, u);
        sz[u] += sz[v];
        if(sz[v] > sz[g[u][0]]) swap(v, g[u][0]);
    }
}
int lca(int u, int v) {
    if (dep[u] < dep[v]) swap(u, v);
    for (int k = LG; k >= 0; k--) if (dep[par[u][k]] >= dep[v]) u = par[u][k];
    if (u == v) return u;
    for (int k = LG; k >= 0; k--) if (par[u][k] != par[v][k]) u = par[u][k], v = par[v][k];
    return par[u][0];
}
int dist(int u, int v) {
    int l = lca(u, v);
    return dep[u] + dep[v] - (dep[l] << 1);
}
int kth(int u, int k) {
    assert(k >= 0);
    for (int i = 0; i <= LG; i++) if (k & (1 << i)) u = par[u][i];
    return u;
}
int T, head[N], st[N], en[N];
void dfs_hld(int u) {
    st[u] = ++T;
    for (auto v: g[u]) {
        head[v] = (v == g[u][0] ? head[u] : v);
        dfs_hld(v);
    }
    en[u] = T;
}
int n;
vector<array<int, 3>> query_up(int u, int v, int ty) {
    vector<array<int, 3>> ans;
    while(head[u] != head[v]){
        ans.push_back({st[head[u]], st[u], ty});
        u = par[head[u]][0];
    }
    ans.push_back({st[v], st[u], ty});
    return ans;
}
vector<array<int, 3>> query(int u, int v) {
    int l = lca(u, v);
    auto ans = query_up(u, l, 0);
    vector<array<int, 3>> res;
    if (v != l) res = query_up(v, kth(v, dep[v] - dep[l] - 1), 1);
    reverse(res.begin(), res.end());
    for (auto p: res) ans.push_back(p);
    return ans;
}
int root[N], rootr[N];
int32_t main() {
    f[1] = 1; f[2] = 1;
    for(int i =  3; i < N; i ++) f[i] = f[i - 1] + f[i - 2];
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int q; cin >> n >> q;
    for (int i = 1; i < n; i++) {
        int u, v; cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs(1);
    head[1] = 1; dfs_hld(1);
    root[0] = t.build(1, n);
    rootr[0] = r.build(1, n);
    int last = 0;
    for(int i = 1; i <= q; i++) {
        string ty; cin >> ty;
        int p = root[i - 1], pr = rootr[i - 1];
        root[i] = p; rootr[i] = pr;
        if (ty == "A") {
            int u, v; cin >> u >> v;
            u ^= last;
            if(u < 1 || u > n) while(1);
            auto path = query(u, v);
            int x = 0;
            for (auto e : path) {
                if(e[2]) p = t.upd(p, 1, n, e[0], e[1], x);
                else pr = r.upd(pr, 1, n, e[0], e[1], x);
                x += e[1] - e[0] + 1;
            }
        }
        else if (ty == "QS") {
            int u, v; cin >> u >> v;
            u ^= last;
            if(u < 1 || u > n) while(1);
            mint ans = 0;
            if (u == v) {
                ans = t.query(p, 1, n, 1, n) + r.query(pr, 1, n, 1, n);
            }
            else if (lca(u, v) == v) {
                int d = dist(u, v);
                int x = kth(u, d - 1);
                ans = t.query(p, 1, n, 1, n) + r.query(pr, 1, n, 1, n);
                ans = ans - t.query(p, 1, n, st[x], en[x])-r.query(pr, 1, n, st[x], en[x]);
            }
            else ans = t.query(p, 1, n, st[v], en[v]) + r.query(pr, 1, n, st[v], en[v]);
            cout << ans << '\n';
            last = ans.value;
        }
        else if (ty == "QC") {
            int u, v; cin >> u >> v;
            u ^= last;
            if(u < 1 || u > n) while(1);
            auto path = query(u, v);
            mint ans = 0;
            for (auto e: path) {
                ans += t.query(p, 1, n, e[0], e[1]) + r.query(pr, 1, n, e[0], e[1]);
            }
            cout << ans << '\n';
            last = ans.value;
        }
        else {
            int k; cin >> k; k ^= last;
            if(k < 0 || k > i) while(1);
            p = root[k];
            pr = rootr[k];
        }
        root[i] = p;
        rootr[i] = pr;
    }
    return 0;
}
//https://www.codechef.com/problems/FIBTREE