#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, mod = 1e9 + 7;

struct Mat {
	int n, m;
	vector< vector<int> > a;
	Mat() { }
	Mat(int _n, int _m) {n = _n; m = _m; a.assign(n, vector<int>(m, 0)); }
	Mat(vector< vector<int> > v) { n = v.size(); m = n ? v[0].size() : 0; a = v; }
	inline void make_unit() {
	    assert(n == m);
	    for (int i = 0; i < n; i++)  {
	    	for (int j = 0; j < n; j++) a[i][j] = i == j;
	    }
	}
	inline Mat operator + (const Mat &b) {
        assert(n == b.n && m == b.m);
        Mat ans = Mat(n, m);
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {
             	ans.a[i][j] = (a[i][j] + b.a[i][j]) % mod;
            }
        }
        return ans;
	}	
	inline Mat operator - (const Mat &b) {
        assert(n == b.n && m == b.m);
        Mat ans = Mat(n, m);
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {
             	ans.a[i][j] = (a[i][j] - b.a[i][j] + mod) % mod;
            }
        }
        return ans;
	}
	inline Mat operator * (const Mat &b) {
		assert(m == b.n);
		Mat ans = Mat(n, b.m);
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < m; j++) {
				for(int k = 0; k < b.n; k++) {
					ans.a[i][j] = (ans.a[i][j] + 1LL * a[i][k] * b.a[k][j] % mod) % mod;
				}
			}
		}
		return ans;
	}
	inline Mat pow(long long k) {
		assert(n == m);
		Mat ans(n, n), t = a; ans.make_unit();
		while (k) {
			if (k & 1) ans = ans * t;
			t = t * t;
			k >>= 1;
		}
		return ans;
	}
	inline Mat& operator += (const Mat& b) { return *this = (*this) + b; }
    inline Mat& operator -= (const Mat& b) { return *this = (*this) - b; }
    inline Mat& operator *= (const Mat& b) { return *this = (*this) * b; }
    inline bool operator == (const Mat& b) { return a == b.a; }
    inline bool operator != (const Mat& b) { return a != b.a; }
};

int32_t main() {
    int t, cs = 0; cin >> t;
    while (t--) {
        int f0, f1, n; cin >> f0 >> f1 >> n;
        vector<vector<int>> a(2, vector<int>(2, 0));
        a[0][0] = 1; a[0][1] = 1; a[1][0] = 1;
        Mat res = Mat(a).pow(n - 1);
        long long ans=(1LL * res.a[0][0] * f1 % mod + 1LL * res.a[0][1] * f0 % mod) % mod;
        printf("Case %d: %lld\n", ++cs, ans);
    }
    return 0;
}
