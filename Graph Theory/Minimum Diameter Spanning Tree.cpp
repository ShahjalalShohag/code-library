#include<bits/stdc++.h>
using namespace std;

const int N = 220;

const long long inf = 1e18;
long long d[N][N], n, m;
vector<array<int, 3>> ed;

void APSP()
{
    for(int i = 1; i <= n; i++) for(int j = 1; j <= n; j++) d[i][j] = i == j ? 0 : inf;
    for(int i = 0; i < m; i++){
        int u = ed[i][0], v = ed[i][1], w = ed[i][2];
        d[u][v] = d[v][u] = min(d[u][v], w * 2LL); //calculate shortest path using double weights
    }
    for(int k = 1; k <= n; k++) for(int i = 1; i <= n; i++) for(int j = 1; j <= n; j++) d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
}

bool vis[N];
vector<pair<int, int>> e;
long long D[N];
void dfs(int u) //shortest path tree from root
{
    vis[u] = 1;
    for(int i = 0; i < m; i++){
        int x = ed[i][0], y = ed[i][1], w = ed[i][2];
        if(x != u && y != u) continue;
        if(x == u && y == u) continue;
        if(y == u) swap(x, y);
        if(!vis[y] && D[u] + w * 2 == D[y]){
            e.push_back({u, y});
            dfs(y);
        }
    }
}
//O(n * m)
long long MDST()
{
    if(n == 1) return 0;
    APSP();
    long long ans = inf;
    pair<int, int> r;
    for(auto p : ed){
        int u = p[0], v = p[1], w= p[2];
        vector<pair<long long, long long>> a;
        for(int i = 1; i <= n; i++) a.push_back({d[u][i], d[v][i]});
        sort(a.begin(), a.end());
        reverse(a.begin(), a.end());
        pair<long long, long long> cur = a[0];
        long long nw = cur.first;
        for(int i = 1; i < n; i++){
            if(a[i].second > cur.second){
                nw = min(nw, (w * 2+ cur.second + a[i].first) / 2);
                cur = a[i];
            }
        }
        nw = min(nw, cur.second);
        if(nw < ans){
            ans = nw;
            r = {u, v};
        }
    }
    e.push_back(r); //r = center edge of the diameter
    int u = r.first, v = r.second;
    for(int i = 1; i <= n; i++) D[i] = min(d[u][i], d[v][i]);
    dfs(u);
    dfs(v);
    return ans;
}

int32_t main()
{
    cin >> n >> m;
    for(int i = 1; i <= m; i++){
        int u, v, w; cin >> u >> v >> w;
        ed.push_back({u, v, w});
    }
    long long ans = MDST();
    cout << ans << '\n';
    assert(e.size() == n - 1);
    for(auto &x: e) if(x.second < x.first) swap(x.first, x.second);
    sort(e.begin(), e.end());
    for(auto &x: e){
        cout << x.first << ' ' << x.second << '\n';
    }
    return 0;
}
//https://www.spoj.com/problems/PT07C/
