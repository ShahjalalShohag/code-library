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
#define stree l=(node<<1),r=l+1,mid=b+(e-b)/2
#define fout(x) fixed<<setprecision(x)
string tostr(int n) {stringstream rr;rr<<n;return rr.str();}
inline void yes(){cout<<"YES\n";exit(0);}
inline void no(){cout<<"NO\n";exit(0);}
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
int sc()
{
    register int n=0;
    char ch=getchar();
    for(;(ch<48||ch>57);ch=getchar())
        ;
    for(;(ch>47&&ch<58);ch=getchar())
        n=(n<<3)+(n<<1)+ch-48;
    return n;
}

const int mod=1e9+7;
const int N=1030;
const ld eps=1e-9;
const ld PI=acos(-1.0);
//ll gcd(ll a,ll b){while(b){ll x=a%b;a=b;b=x;}return a;}
//ll lcm(ll a,ll b){return a/gcd(a,b)*b;}
//ll qpow(ll n,ll k) {ll ans=1;assert(k>=0);n%=mod;while(k>0){if(k&1) ans=(ans*n)%mod;n=(n*n)%mod;k>>=1;}return ans%mod;}

int n,m;
struct segtree
{
    int a[N*4];
    segtree()
    {
        for(int i=0;i<N*4;i++) a[i]=0;
    }
    void reset()
    {
        for(int i=0;i<N*4;i++) a[i]=0;
    }
    // update i-th column by val
    void upd(int node,int b,int e,int i,int val,vi &v)
    {
        v.pb(node);
        if(b==e)
        {
            a[node]=val;
            return;
        }
        int stree;
        if(i<=mid) upd(l,b,mid,i,val,v);
        else upd(r,mid+1,e,i,val,v);
        a[node]=a[l]+a[r];
    }
    //sum from column i to j
    int query(int node,int b,int e,int i,int j)
    {
        if(j<b||i>e) return 0;
        if(b>=i&&e<=j) return a[node];
        int stree;
        return query(l,b,mid,i,j)+query(r,mid+1,e,i,j);
    }
};
struct segtree2d
{
    segtree a[N*4];
    void reset()
    {
        for(int i=0;i<N*4;i++) a[i].reset();
    }
    vi v;
    //set a[i][j]=val
    void upd(int node,int b,int e,int i,int j,int val)
    {
        if(b==e)
        {
            v.clear();
            a[node].upd(1,1,m,j,val,v);
            return;
        }
        int stree;
        if(i>=b&&i<=mid) upd(l,b,mid,i,j,val);
        else upd(r,mid+1,e,i,j,val);
        for(auto x:v) a[node].a[x]=a[l].a[x]+a[r].a[x];
    }
    //return sum from top-left (i,y1) to bottom-right (j,y2)
    int query(int node,int b,int e,int i,int y1,int j,int y2)
    {
        if(j<b||i>e) return 0;
        if(b>=i&&e<=j) return a[node].query(1,1,m,y1,y2);
        int stree;
        return query(l,b,mid,i,y1,j,y2)+query(r,mid+1,e,i,y1,j,y2);
    }
};
segtree2d t;
int main()
{
   // fast;
    int i,j,k,x,x1,y1,x2,y2,typ,q;
//    cin>>n>>m;
//    for(i=1;i<=n;i++){
//        for(j=1;j<=m;j++){
//            cin>>k;
//            t.upd(1,1,n,i,j,k);
//        }
//    }
    int tt;
    tt=sc();
    while(tt--){
        n=sc();
        m=n;
        while(1){
            char s[5];
            sf("%s",&s);
            if(s[2]=='D') break;///end
            else if(s[2]=='T'){///set a[i][j] as k
                i=sc(),j=sc(),x=sc();
                i++,j++;
                t.upd(1,1,n,i,j,x);
            }
            else{

                x1=sc(),y1=sc(),x2=sc(),y2=sc();
                x1++,y1++,x2++,y2++;
                // make sure (x1,y1) is top-left and (x2,y2) is bottom-right
                pf("%d\n",t.query(1,1,n,x1,y1,x2,y2));///sum of the rectangle
            }
        }
        t.reset();
    }
    return 0;
}
