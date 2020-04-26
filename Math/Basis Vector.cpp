#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

struct Basis {
	vector<int> a;
	void insert(int x) {
	    for (auto &i: a) x = min(x, x ^ i);
	    if (!x) return;
	    for (auto &i: a) if((i ^ x) < i) i ^= x;
	    a.push_back(x);
	    sort(a.begin(), a.end());
	}
	bool can(int x) {
	    for(auto &i: a) x = min(x, x ^ i);
	    return !x;
	}
	int maxxor(int x = 0) {
	    for (auto &i: a) x = max(x, x ^ i);
		return x;
	}	
	int minxor(int x = 0) {
	    for (auto &i: a) x = min(x, x ^ i);
		return x;
	}
	int kth(int k) { // 1st is 0
	    int sz = (int)a.size();
	    if (k > (1 << sz)) return -1;
	    k--; int ans = 0;
	    for(int i = 0; i < sz; i++) if(k >> i & 1) ans ^= a[i];
	    return ans;
	}
}t;
int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);

    int q; cin >> q;
    while (q--) {
    	int ty, k; cin >> ty >> k;
    	if (ty == 1) t.insert(k);
    	else cout << t.kth(k) << '\n';
    }
    return 0;
}
//https://codeforces.com/group/qcIqFPYhVr/contest/203881/problem/S