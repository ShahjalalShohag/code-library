#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;

#define ll long long
#define ull unsigned long long
#define ld long double
#define pii pair<int,int>
#define pll pair<ll,ll>
#define vi vector<int>
#define vll vector<ll>
#define vc vector<char>
#define vs vector<string>
#define vpll vector< pair<ll,ll> >
#define umap unordered_map
#define uset unordered_set
#define PQ priority_queue

#define printa(a,L,R) for(int i=L;i<R;i++) cout<<a[i]<<(i==R-1?'\n':' ')
#define printv(a) printa(a,0,a.size())
#define print2d(a,r,c) for(int i=0;i<r;i++) for(int j=0;j<c;j++) cout<<a[i][j]<<(j==c-1?'\n':' ')
#define pb push_back
#define eb emplace_back
#define mt make_tuple
#define fbo find_by_order
#define ook order_of_key
#define MP make_pair
#define UB upper_bound
#define LB lower_bound
#define SQ(x) ((x)*(x))
#define issq(x) (((ll)(sqrt((x))))*((ll)(sqrt((x))))==(x))
#define F first
#define S second
#define mem(a,x) memset(a,x,sizeof(a))
#define inf 0x3f3f3f3f
#define PI (acos(-1.0))
#define E 2.71828182845904523536
#define gamma 0.5772156649
#define nl "\n"
#define lg(r,n) (int)(log2(n)/log2(r))
#define pf printf
#define sf scanf
#define _ccase printf("Case %lld: ",++cs)
#define _case cout<<"Case "<<++cs<<": "
#define by(x) [](const auto& a, const auto& b) { return a.x < b.x; }

#define asche cerr<<"Ekhane asche\n";
#define rev(v) reverse(v.begin(),v.end())
#define srt(v) sort(v.begin(),v.end())
#define grtsrt(v) sort(v.begin(),v.end(),greater<int>())
#define all(v) v.begin(),v.end()
#define mnv(v) *min_element(v.begin(),v.end())
#define mxv(v) *max_element(v.begin(),v.end())
#define toint(a) atoi(a.c_str())
#define fast ios_base::sync_with_stdio(false)
#define valid(tx,ty) (tx>=0&&tx<n&&ty>=0&&ty<m)
#define dist(x1,y1,x2,y2) sqrt(SQ(x1-x2)+SQ(y1-y2))
#define one(x) __builtin_popcount(x)
#define Unique(v) v.erase(unique(all(v)),v.end())
string tostr(int n) {stringstream rr;rr<<n;return rr.str();}
template <typename T> using o_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
//ll dx[]={1,0,-1,0,1,-1,-1,1};
//ll dy[]={0,1,0,-1,1,1,-1,-1};

#define debug(args...) { string _s = #args; replace(_s.begin(), _s.end(), ',', ' '); stringstream _ss(_s); istream_iterator<string> _it(_ss); deb(_it, args); }
void deb(istream_iterator<string> it) {}
template<typename T, typename... Args>
void deb(istream_iterator<string> it, T a, Args... args) {
    cerr << *it << " = " << a << endl;
    deb(++it, args...);
}

const int mod=1e9+7;
const int mxn=40005,mxm=1e5+9;
const ld eps=1e-9;
//ll gcd(ll a,ll b){while(b){ll x=a%b;a=b;b=x;}return a;}
//ll lcm(ll a,ll b){return a/gcd(a,b)*b;}
//ll qpow(ll n,ll k) {ll ans=1;assert(k>=0);while(k>0){if(k&1) ans=(ans*n)%mod;n=(n*n)%mod;k>>=1;}return ans;}
///unique elements in path u to v
int ans,curl,curr,timee,que,block,val[mxn],node[mxn*2],n;
int st[mxn],en[mxn],level[mxn],par[mxn][20],cnt[mxn],id[mxn],sol[mxm];
bool vis[mxn];
vi g[mxn];
void remov(int u);
struct sj
{
    int l,r,lc,idx;
}q[mxm];
void dfs(int u,int prev)
{
    par[u][0]=prev;
    level[u]=level[prev]+1;
    node[timee]=u;
    st[u]=timee++;
    for(auto v:g[u]){
        if(v==prev) continue;
        dfs(v,u);
    }
    node[timee]=u;
    en[u]=timee++;
}
int lca(int u,int v)
{
    if(level[u]<level[v]) swap(u,v);
    for(int k=19;k>=0;k--) if(level[par[u][k]]>=level[v]) u=par[u][k];
    if(u==v) return u;
    for(int k=19;k>=0;k--) if(par[u][k]!=par[v][k]) u=par[u][k],v=par[v][k];
    return par[u][0];
}
bool cmp(sj x,sj y)
{
    int l=(x.l-1)/block;
    int r=(y.l-1)/block;
    if(l==r) return x.r<y.r;
    else return l<r;
}
//void chk(int u)
//{
//    int x=id[u];
//    if(vis[u]==0&&(cnt[x]++==0)) ans++;
//    else if(vis[u]&&(--cnt[x]==0)) ans--;
//    vis[u]^=1;
//}
void add(int u)
{
    int x=id[u];
    if(cnt[x]++==0) ans++;
    return;
}
void remov(int u)
{
    int x=id[u];
    if(--cnt[x]==0) ans--;
    return;
}
void add_list(int u)
{
    if(vis[u]==0) add(u);
    else remov(u);
    vis[u]^=1;
}
void remov_list(int u)
{
    if(vis[u]==0) add(u);
    else remov(u);
    vis[u]^=1;
}
void mos_algo()
{
    int i,j,k,u,v,l,r;
    sort(q,q+que,cmp);
    curl=q[0].l,curr=q[0].l-1;
    ans=0;
    for(i=0;i<que;i++){
        l=q[i].l,r=q[i].r;
        while(curl<l) remov_list(node[curl++]);
        while(curl>l) add_list(node[--curl]);
        while(curr<r) add_list(node[++curr]);
        while(curr>r) remov_list(node[curr--]);
        u=node[curl],v=node[curr];
        if(q[i].lc!=u&&q[i].lc!=v) add_list(q[i].lc);
        sol[q[i].idx]=ans;
        if(q[i].lc!=u&&q[i].lc!=v) remov_list(q[i].lc);
    }
}
void compress()
{
    map<int,int>mp;
    for(int i=1;i<=n;i++){
        if(mp.find(val[i])==mp.end()) mp[val[i]]=mp.size();
        id[i]=mp[val[i]];
    }
}
int main()
{
    fast;
    int i,j,k,m,u,v;
    while(cin>>n>>que){
        timee=1;
        ans=0;
        for(i=1;i<=n;i++) cin>>val[i];
        compress();
        for(i=1;i<n;i++) cin>>u>>v,g[u].pb(v),g[v].pb(u);
        dfs(1,0);
        for(k=1;k<20;k++) for(i=1;i<=n;i++) par[i][k]=par[par[i][k-1]][k-1];
        timee--;
        block=(int)(sqrt(timee)+eps);
        for(i=0;i<que;i++){
            cin>>u>>v;
            if(level[u]>level[v]) swap(u,v);
            q[i].lc=lca(u,v);
            if(q[i].lc==u) q[i].l=st[u],q[i].r=st[v];
            else q[i].l=en[u],q[i].r=st[v];
            q[i].idx=i;
        }
        mos_algo();
        for(i=0;i<que;i++) cout<<sol[i]<<nl;
        for(i=0;i<=n;i++){
            level[i]=0;
            vis[i]=0,cnt[i]=0,g[i].clear();
            for(k=0;k<20;k++) par[i][k]=0;
        }
    }
    return 0;
}

