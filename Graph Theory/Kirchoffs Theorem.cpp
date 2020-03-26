#include<bits/stdc++.h>
using namespace std;

const int N = 105;

int pow(long long n, long long k, const int mod)
{
    int ans = 1 % mod; n %= mod;
    while(k > 0){
        if(k & 1) ans = 1LL * ans * n % mod;
        n = 1LL * n * n % mod;
        k >>= 1;
    }
    return ans;
}

int det(vector<vector<long long>>a, const int mod)
{
    int n=a.size();
    for(int i = 0; i < n; i++) for(int j = 0; j < n; j++) a[i][j] = (a[i][j] + mod) % mod;
    int det = 1;
    for (int i=0; i<n; ++i){
        int k = i;
        for (int j=i+1; j<n; ++j)  if (a[j][i] > a[k][i]) k = j;
        if(a[k][i] == 0) return 0;
        swap (a[i], a[k]);
        if (i != k) det = mod - det;
        det = a[i][i] * det % mod;
        int d = pow(a[i][i], mod - 2, mod);
        for (int j = i+1; j<n; ++j) a[i][j]  = a[i][j] * d % mod;
        for (int j=0; j<n; ++j) if (j != i && a[j][i]){
                for (int k=i+1; k<n; ++k)
                    a[j][k] -= a[i][k] * a[j][i] % mod, a[j][k] = (a[j][k] + mod) % mod;
        }
    }
    return det;
}
struct dsu {
	vector<int> par, rnk, size; int c;
	dsu(int n) : par(n+1), rnk(n+1,0), size(n+1,1), c(n) {
		for (int i = 1; i <= n; ++i) par[i] = i;
	}
	int find(int i) { return (par[i] == i ? i : (par[i] = find(par[i]))); }
	bool same(int i, int j) { return find(i) == find(j); }
	int get_size(int i) { return size[find(i)]; }
	int count() { return c; } //connected components
	int merge(int i, int j) {
		if ((i = find(i)) == (j = find(j))) return -1; else --c;
		if (rnk[i] > rnk[j]) swap(i, j);
		par[i] = j; size[j] += size[i];
		if (rnk[i] == rnk[j]) rnk[j]++;
		return j;
	}
};
int w[N][N], deg[N], vis[N];
vector<int> g[N];
//w[u][v]++, w[v][u]++, deg[u]++,deg[v]++
inline void add2(int u, int v)
{
    w[u][v]++;
    deg[u]++;
}
inline void add(int u, int v)
{
    if(u == v) return;
    g[u].push_back(v); g[v].push_back(u);
}
map<int, int> mp;
int get_id(int k)
{
    if(mp.find(k) == mp.end()) mp[k] = mp.size();
    return mp[k];
}
void dfs(int u)
{
    vis[u] = 1;
    for(auto v: g[u]){
        if(!vis[v]) dfs(v);
        add2(get_id(u), get_id(v));
    }
}
int extended_euclid(int a, int b, int &x, int &y) {
    int xx = y = 0;
    int yy = x = 1;
    while (b) {
        int q = a / b;
        int t = b; b = a%b; a = t;
        t = xx; xx = x - q*xx; x = t;
        t = yy; yy = y - q*yy; y = t;
    }
    return a;
}

pair<int, int> CRT(int x, int a, int y, int b) {
	int s, t;
	int d = extended_euclid(x, y, s, t);
	if (a % d != b % d) return make_pair(0, -1);
	int m=x*y;
	s%=m;
	t%=m;
	s=(s+m)%m;
	t=(t+m)%m;
	return make_pair( (s * b%m * x%m + t * a%m * y%m)% m/d, m/ d);
}
int32_t main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n, m; cin >> n >> m;
    vector< array<int, 3> > ed;
    for(int i = 1; i <= m; i++){
        int u, v, w; cin >> u >> v >> w;
        ed.push_back({w, u, v});
    }
    sort(ed.begin(), ed.end());
    dsu d(n);
    int ans1 = 1, ans2 = 1;
    for(int i = 0, j; i < m; i = j){
        memset(vis, 0, sizeof vis);
        for(int i = 1; i <= n; i++) g[i].clear();
        add(d.find(ed[i][1]), d.find(ed[i][2]));
        for(j = i + 1; j < m && ed[i][0] == ed[j][0]; j++) add(d.find(ed[j][1]), d.find(ed[j][2]));
        for(int u = 1; u <= n; u++) if(!vis[u]){
            memset(w, 0, sizeof w);
            memset(deg, 0, sizeof deg);
            dfs(u);
            int sz = mp.size();
            mp.clear();
            if(sz <= 1) continue;
            for(int u = 1; u <= sz; u++) for(int v = 1; v <= sz; v++){
                if(u == v) w[u][v] = deg[u];
                else w[u][v] *= -1;
            }
            vector< vector<long long> > mat(sz - 1, vector<long long> (sz - 1, 0));
            for(int i = 1; i < sz; i++) for(int j = 1; j < sz; j++) mat[i - 1][j - 1] = w[i][j];
            ans1 = ans1 * det(mat, 3) % 3;
            ans2 = ans2 * det(mat, 10337) % 10337;
        }
        for(int k = i; k < j; k++) d.merge(ed[k][1], ed[k][2]);
    }
    cout << CRT(3, ans1, 10337, ans2).first << '\n';
    return 0;
}
/*
https://www.spoj.com/problems/MSTS/
*/
