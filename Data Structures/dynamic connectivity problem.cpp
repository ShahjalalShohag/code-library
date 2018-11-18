#pragma comment(linker, "/stack:200000000")
#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
#pragma GCC optimize("unroll-loops")

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
#define vpll vector<pll>
#define vpii vector<pii>
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
#define inf 1e18
#define E 2.71828182845904523536
#define gamma 0.5772156649
#define nl "\n"
#define lg(r,n) (int)(log2(n)/log2(r))
#define pf printf
#define sf scanf
#define sf1(a)                scanf("%d",&a)
#define sf2(a,b)              scanf("%d %d",&a,&b)
#define sf3(a,b,c)            scanf("%d %d %d",&a,&b,&c)
#define pf1(a)                printf("%d\n",a);
#define pf2(a,b)              printf("%d %d\n",a,b)
#define pf3(a,b,c)            printf("%d %d %d\n",a,b,c)
#define sf1ll(a)              scanf("%lld",&a)
#define sf2ll(a,b)            scanf("%I64d %I64d",&a,&b)
#define sf3ll(a,b,c)          scanf("%I64d %I64d %I64d",&a,&b,&c)
#define pf1ll(a)              printf("%lld\n",a);
#define pf2ll(a,b)            printf("%I64d %I64d\n",a,b)
#define pf3ll(a,b,c)          printf("%I64d %I64d %I64d\n",a,b,c)
#define _ccase printf("Case %lld: ",++cs)
#define _case cout<<"Case "<<++cs<<": "
#define by(x) [](const auto& a, const auto& b) { return a.x < b.x; }

#define asche cerr<<"Ekhane asche\n";
#define rev(v) reverse(v.begin(),v.end())
#define srt(v) sort(v.begin(),v.end())
#define grtsrt(v) sort(v.begin(),v.end(),greater<ll>())
#define all(v) v.begin(),v.end()
#define mnv(v) *min_element(v.begin(),v.end())
#define mxv(v) *max_element(v.begin(),v.end())
#define toint(a) atoi(a.c_str())
#define BeatMeScanf ios_base::sync_with_stdio(false)
#define valid(tx,ty) (tx>=0&&tx<n&&ty>=0&&ty<m)
#define one(x) __builtin_popcount(x)
#define Unique(v) v.erase(unique(all(v)),v.end())
#define stree l=(n<<1),r=l+1,mid=b+(e-b)/2
#define fout(x) fixed<<setprecision(x)
string tostr(int n) {stringstream rr;rr<<n;return rr.str();}
inline void yes(){cout<<"YES\n";exit(0);}
inline void no(){cout<<"NO\n";exit(0);}
template <typename T> using o_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
//ll dx[]={1,0,-1,0,1,-1,-1,1};
//ll dy[]={0,1,0,-1,1,1,-1,-1};
//random_device rd;
//mt19937 random(rd());
#define debug(args...) { string _s = #args; replace(_s.begin(), _s.end(), ',', ' '); stringstream _ss(_s); istream_iterator<string> _it(_ss); deb(_it, args); }
void deb(istream_iterator<string> it) {}
template<typename T, typename... Args>
void deb(istream_iterator<string> it, T a, Args... args) {
    cerr << *it << " = " << a << endl;
    deb(++it, args...);
}

const int mod=1e9+7;
const int N=3e5+9;
const ld eps=1e-9;
const ld PI=acos(-1.0);
//ll gcd(ll a,ll b){while(b){ll x=a%b;a=b;b=x;}return a;}
//ll lcm(ll a,ll b){return a/gcd(a,b)*b;}
//ll qpow(ll n,ll k) {ll ans=1;assert(k>=0);n%=mod;while(k>0){if(k&1) ans=(ans*n)%mod;n=(n*n)%mod;k>>=1;}return ans%mod;}


struct persistent_dsu
{
	struct state
	{
		int u, v, rnku, rnkv;
		state() {u = -1; v = -1; rnkv = -1; rnku = -1;}
		state(int _u, int _rnku, int _v, int _rnkv)
		{
			u = _u;
			rnku = _rnku;
			v = _v;
			rnkv = _rnkv;
		}
	};

	stack<state> st;
	int par[N], depth[N];
	int comp;
	persistent_dsu() {comp=0;memset(par, -1, sizeof(par)); memset(depth, 0, sizeof(depth));}

	int root(int x)
	{
		if(x == par[x]) return x;
		return root(par[x]);
	}

	void init(int n)
	{
	    comp=n;
		for(int i = 0; i <= n; i++)
		{
			par[i] = i;
			depth[i] = 1;
		}
	}

	bool connected(int x, int y)
	{
		return root(x) == root(y);
	}

	void unite(int x, int y)
	{
		int rx = root(x), ry = root(y);
		if(rx==ry){
            st.push(state());
            return;
		}
		if(depth[rx] < depth[ry])
			par[rx] = ry;
		else if(depth[ry] < depth[rx])
			par[ry] = rx;
		else
		{
			par[rx] = ry;
			depth[rx]++;
		}
		comp--;
		st.push(state(rx, depth[rx], ry, depth[ry]));
	}
	///how many last added edges you want to erase
	void backtrack(int c)
	{
		while(!st.empty() && c)
		{
		    if(st.top().u==-1){
                st.pop();
                c--;
                continue;
		    }
			par[st.top().u] = st.top().u;
			par[st.top().v] = st.top().v;
			depth[st.top().u] = st.top().rnku;
			depth[st.top().v] = st.top().rnkv;
			st.pop();
			c--;
			comp++;
		}
	}
};
persistent_dsu d;
vpii alive[4*N];
void upd(int n,int b,int e,int i,int j,pii &p)
{
    if(b>j||e<i) return;
    if(b>=i&&e<=j){
        alive[n].eb(p);///this edge was alive in this time range
        return;
    }
    int stree;
    upd(l,b,mid,i,j,p);
    upd(r,mid+1,e,i,j,p);
}
int ans[N];
void query(int n,int b,int e)
{
    if(b>e) return;
    int prevsz=d.st.size();
    ///add edges which were alive in this range
    for(auto p:alive[n]) d.unite(p.F,p.S);
    if(b==e){
        ans[b]=d.comp;
        d.backtrack(d.st.size()-prevsz);
        return;
    }
    int stree;
    query(l,b,mid);
    query(r,mid+1,e);
    d.backtrack(d.st.size()-prevsz);
}
struct HASH{
  size_t operator()(const pair<int,int>&x)const{
    return hash<long long>()(((long long)x.first)^(((long long)x.second)<<32));
  }
};
set<pii>se;
bool isquery[N];
map<pii,int>st;
int main()
{
    BeatMeScanf;
    //freopen("connect.in", "r", stdin);
    //freopen("connect.out", "w", stdout);
    int i,j,k,n,m,u,v;
    cin>>n>>m;
    d.init(n);
    for(i=1;i<=m;i++){
        string ty;
        cin>>ty;
        if(ty=="?"){
            isquery[i]=1;
        }
        else if(ty=="+"){
            cin>>u>>v;
            if(u>v) swap(u,v);
            pii p={u,v};
            se.insert(p);
            st[p]=i;
        }
        else{
            cin>>u>>v;
            if(u>v) swap(u,v);
            pii p={u,v};
            se.erase(p);
            upd(1,1,m,st[p],i-1,p);///in this time range this edge was in the DSU
        }
    }
    for(auto p:se) upd(1,1,m,st[p],m,p);///update rest of the edges
    se.clear();
    query(1,1,m);
    for(i=1;i<=m;i++) if(isquery[i]) cout<<ans[i]<<nl;
    return 0;
}
