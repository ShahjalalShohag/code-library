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


vi decimal_to_factoradic(int n)
{
    vi v;
    int i=1;
    while(n){
        v.eb(n%i);
        n/=i;
        i++;
    }
    rev(v);
    return v;
}
int factoradic_to_decimal(vi &v)
{
    int n=v.size();
    int ans=0;
    for(int i=0,mul=n;i<n;i++,mul--) ans=(ans*mul%mod+v[i])%mod;
    return ans;
}
vi permutation(int n,vi &v)
{
    o_set<int>se;
    int sz=v.size();
    vi p;
    for(int i=0;i<n-sz;i++) p.eb(i);
    for(int i=n-sz;i<n;i++) se.insert(i);
    for(int i=0;i<sz;i++){
        int nw=*se.fbo(v[i]);
        p.eb(nw);
        se.erase(nw);
    }
    return p;
}
///returns k-th lexicographically smallest permutation of size n
///0-th permutation is the unit permutation i.e. 0,1,2,....n-1
vi kth_perm(int n,int k)
{
    ///need to return something when k>=n!
    vi v=decimal_to_factoradic(k);
    return permutation(n,v);
}
vi factoradic_order(vi &p)
{
    o_set<int>se;
    int n=p.size();
    for(int i=0;i<n;i++) se.insert(p[i]);
    vi fac;
    for(int i=0;i<n;i++){
        int x=se.ook(p[i]);
        fac.eb(x);
        se.erase(p[i]);
    }
    return fac;
}
///?-th lexicographically smallest permutation of size n
int order(vi &p)
{
    vi fac=factoradic_order(p);
    return factoradic_to_decimal(fac);
}



///Given two permutations of size n, find Perm((ord(P)+ord(Q))mod n!)
///where Perm(k) is k-th lexicographically smallest permutation
///and ord(P) is the number k of the permutation
int main()
{
    BeatMeScanf;
    int i,j,k,n,m;
    cin>>n;
    vi p(n);
    for(i=0;i<n;i++) cin>>p[i];
    vi q(n);
    for(i=0;i<n;i++) cin>>q[i];
    vi ordp=factoradic_order(p);
    vi ordq=factoradic_order(q);
    vi sum=ordp;
    int carry=0;
    for(i=n-1;i>=0;i--){
        sum[i]+=ordq[i]+carry;
        carry=sum[i]/(n-i);
        sum[i]%=(n-i);
    }
    vi perm=permutation(n,sum);
    for(i=0;i<n;i++) cout<<perm[i]<<' ';
    return 0;
}

