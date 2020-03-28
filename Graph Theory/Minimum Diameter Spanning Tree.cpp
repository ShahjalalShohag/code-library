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

//O(n * m)
long long MDST()
{
    if(n == 1) return 0;
    APSP();
    long long ans = inf;
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
        ans = min(ans, nw);
    }
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
    cout << fixed << setprecision(2) << ans / 2.0 << '\n';
    return 0;
}
//https://codeforces.com/contest/266/problem/D
