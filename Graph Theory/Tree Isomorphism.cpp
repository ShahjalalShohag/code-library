#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, mod = 998244353;

int pw[N];
const int base = 10;
struct Tree
{
    int n;
    vector<vector<int>> g;
    Tree(){}
    Tree(int _n) : n(_n){g.resize(n + 1);}
    void add_edge(int u, int v)
    {
        g[u].push_back(v);
        g[v].push_back(u);
    }
    vector<int> bfs(int s)
    {
        queue<int> q;
        vector<int> d(n + 1, n * 2); d[0] = -1;
        q.push(s); d[s] = 0;
        while(!q.empty()){
            int u = q.front(); q.pop();
            for(auto v: g[u]) if(d[u] + 1 < d[v]){
                d[v] = d[u] + 1;
                q.push(v);
            }
        }
        return d;
    }
    vector<int> get_centers()
    {
        auto du = bfs(1);
        int v = max_element(du.begin(), du.end()) - du.begin();
        auto dv = bfs(v);
        int u = max_element(dv.begin(), dv.end()) - dv.begin();
        du = bfs(u);
        vector<int> ans;
        for(int i = 1; i <= n; i++) if(du[i] + dv[i] == du[v] && du[i] >= du[v] / 2 && dv[i] >= du[v] / 2){
            ans.push_back(i);
        }
        return ans;
    }
    pair<int, int> yo(int u, int pre = 0)
    {
        vector<pair<int, int>> nw;
        for(auto v: g[u]) if(v != pre) nw.push_back(yo(v, u));
        sort(nw.begin(), nw.end());
        int ans = 0; int len = 0;
        for(auto x: nw){
            int v = x.first; int l = x.second;
            ans = 1LL * ans * pw[l] % mod; ans += v; if(ans >= mod) ans -= mod;
            len += l;
        }
        ans += pw[len]; ans = 1LL * ans * 10 % mod; len += 2;
        return {ans, len};
    }
    bool iso(Tree &t)
    {
        auto a = get_centers();
        auto b = t.get_centers();
        for(auto x: a) for(auto y: b) if(yo(x) == t.yo(y)) return 1;
        return 0;
    }
};
int32_t main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    pw[0] = 1;
    for(int i = 1; i < N; i++) pw[i] = 1LL * pw[i - 1] * base % mod;
    int t; cin >> t;
    while(t--){
        int n; cin >> n;
        Tree t1(n);
        for(int i = 1; i < n; i++){
            int u, v; cin >> u >> v;
            t1.add_edge(u, v);
        }
        Tree t2(n);
        for(int i = 1; i < n; i++){
            int u, v; cin >> u >> v;
            t2.add_edge(u, v);
        }
        if(t1.iso(t2)) cout << "YES\n";
        else cout << "NO\n";
    }
    return 0;
}
