#include<bits/stdc++.h>
using namespace std;

const int N = 1e6 + 9;

int ans[N], aux[N];

// Generates cyclic string of length k^n that contains every length n string as substring. alphabet = [0, k - 1]
// O(k^n)
int DeBruijin(int k, int n) { //returns length = k^n
	if(k == 1) {
		ans[0] = 0;
		return 1;
	}
	for(int i = 0; i < k * n; i++) aux[i] = 0;
	int sz = 0;
	function<void(int, int)> yo = [&](int t, int p) {
		if(t > n) {
			if(n % p == 0) {
				for(int i = 1; i <= p; i++) ans[sz++] = aux[i];
			}
		}
		else {
			aux[t] = aux[t - p];
			yo(t + 1, p);
			for(int i = aux[t - p] + 1; i < k; i++) {
				aux[t] = i;
				yo(t + 1, t);
			}
		}
	};
	yo(1, 1);
	return sz;
}
int32_t main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	int k, n; cin >> k >> n;
	int len = DeBruijin(k, n);
	for (int i = 0; i < len; i++) cout << ans[i] << ' '; cout << '\n';
    return 0;
}