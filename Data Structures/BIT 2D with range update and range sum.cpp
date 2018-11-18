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
#define fast ios_base::sync_with_stdio(false),cin.tie(NULL)
#define valid(tx,ty) (tx>=0&&tx<n&&ty>=0&&ty<m)
#define dist(x1,y1,x2,y2) sqrt(SQ(x1-x2)+SQ(y1-y2))
#define one(x) __builtin_popcount(x)
#define Unique(v) v.erase(unique(all(v)),v.end())
#define stree ll l=(n<<1),r=l+1,mid=b+(e-b)/2
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
const int N=1010;
const ld eps=1e-9;
//ll gcd(ll a,ll b){while(b){ll x=a%b;a=b;b=x;}return a;}
//ll lcm(ll a,ll b){return a/gcd(a,b)*b;}
//ll qpow(ll n,ll k) {ll ans=1;assert(k>=0);while(k>0){if(k&1) ans=(ans*n)%mod;n=(n*n)%mod;k>>=1;}return ans;}
ll multree[N][N][2],addtree[N][N][2];
ll yo(ll x)
{
    ///for range sum
    return x;
    ///for range xor
    ///return (x%2);
}
ll query2(ll tree[N][N][2],ll x,ll y)
{
    ll mul=0,add=0;
    for(ll i=y;i>0;i-=i&-i){
        mul+=tree[x][i][0];
        add+=tree[x][i][1];
        ///mul^=tree[x][i][0];
        ///add^=tree[x][i][1];
    }
    return (mul*yo(y))+add;
    ///return (mul*yo(y))^add;
}
ll query1(ll x,ll y)
{
    ll mul=0,add=0;
    for(ll i=x;i>0;i-=i&-i){
        mul+=query2(multree,i,y);
        add+=query2(addtree,i,y);
        ///mul^=query2(multree,i,y);
        ///add^=query2(addtree,i,y)
    }
    return (mul*yo(x))+add;
    ///return (mul*yo(x))^add;
}
ll query(ll x1,ll y1,ll x2,ll y2)
{
    return (query1(x2,y2)-query1(x1-1,y2)-query1(x2,y1-1)+query1(x1-1,y1-1));
    ///return (query1(x2,y2)^query1(x1-1,y2)^query1(x2,y1-1)^query1(x1-1,y1-1));
}
void upd2(ll tree[N][N][2],ll x,ll y,ll mul,ll add)
{
    for(ll i=x;i<N;i+=i&-i){
        for(ll j=y;j<N;j+=j&-j){
            tree[i][j][0]+=mul;
            tree[i][j][1]+=add;
            ///tree[i][j][0]^=mul;
            ///tree[i][j][1]^=add;
        }
    }
}
void upd1(ll x,ll y1,ll y2,ll mul,ll add)
{
    ///for range sum
    upd2(multree,x,y1,mul,-mul*yo(y1-1));
    upd2(multree,x,y2,-mul,mul*yo(y2));
    upd2(addtree,x,y1,add,-add*yo(y1-1));
    upd2(addtree,x,y2,-add,add*yo(y2));
    ///for range xor
    ///upd2(multree,x,y1,mul,mul*yo(y1-1));
    ///upd2(multree,x,y2,mul,mul*yo(y2));
    ///upd2(addtree,x,y1,add,add*yo(y1-1));
    ///upd2(addtree,x,y2,add,add*yo(y2));
}
void upd(ll x1,ll y1,ll x2,ll y2,ll val)
{
    ///for range sum
    upd1(x1,y1,y2,val,-val*yo(x1-1));
    upd1(x2,y1,y2,-val,val*yo(x2));
    ///for range xor
    ///upd1(x1,y1,y2,val,val*yo(x1-1));
    ///upd1(x2,y1,y2,val,val*yo(x2));
}
int main()
{
    fast;
    ll i,j,k,n,m,tt,x1,y1,x2,y2,q,val;
    cin>>n>>m;
    for(i=1;i<=n;i++){
        for(j=1;j<=m;j++){
            cin>>k;
            upd(i,j,i,j,k);
        }
    }
    cin>>q;
    while(q--){
        cin>>tt;
        if(tt==1){
            cin>>x1>>y1>>x2>>y2>>val;
            /// add val from top-left(x1,y1) to bottom-right (x2,y2);
            upd(x1,y1,x2,y2,val);
        }
        else{
            cin>>x1>>y1>>x2>>y2;
            /// output sum from top-left(x1,y1) to bottom-right (x2,y2);
            cout<<query(x1,y1,x2,y2)<<nl;
        }
    }
    return 0;
}

