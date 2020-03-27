#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

/*
Directed MST is finding a spanning arborescence of minimum weight
An arborescence is a directed graph in which,a vertex u
called the root and for any other vertex v, there is exactly one
directed path from u to v. An arborescence is thus
the directed-graph form of a rooted tree,
complexity O(E * V) or faster
*/

const long long inf = 1e18;
struct edge
{
    int u, v; long long w; int id;
    edge(int u = 0, int v = 0, long long w = 0): u(u), v(v), w(w){}
    bool operator < (const edge &b) const{ return w < b.w; }
};

vector<edge> e[N], edges;
vector<int> g[N];
int par[N], col[N];
long long wt[N]; int snap[N];
bool used[N];
vector<int> cd;
long long directed_mst(int n, int m, int root)
{
    e[root].clear();
    for(int i = 0; i < n; i++){
        par[i] = i;
        sort(e[i].begin(), e[i].end());
    }
    bool cycle = 1;
    while(cycle){
        cycle = 0;
        memset(col, 0, sizeof col);
        col[root] = -1;
        for(int i = 0, t = 1; i < n; i++, t++){
            int u = par[i];
            if(col[u]) continue;
            int v;
            for(v = u; !col[v]; v = par[e[v][0].u]){
                col[v] = t;
                cd.push_back(e[v][0].id);
            }
            if(col[v] != t) continue;
            cycle = 1;
            long long sum = 0; int super = v;
            for(; col[v] == t; v = par[e[v][0].u]){
                col[v]++;
                sum += e[v][0].w;
            }
            for(int j = 0; j < n; j++) wt[j] = inf;
            for(; col[v] == t + 1; v = par[e[v][0].u]){
                col[v]--;
                for(int j = 1; j < e[v].size(); j++){
                    long long w = sum + e[v][j].w - e[v][0].w;
                    if(w < wt[e[v][j].u]){
                        wt[e[v][j].u] = w;
                        snap[e[v][j].u] = e[v][j].id;
                    }
                }
                par[v] = super;
            }
            e[super].clear();
            for(int j = 0; j < n; j++) if(par[j] != par[par[j]]) par[j] = par[par[j]];
            for(int j = 0; j < n; j++){
                if(wt[j] < inf && par[j] != super){
                    edge ed = edge(j, super, wt[j]);
                    ed.id = snap[j];
                    e[super].push_back(ed);
                }
            }
            sort(e[super].begin(),e[super].end());
        }
    }
    long long sum = 0;
    for(int i = 0; i < n; i++){
        if(i != root && par[i] == i){
            sum += e[i][0].w; // i-th node-s zero-th edge contains the minimum cost after directed mst algo.
        }
    }
    return sum;
}

long long solve(int n, int m, int root)
{
    for(int i = 0; i < n; i++){
        for(int j = 0; j < e[i].size(); j++) g[e[i][j].u].push_back(e[i][j].v);
    }
    queue<int> q;
    q.push(root);
    memset(col, 0, sizeof col);
    col[root] = 1;
    while(!q.empty()){
        int u = q.front();
        q.pop();
        for(int i = 0; i < g[u].size(); i++){
            int v = g[u][i];
            if(col[v]) continue;
            col[v] = 1;
            q.push(v);
        }
    }
    for(int i = 0; i < n; i++) if(!col[i]) return -1;
    return directed_mst(n, m, root);
}
//0-indexed
int main()
{
    int n, m; cin >> n >> m;
    for(int i = 0; i < m; i++){
        edge ed;
        cin >> ed.u >> ed.v >> ed.w; //u -> v
        ed.u--; ed.v--; ed.id = i;
        e[ed.v].push_back(ed);
        edges.push_back(ed);
    }
    int root = 0;
    long long res = solve(n, m, root);
    if(res == -1) cout << "impossible\n";
    else{
        memset(used, 0, sizeof used);
        memset(col, 0, sizeof col);
        for(int i = (int)cd.size() - 1; i >= 0; i--){
            edge ed = edges[cd[i]];
            if(col[ed.v]) continue;
            col[ed.v] = 1;
            used[cd[i]] = 1;
        }
        cout << res << '\n';
        for(int i = 0; i < m; i++) if(used[i]) cout << i + 1 << ' ';
        cout << '\n';
    }
    return 0;
}
