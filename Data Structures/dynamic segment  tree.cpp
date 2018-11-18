//#pragma comment(linker, "/stack:200000000")
//#pragma GCC optimize("Ofast")
//#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
//#pragma GCC optimize("unroll-loops")

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
//template <typename T> using o_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
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
const int N=1e5+9;
const ld eps=1e-9;
const ld PI=acos(-1.0);
//ll gcd(ll a,ll b){while(b){ll x=a%b;a=b;b=x;}return a;}
//ll lcm(ll a,ll b){return a/gcd(a,b)*b;}
//ll qpow(ll n,ll k) {ll ans=1;assert(k>=0);n%=mod;while(k>0){if(k&1) ans=(ans*n)%mod;n=(n*n)%mod;k>>=1;}return ans%mod;}

struct DynamicSegTree {
    int sum, add, tl, tr, l, r;
    DynamicSegTree () : sum (0), add (0), l (-1), r (-1) {}
};

int cnt = 2;
DynamicSegTree t[64 * N];

void push (int v) {
    if (t[v].add) {
        t[v].sum = t[v].tr - t[v].tl + 1;
        int tm = (t[v].tl + t[v].tr) >> 1;
        if (t[v].l == -1) {
            t[v].l = cnt++;
            t[t[v].l].tl = t[v].tl;
            t[t[v].l].tr = tm;
        }
        if (t[v].r == -1) {
            t[v].r = cnt++;
            t[t[v].r].tl = tm + 1;
            t[t[v].r].tr = t[v].tr;
        }
        t[t[v].l].add = t[t[v].r].add = 1;
        t[v].add = 0;
    }
}

void update (int v, int l, int r) {
    push (v);
    if (l == t[v].tl && r == t[v].tr) {
        t[v].add = 1;
        push (v);
    } else {
        int tm = (t[v].tl + t[v].tr) >> 1;
        if (t[v].l == -1) {
            t[v].l = cnt++;
            t[t[v].l].tl = t[v].tl;
            t[t[v].l].tr = tm;
        }
        if (t[v].r == -1) {
            t[v].r = cnt++;
            t[t[v].r].tl = tm + 1;
            t[t[v].r].tr = t[v].tr;
        }
        if (l > tm) {
            update (t[v].r, l, r);
        } else if (r <= tm) {
            update (t[v].l, l, r);
        } else {
            update (t[v].l, l, tm);
            update (t[v].r, tm + 1, r);
        }
        push (t[v].l);
        push (t[v].r);
        t[v].sum = t[t[v].l].sum + t[t[v].r].sum;
    }
}

int get (int v, int l, int r) {
    push (v);
    if (l == t[v].tl && r == t[v].tr) {
        return t[v].sum;
    } else {
        int tm = (t[v].tl + t[v].tr) >> 1;
        if (t[v].l == -1) {
            t[v].l = cnt++;
            t[t[v].l].tl = t[v].tl;
            t[t[v].l].tr = tm;
        }
        if (t[v].r == -1) {
            t[v].r = cnt++;
            t[t[v].r].tl = tm + 1;
            t[t[v].r].tr = t[v].tr;
        }
        if (l > tm) {
            return get (t[v].r, l, r);
        } else if (r <= tm) {
            return get (t[v].l, l, r);
        } else {
            return get (t[v].l, l, tm) + get (t[v].r, tm + 1, r);
        }
    }
}

int main()
{
    int q;
    scanf ("%d", &q);
    t[1].sum = 0; t[1].add = 0;
    t[1].tl = 1; t[1].tr = 1e9;
    int c = 0;
    while (q--) {
        int d, x, y;
        scanf ("%d %d %d", &d, &x, &y);
        if (d == 2) {
            printf ("%d\n", get (1, x, y));
        } else {
            update (1, x , y );
        }
    }
    return 0;
}
