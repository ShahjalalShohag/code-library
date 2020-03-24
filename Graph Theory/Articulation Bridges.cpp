#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

//diameter of the Articulation Bridge Tree
int T, low[N], dis[N];
vector<int> g[N];
void dfs(int u, int pre = 0)
{
    low[u] = dis[u] = ++T;
    for(auto v: g[u]){
        if(!dis[v]){
            dfs(v, u);
            low[u] = min(low[u], low[v]); //bridge if low[v] > dis[u]
        }
        else if(v != pre) low[u] = min(low[u], dis[v]);
    }
}

vector<int> bt[N];
bool vis[N];
int sz[N]; //size of each component in the bridge tree
void make_tree(int u, int pre = 0)
{
    vis[u] = 1;
    if(low[u] == dis[u]){
        int v = ++T;
        if(pre != 0) bt[pre].push_back(v), bt[v].push_back(pre);
        pre = v;
    }
    sz[pre] ++;
    for(auto v: g[u]) if(!vis[v]) make_tree(v, pre);
}

int dep[N];
int diameter(int u, int pre = 0, int d = 0)
{
    dep[u] = d;
    int ans = u;
    for(auto v: bt[u]) if(v != pre){
        int nw = diameter(v, u, d + 1);
        if(dep[nw] > dep[ans]) ans = nw;
    }
    return ans;
}

int32_t main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n, m; cin >> n >> m;
    while(m--){
        int u, v; cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs(1);
    T = 0; make_tree(1);
    cout << dep[diameter(diameter(1))] << '\n';
}

