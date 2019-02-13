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
#define E 2.71828182845904523536
#define gamma 0.5772156649
#define nl "\n"
#define lg(r,n) (int)(log2(n)/log2(r))
#define sz(v) (int)v.size()
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
ll dx[]={1,0,-1,0,1,-1,-1,1};
ll dy[]={0,1,0,-1,1,1,-1,-1};
//random_device rd;
//mt19937 rnd(rd());
#define debug(args...) { string _s = #args; replace(_s.begin(), _s.end(), ',', ' '); stringstream _ss(_s); istream_iterator<string> _it(_ss); deb(_it, args); }
void deb(istream_iterator<string> it) {}
template<typename T, typename... Args>
void deb(istream_iterator<string> it, T a, Args... args) {
    cerr << *it << " = " << a << endl;
    deb(++it, args...);
}

const int mod=998244353;
const int N=3e5+9;
const ld eps=1e-9;
const ld PI=acos(-1.0);
ll gcd(ll a,ll b){while(b){ll x=a%b;a=b;b=x;}return a;}
ll lcm(ll a,ll b){return a/gcd(a,b)*b;}
ll qpow(ll n,ll k) {ll ans=1;assert(k>=0);n%=mod;while(k>0){if(k&1) ans=(ans*n)%mod;n=(n*n)%mod;k>>=1;}return ans%mod;}



///returns sum of nCi,0<=i<=k in sqrt(n).log^2(n) (maybe)
#define maxn 800005
inline int ksm(int x,int k){int s=1;for(;k;k>>=1,x=1ll*x*x%mod)if(k&1)s=1ll*s*x%mod;return s;}
inline int del(int x){return x<0?x+mod:x;}
inline int add(int x){return x<mod?x:x-mod;}
///Don't use pragma before using this
namespace ntt
{
    struct num
    {
        double x,y;
        num() {x=y=0;}
        num(double x,double y):x(x),y(y){}
    };
    inline num operator+(num a,num b) {return num(a.x+b.x,a.y+b.y);}
    inline num operator-(num a,num b) {return num(a.x-b.x,a.y-b.y);}
    inline num operator*(num a,num b) {return num(a.x*b.x-a.y*b.y,a.x*b.y+a.y*b.x);}
    inline num conj(num a) {return num(a.x,-a.y);}

    int base=1;
    vector<num> roots={{0,0},{1,0}};
    vector<int> rev={0,1};
    const double PI=acosl(-1.0);

    void ensure_base(int nbase)
    {
        if(nbase<=base) return;
        rev.resize(1<<nbase);
        for(int i=0;i<(1<<nbase);i++)
            rev[i]=(rev[i>>1]>>1)+((i&1)<<(nbase-1));
        roots.resize(1<<nbase);
        while(base<nbase)
        {
            double angle=2*PI/(1<<(base+1));
            for(int i=1<<(base-1);i<(1<<base);i++)
            {
                roots[i<<1]=roots[i];
                double angle_i=angle*(2*i+1-(1<<base));
                roots[(i<<1)+1]=num(cos(angle_i),sin(angle_i));
            }
            base++;
        }
    }

    void fft(vector<num> &a,int n=-1)
    {
        if(n==-1) n=a.size();
        assert((n&(n-1))==0);
        int zeros=__builtin_ctz(n);
        ensure_base(zeros);
        int shift=base-zeros;
        for(int i=0;i<n;i++)
            if(i<(rev[i]>>shift))
                swap(a[i],a[rev[i]>>shift]);
        for(int k=1;k<n;k<<=1)
        {
            for(int i=0;i<n;i+=2*k)
            {
                for(int j=0;j<k;j++)
                {
                    num z=a[i+j+k]*roots[j+k];
                    a[i+j+k]=a[i+j]-z;
                    a[i+j]=a[i+j]+z;
                }
            }
        }
    }

    vector<num> fa,fb;

    vector<int> multiply(vector<int> &a, vector<int> &b)
    {
        int need=a.size()+b.size()-1;
        int nbase=0;
        while((1<<nbase)<need) nbase++;
        ensure_base(nbase);
        int sz=1<<nbase;
        if(sz>(int)fa.size()) fa.resize(sz);
        for(int i=0;i<sz;i++)
        {
            int x=(i<(int)a.size()?a[i]:0);
            int y=(i<(int)b.size()?b[i]:0);
            fa[i]=num(x,y);
        }
        fft(fa,sz);
        num r(0,-0.25/sz);
        for(int i=0;i<=(sz>>1);i++)
        {
            int j=(sz-i)&(sz-1);
            num z=(fa[j]*fa[j]-conj(fa[i]*fa[i]))*r;
            if(i!=j) fa[j]=(fa[i]*fa[i]-conj(fa[j]*fa[j]))*r;
            fa[i]=z;
        }
        fft(fa,sz);
        vector<int> res(need);
        for(int i=0;i<need;i++) res[i]=fa[i].x+0.5;
        return res;
    }

    vector<int> multiply(vector<int> &a,vector<int> &b,int m,int eq=0)
    {
        int need=a.size()+b.size()-1;
        int nbase=0;
        while((1<<nbase)<need) nbase++;
        ensure_base(nbase);
        int sz=1<<nbase;
        if(sz>(int)fa.size()) fa.resize(sz);
        for(int i=0;i<(int)a.size();i++)
        {
            int x=(a[i]%m+m)%m;
            fa[i]=num(x&((1<<15)-1),x>>15);
        }
        fill(fa.begin()+a.size(),fa.begin()+sz,num{0,0});
        fft(fa,sz);
        if(sz>(int)fb.size()) fb.resize(sz);
        if(eq) copy(fa.begin(),fa.begin()+sz,fb.begin());
        else
        {
            for(int i=0;i<(int)b.size();i++)
            {
                int x=(b[i]%m+m)%m;
                fb[i]=num(x&((1<<15)-1),x>>15);
            }
            fill(fb.begin()+b.size(),fb.begin()+sz,num{0,0});
            fft(fb,sz);
        }
        double ratio=0.25/sz;
        num r2(0,-1),r3(ratio,0),r4(0,-ratio),r5(0,1);
        for(int i=0;i<=(sz>>1);i++)
        {
            int j=(sz-i)&(sz-1);
            num a1=(fa[i]+conj(fa[j]));
            num a2=(fa[i]-conj(fa[j]))*r2;
            num b1=(fb[i]+conj(fb[j]))*r3;
            num b2=(fb[i]-conj(fb[j]))*r4;
            if(i!=j)
            {
                num c1=(fa[j]+conj(fa[i]));
                num c2=(fa[j]-conj(fa[i]))*r2;
                num d1=(fb[j]+conj(fb[i]))*r3;
                num d2=(fb[j]-conj(fb[i]))*r4;
                fa[i]=c1*d1+c2*d2*r5;
                fb[i]=c1*d2+c2*d1;
            }
            fa[j]=a1*b1+a2*b2*r5;
            fb[j]=a1*b2+a2*b1;
        }
        fft(fa,sz);fft(fb,sz);
        vector<int> res(need);
        for(int i=0;i<need;i++)
        {
            ll aa=fa[i].x+0.5;
            ll bb=fb[i].x+0.5;
            ll cc=fa[i].y+0.5;
            res[i]=(aa+((bb%m)<<15)+((cc%m)<<30))%m;
        }
        return res;
    }
    vector<int> square(vector<int> &a,int m)
    {
        return multiply(a,a,m,1);
    }
};
typedef vector<int> vec;
int n,v,rev,fac[maxn],inv[maxn],mul[maxn];
int gd[maxn],g2d[maxn],fd[maxn],f2d[maxn],dlt[maxn];
int A[maxn],B[maxn],C[maxn];
int pre_[maxn],*pre=pre_+1,suf[maxn];
inline void calc(int *h,int *out,int d,int k)
{
	int off=k-d-1;mul[0]=1;
	for(int i=1;i<=2*d+3;i++)
	{
		int s=(i+off)%mod;s=del(s);
		mul[i]=1ll*mul[i-1]*s%mod;
	}
	int len=1;for(;len<3*d+5;len<<=1);;
	vi a,b;
	for(int i=0;i<=d;i++)
	{
		A[i]=1ll*h[i]*inv[i]%mod*inv[d-i]%mod;
		if((d-i)&1)A[i]=mod-A[i];
		a.eb(A[i]);
	}
	for(int i=0;i<=2*d;i++)B[i]=ksm((i-d+k)%mod,mod-2),B[i]=del(B[i]),b.eb(B[i]);
	vi c=ntt::multiply(a,b,mod);
	while(sz(c)<len) c.eb(0);
	for(int i=0;i<len;i++) C[i]=c[i];
	for(int i=0;i<=d;i++)
	{
		out[i]=1ll*C[i+d]*mul[i+k-off]%mod*ksm(mul[i+k-d-off-1],mod-2)%mod;
		out[i]=del(out[i]);
	}
}
void solve(int x,int *G,int *F)
{
	if(x==0){G[0]=1;F[0]=0;return;}
	if(x&1)
	{
		solve(x-1,G,F);
		for(int i=0;i<x;i++)gd[i]=G[i];
		calc(gd,dlt,x-1,-1ll*(n+1)*rev%mod);
		for(int i=0;i<x;i++)G[i]=1ll*G[i]*(i*v+x)%mod;
		int p=1;
		for(int i=1;i<=x;i++)p=1ll*p*(v*x+i)%mod;G[x]=p;
		for(int i=0;i<x;i++)
		{
			if((x-1)&1)F[i]-=dlt[i],F[i]=del(F[i]);
			else F[i]+=dlt[i],F[i]=add(F[i]);
			F[i]=1ll*F[i]*(i*v+x)%mod;
		}
		suf[x+1]=1;for(int i=x;i>=0;i--)suf[i]=1ll*suf[i+1]*(v*x+i)%mod;
		pre[-1]=1;for(int i=0;i<=x;i++)pre[i]=1ll*pre[i-1]*(n-i-v*x)%mod,pre[i]=del(pre[i]);
		p=0;for(int i=0;i<x;i++)p=(p+1ll*suf[i+1]*pre[i-1])%mod;F[x]=p;
		return;
	}
	int d=x>>1;solve(d,G,F);
	for(int i=0;i<=d;i++)gd[i]=G[i];
	calc(gd,gd+d+1,d,d+1);
	calc(gd,g2d,2*d,1ll*d*rev%mod);
	calc(gd,dlt,2*d,-1ll*(n+1)*rev%mod);
	for(int i=0;i<=d;i++)fd[i]=F[i];
	calc(fd,fd+d+1,d,d+1);
	calc(fd,f2d,2*d,1ll*d*rev%mod);
	for(int i=0;i<=x;i++)
	{
		int s=1ll*dlt[i]*f2d[i]%mod;if(d&1)s=mod-s;
		F[i]=(1ll*g2d[i]*fd[i]+s)%mod;
	}
	for(int i=0;i<=x;i++)G[i]=1ll*gd[i]*g2d[i]%mod;
}
int m,F[maxn],G[maxn];
inline int get(int x,int y)
{
    n=x,m=y;
	v=sqrt(n+0.5);rev=ksm(v,mod-2);
	solve(v,G,F);
	int fac=1;for(int i=0;i<v;i++)fac=1ll*fac*G[i]%mod;
	for(int i=v*v+1;i<=n;i++)fac=1ll*fac*i%mod;
    for(int i=0;i<=v;i++)gd[i]=G[i];
	calc(gd,dlt,v,-1ll*(n+1)*rev%mod);
	pre[-1]=1;for(int i=0;i<=v;i++)pre[i]=1ll*pre[i-1]*dlt[i]%mod;
	suf[v]=1;for(int i=v*v+1;i<=n;i++)suf[v]=1ll*suf[v]*i%mod;
	for(int i=v-1;i>=0;i--)suf[i]=1ll*suf[i+1]*G[i]%mod;
	int ans=0,cr=0;
	for(int i=0;i<v&&(i+1)*v-1<=m;i++)
	{
		int t=1ll*suf[i+1]*pre[i-1]%mod*F[i]%mod;if((i*v)&1)t=mod-t;
		ans+=t;ans=add(ans);cr=i+1;
	}
	ans=1ll*ans*ksm(fac,mod-2)%mod;
	int C=fac,Mg=1;
	for(int i=0;i<cr;i++)Mg=1ll*Mg*G[i]%mod;
	int s=n-cr*v,cp=0;
    for(int i=0;i<v&&(i+1)*v<=s;i++)Mg=1ll*Mg*G[i]%mod,cp=i+1;
    for(int i=cp*v+1;i<=s;i++)Mg=1ll*Mg*i%mod;
	C=1ll*C*ksm(Mg,mod-2)%mod;
	for(int i=cr*v;i<=m;i++)
	{
		if(i!=cr*v)C=1ll*C*ksm(i,mod-2)%mod*(n-i+1)%mod;
		ans+=C;ans=add(ans);
	}
	for(int i=0;i<=v;i++)F[i]=G[i]=0;
	return ans;
}

int32_t main()
{
    BeatMeScanf;
    int i,j,k,m,t;
    fac[0]=1;for(int i=1;i<maxn;i++)fac[i]=1ll*fac[i-1]*i%mod;
	inv[maxn-1]=ksm(fac[maxn-1],mod-2);
	for(int i=maxn-2;i>=0;i--)inv[i]=1ll*inv[i+1]*(i+1)%mod;
    cin>>t;
	while(t--){
		cin>>n>>m;
		cout<<get(n,m)<<nl;
	}
    return 0;
}
///Before submit=>
///    *check for integer overflow,array bounds
///    *check for n=1

