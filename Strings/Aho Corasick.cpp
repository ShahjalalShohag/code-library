#include<bits/stdc++.h>
using namespace std;

#define pii pair<int,int>
#define pll pair<ll,ll>
#define eb emplace_back
#define ll long long
#define nl '\n'
#define deb(x) cerr<<#x" = "<<x<<nl
#define in() ( { int a ; scanf("%d",&a); a; } )

const int N = 3e5 + 9;
const int mod = 1e9 + 7;

struct aho_corasick
{
	bool is_end[N];
	int link[N]; ///link to the longest proper suffix
    int sz;
	map<char, int> to[N], dp[N];
    int cnt[N], path[N], ind[N], len; ///for number of occurrences
	void clear()
	{
		for(int i = 0; i < sz; i++)
			is_end[i] = 0,cnt[i]=0,path[i]=0, ind[i]=0,link[i] = 0, to[i].clear(), dp[i].clear();
		sz = 1;
		is_end[0] = 1;
		len=0;
	}
	aho_corasick() { sz = N - 2; clear(); }
	void add_word(string &s, int idx)
	{
		int u = 0;
		for(char c: s)
		{
			if(!to[u].count(c)) to[u][c] = sz++;
			u = to[u][c];
		}
		is_end[u] = 1;
		ind[idx]=u;
	}
	void push_links()
	{
		queue<int> q;
		int u, v, j;
		char c;
		q.push(0);///root
		link[0] = -1;
		while(!q.empty())
		{
			u = q.front();
			q.pop();
			for(auto it: to[u])
			{
				v = it.second;
				c = it.first;
				j = link[u];
				while(j != -1 && !to[j].count(c)) j = link[j];
				if(j != -1) link[v] = to[j][c];
				else link[v] = 0;
				q.push(v);
				path[len++]=v;
			}
		}
	}
	///go to the state if we add the character ch
	int go(int v, char ch) {
        if (!dp[v].count(ch)) {
            if (to[v].count(ch)) return dp[v][ch]= to[v][ch];
            return dp[v][ch] = (v == 0 ? 0 : go(link[v], ch));
        }
        return dp[v][ch];
    }
	void traverse(string &s)
    {
        int n = s.size();
        int cur = 0;
        for(int i=0; i<n; i++){
            cur = go(cur, s[i]);
            cnt[cur]++;
        }
        for(int i=len-1; i>=0; i--) cnt[link[path[i]]] += cnt[path[i]];
    }
}t;

string p[N];
int32_t main()
{
	int i,j,k,n,q;
	string s; cin>>s;
	cin>>q;
    for(i=0;i<q;i++){
        cin>>p[i];
        t.add_word(p[i],i);
    }
    t.push_links();
    t.traverse(s);
    ///print the number of occurrences
    for(i=0;i<q;i++) cout<<t.cnt[t.ind[i]]<<nl;
	return 0;
}
