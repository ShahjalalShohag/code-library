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

#define debug(args...) { string _s = #args; replace(_s.begin(), _s.end(), ',', ' '); stringstream _ss(_s); istream_iterator<string> _it(_ss); deb(_it, args); }
void deb(istream_iterator<string> it) {}
template<typename T, typename... Args>
void deb(istream_iterator<string> it, T a, Args... args) {
    cerr << *it << " = " << a << endl;
    deb(++it, args...);
}

const int mod=1e9+7;
const int N=1e5+9;
const ld eps=1e-9;
const ld PI=acos(-1.0);
//ll gcd(ll a,ll b){while(b){ll x=a%b;a=b;b=x;}return a;}
//ll lcm(ll a,ll b){return a/gcd(a,b)*b;}
//ll qpow(ll n,ll k) {ll ans=1;assert(k>=0);n%=mod;while(k>0){if(k&1) ans=(ans*n)%mod;n=(n*n)%mod;k>>=1;}return ans%mod;}
int block;
struct query
{
    int l,r;///query range
    int updcnt;///update happened before this query
    int idx;///query index
    bool operator<(const query& x)const
    {
        if(l/block==x.l/block){
            if(r/block==x.r/block) return updcnt<x.updcnt;
            else return r/block<x.r/block;
        }
        else return l/block<x.l/block;
    }
}q[N];
struct Upd
{
    int idx;///update index
    int prv;///array had value prv before this update
    int nxt;///array will have value nxt after this update
}upd[N];
int curl,curr,a[N],cnt[10*N],val[N];
ll ans;
umap<int,int>mp;
/// When adding a number, we first nullify it's effect on current
/// answer, then update corresponding array, then account for it's effect again.
inline void add(int x)
{
    cnt[x]++;
    if(cnt[x]==1) ans+=val[x];
}
///removing is quite same as adding
inline void remov(int x)
{
    cnt[x]--;
    if(cnt[x]==0) ans-=val[x];
}
inline void do_upd(int upd_idx)
{
    int i=upd[upd_idx].idx;
    int now=upd[upd_idx].nxt;
    ///if the update is within our current range then re-correct our ans
    if(i>=curl&&i<=curr) remov(a[i]);
    a[i]=now;
    if(i>=curl&&i<=curr) add(a[i]);
}
inline void undo_upd(int upd_idx)
{
    int i=upd[upd_idx].idx;
    int now=upd[upd_idx].prv;
    ///if the update is within our current range then re-correct our ans
    if(i>=curl&&i<=curr) remov(a[i]);
    a[i]=now;
    if(i>=curl&&i<=curr) add(a[i]);
}
int aux[N];
ll res[N];
int main()
{
   // BeatMeScanf;
    int i,j,k,n,m,U=0,Q=0,que,l,r,t,z=0;
    char ch;
    sf1(n);
    for(i=1;i<=n;i++){
        sf1(a[i]);
        if(!mp.count(a[i])) mp[a[i]]=++z,val[z]=a[i];///compressing
        a[i]=mp[a[i]];
        aux[i]=a[i];

    }
    sf1(que);
    for(i=0;i<que;i++){
        sf(" %c",&ch);
        sf2(l,r);
        if(ch=='U'){
            ++U;
            upd[U].idx=l;
            if(!mp.count(r)) mp[r]=++z,val[z]=r;///compressing
            upd[U].nxt=mp[r];
            upd[U].prv=aux[l];
            aux[l]=mp[r];
        }
        else{
            q[Q].l=l;
            q[Q].r=r;
            q[Q].updcnt=U;
            q[Q].idx=i;
            Q++;
        }
    }
    block=(int)(cbrt(n*1.0)+eps);
    block*=block;
    sort(q,q+Q);
    int cur=0;///tracks the number of updates already happened
    mem(res,-1);
    for(i=0;i<Q;i++){
        l=q[i].l,r=q[i].r,t=q[i].updcnt;
        while(cur<t){
            cur++;
            do_upd(cur);
        }
        while(cur>t){
            undo_upd(cur);
            cur--;
        }
        while(curl<l) remov(a[curl++]);
        while(curl>l) add(a[--curl]);
        while(curr<r) add(a[++curr]);
        while(curr>r) remov(a[curr--]);
        res[q[i].idx]=ans;
    }
    for(i=0;i<que;i++) if(res[i]!=-1) pf1ll(res[i]);
    return 0;
}


