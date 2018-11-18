#pragma comment(linker, "/stack:200000000")
#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
#pragma GCC optimize("unroll-loops")

#include<bits/stdc++.h>
//#include<ext/pb_ds/detail/rb_tree_map_/rb_tree_.hpp>
//#include<ext/pb_ds/tree_policy.hpp>
//using namespace __gnu_pbds;
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
#define vpii vector< pair<int,int> >
#define vpll vector< pair<ll,ll> >
#define ppll pair< ll,pll >
#define pllp pair< pll,ll >
#define stll stack<ll>
#define qll queue<ll>
#define pqll priority_queue<ll>
#define mppll map<pll,ll>
#define mpii map<int,int>
#define mpll map<ll,ll>
#define mpss map<string,string>
#define mpsll map<string,ll>
#define mplls map<ll,string>
#define umpii unordered_map<int,int>
#define umll unordered_map<ll,ll>
#define umpss unordered_map<string,string>
#define umpsll unordered_map<string,ll>
#define umplls unordered_map<ll,string>
#define umap unordered_map
#define uset unordered_set
#define PQ priority_queue

#define rep(i,n) for(i=0;i<n;i++)
#define itfor(i, c) for (typeof((c).begin()) i = (c).begin(); i != (c).end(); i++)
#define printa(a,L,R) for(int i=L;i<R;i++) cout<<a[i]<<(i==R-1?’\n’:’ ‘)
#define printv(a) printa(a,0,a.size())
#define print2d(a,r,c) for(int i=0;i<r;i++) for(int j=0;j<c;j++) cout<<a[i][j]<<(j==c-1?’\n’:’ ‘)
#define pb push_back
#define MP make_pair
#define UB upper_bound
#define LB lower_bound
#define SQ(x) ((x)*(x))
#define issq(x) (((ll)(sqrt((x))))*((ll)(sqrt((x))))==(x))
#define F first
#define S second
#define mem(a,x) memset(a,x,sizeof(a))
#define inf 0x3f3f3f3f
#define PI 3.14159265358979323846
#define E 2.71828182845904523536
#define gamma 0.5772156649
#define nl "\n"
#define lg(r,n) (int)(log2(n)/log2(r))
#define sf(a) scanf("%d",&a)
#define sfl(a) scanf("%lld",&a)
#define sfc(a) scanf("%c",&a)
#define sff(a,b) scanf("%d %d",&a,&b)
#define sffl(a,b) scanf("%lld %lld",&a,&b)
#define sfff(a,b,c) scanf("%d %d %d",&a,&b,&c)
#define sfffl(a,b,c) scanf("%lld %lld %lld",&a,&b,&c)
#define pf printf
#define pfi(a) pf("%d\n",a)
#define pfl(a) pf("%lld\n",a)
#define _ccase printf("Case %lld: ",++cs)
#define _case cout<<"Case "<<++cs<<": "
#define debug(x) cout<<#x"="<<(x)<<nl

#define rev(v) reverse(v.begin(),v.end())
#define srt(v) sort(v.begin(),v.end())
#define grtsrt(v) sort(v.begin(),v.end(),greater<int>())
#define all(v) v.begin(),v.end()
#define mnv(v) *min_element(v.begin(),v.end())
#define mxv(v) *max_element(v.begin(),v.end())
#define countv(v,a) count(v.begin(),v.end(),a)
#define toint(a) atoi(a.c_str())
#define fast ios_base::sync_with_stdio(false),cin.tie(NULL)
string tostr(int n) {stringstream rr;rr<<n;return rr.str();}
//template <typename T> using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

const int mod=1e9+7;
const int mxn=2e5+9;
const ld eps=1e-9;
//ll qpow(ll n,ll k) {ll ans=1;while(k>0){if(k&1) ans=(ans*n)%mod;n=(n*n)%mod;k>>=1;}return ans;}
struct sj
{
    int l,r,idx;
}q[mxn];
int block,a[mxn],curl,curr,cnt[mxn*5];
ll ans,sol[mxn];
bool cmp(sj a,sj b)
{
    if(a.l/block==b.l/block) return a.r<b.r;
    else return a.l/block<b.l/block;
}
void add(ll i)
{
    ans+=(ll)(2*cnt[a[i]]+1)*a[i];
    cnt[a[i]]++;
}
void remov(ll i)
{
    ans-=(ll)(2*cnt[a[i]]-1)*a[i];
    cnt[a[i]]--;
}
int main()
{
    fast;
    int i,j,k,n,m,t;
    cin>>n>>t;
    for(i=1;i<=n;i++) cin>>a[i];
    for(i=0;i<t;i++){
        cin>>q[i].l>>q[i].r;
        q[i].idx=i;
    }
    block=(int)sqrt(n);
    sort(q,q+t,cmp);
    for(i=0;i<t;i++){
        while(curl<q[i].l) remov(curl++);
        while(curl>q[i].l) add(--curl);
        while(curr<q[i].r) add(++curr);
        while(curr>q[i].r) remov(curr--);
        sol[q[i].idx]=ans;
    }
    for(i=0;i<t;i++) cout<<sol[i]<<nl;
    return 0;
}

