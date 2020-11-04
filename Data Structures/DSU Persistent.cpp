#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9, B = 320;

struct PDSU
{
	int c, dep[N], par[N];
	stack<array<int, 4>> st;
	PDSU() {}
	void init(int n) {
		c = n;
		for(int i = 0; i <= n; i++) par[i] = i, dep[i] = 1;
        st = stack<array<int, 4>>();
	}
	int root(int x) { return x == par[x] ? x : root(par[x]); }
	bool same(int x, int y) { return root(x) == root(y); }
	int size(int x) { return dep[root(x)]; }
	int count() { return c; }
	void unite(int x, int y) {
		if((x = root(x)) == (y = root(y))) return;
		st.push({x, dep[x], y, dep[y]});
		if(dep[x] <= dep[y]) par[x] = y, dep[y] += dep[x];
		else par[y] = x, dep[x] += dep[y];
		c--;
	}
	void snapshot() { st.push({-1, -1, -1, -1}); }
	void rollback() {
		while(!st.empty()){
			if(st.top()[0] == -1) return;
			++c;
			for(int i = 0; i < 3; i += 2) par[st.top()[i]] = st.top()[i], dep[st.top()[i]] = st.top()[i + 1];
			st.pop();
		}
	}
};

PDSU d;

int u[N], v[N];
int c = 0;
bool ok(int l, int r)
{
    d.snapshot();
    for(int i = l; i <= r; i++){
        d.unite(u[i], v[i]);
        c++;
    }
    int ok = 0;
    if(d.c == 1) ok = 1;
    d.rollback();
    return ok;
}
int32_t main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n, m; cin >> n >> m;
    for(int i = 1; i <= m; i++) cin >> u[i] >> v[i];
    int r = 0;
    long long ans = 0;
    d.init(n);
    int en = B - 1;
    for(int l = 1; l <= m; l++){
        if(r < l - 1){
            r = l - 1;
            d.init(n);
        }
        if(l % B == 0){
            d.init(n);
            en = l + B - 1;
            for(int i = en + 1; i <= r; i++)  d.unite(u[i], v[i]), c++;
        }
        while(r + 1 <= m && !ok(l, min(r, en))){
            r++;
            if(r > en) d.unite(u[r], v[r]), c++;
        }
        if(ok(l, min(r, en))) ans += m - r + 1;
        else break;
    }
    cout << ans << '\n';
    return 0;
}
//https://www.hackerearth.com/practice/algorithms/graphs/depth-first-search/practice-problems/algorithm/connected-intervals-1/
