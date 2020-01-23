#include<bits/stdc++.h>
using namespace std;

#define pii pair<int,int>
#define pll pair<ll,ll>
#define eb emplace_back
#define ll long long
#define nl '\n'
#define deb(x) cerr<<#x" = "<<x<<nl
#define in() ( { int a ; scanf("%d",&a); a; } )

const int N = 1e6 + 9;
const int mod = 1e9 + 7;

struct aho_corasick_static
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
	aho_corasick_static() { sz = N - 2; clear(); }
	void add_word(string &s)
	{
		int u = 0;
		for(char c: s)
		{
			if(!to[u].count(c)) to[u][c] = sz++;
			u = to[u][c];
		}
		cnt[u]++;
		is_end[u] = 1;
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
				cnt[v]+=cnt[link[v]];
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
	int get_count(string &s)
	{
	    int ans=0, cur=0;
        for(int i=0; s[i]; i++){
            cur=go(cur, s[i]);
            ans+=cnt[cur];
        }
        return ans;
	}
};
struct aho_corasick
{
	vector<string> li[20];
	aho_corasick_static ac[20];
	void clear()
	{
		for(int i = 0; i < 20; i++)
		{
			li[i].clear();
			ac[i].clear();
		}
	}
	aho_corasick() { clear(); }
	void add_word(string s)
	{
		int pos = 0;
		for(int l = 0; l < 20; l++)if(li[l].empty()){
				pos = l;
				break;
			}
		li[pos].push_back(s);
		ac[pos].add_word(s);
		for(int bef = 0; bef < pos; bef++)
		{
			ac[bef].clear();
			for(string s2: li[bef])
			{
				li[pos].push_back(s2);
				ac[pos].add_word(s2);
			}
			li[bef].clear();
		}
		ac[pos].push_links();
	}
	///sum of occurrences of all patterns in this string
	int get_count(string s)
	{
		int ans = 0;
		for(int l = 0; l < 20; l++){
			ans += ac[l].get_count(s);
		}
		return ans;
	}
};

string s;
aho_corasick aho;

int main()
{
    ios_base::sync_with_stdio(0); cin.tie(0);
    int i,j,k,n,m;
    cin >> n;
	while(n--){
        cin>>m>>s;
        if(m==0) aho.add_word(s);
        else{
            if(aho.get_count(s)) cout<<"YES\n";
            else cout<<"NO\n";
        }
	}
    return 0;
}

