#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

namespace DiscreteLogarithm {
	int totient(int n) {
	    int ans = n;
	    for (int i = 2; i * i <= n; i++) {
	        if (n % i == 0) {
	            while (n % i == 0) n /= i;
	            ans = ans / i * (i - 1);
	        }
	    }
	    if (n > 1) ans = ans / n * (n - 1);
	    return ans;
	}
	int powmod(int a, int b, int m) {
	    int res = 1;
	    while (b > 0) {
	        if (b & 1) res = 1LL * res * a % m;
	        a = 1LL * a * a % m;
	        b >>= 1;
	    }
	    return res;
	}
	// g is a primitive root modulo p if and only if for any integer a such that
	// gcd(a, p) = 1, there exists an integer k such that: g^k = a(mod p).
	// this code assumes p is prime
	int PrimitiveRoot(int p) {
	    vector<int> fact;
	    int phi = totient(p),  n = phi;
	    for (int i = 2; i * i <= n; ++i) {
	        if (n % i == 0) {
	            fact.push_back(i);
	            while (n % i == 0) n /= i;
	        }
	    }
	    if (n > 1) fact.push_back(n);
	    for (int res = 2; res <= p; ++res) {
	        bool ok = true;
	        for (size_t i = 0; i < fact.size() && ok; ++i)
	            ok &= powmod(res, phi / fact[i], p) != 1;
	        if (ok)  return res;
	    }
	    return -1;
	}
	// baby step - giant step
	// find minimum integer x such that a^x = b (mod m)
	// where a and m are co-prime
	int DiscreteLog(int a, int b, int m) {
		static const int inf = 2e9;
	    int n = (int) sqrt (m + .0) + 1;
	    int pw = 1;
	    for (int i = 0; i < n; ++i) pw = 1LL * pw * a % m;
	    unordered_map<int, int> vals;
	    for (int p = 1, cur = pw; p <= n; ++p) {
	        if (!vals[cur]) vals[cur] = p;
	        cur = 1LL * cur * pw % m;
	    }
	    int ans = inf;
	    for (int q = 0, cur = b; q <= n; ++q) {
	        if (vals.find(cur) != vals.end()) {
	            long long nw = 1LL * vals[cur] * n - q;
	            if (nw < ans) ans = nw;
	        }
	        cur = (1LL * cur * a) % m;
	    }
	    if (ans == inf) ans = -1;
	    return ans;
	}
	// returns (x, y) s.t. ax + by = gcd(a,b)
	pair<long long, long long> ExtendedEuclid(int a, int b) {
	    if(b == 0) return pair<long long, long long>(a >= 0? 1LL : -1LL, 0LL);
	    else {
	        pair<long long, long long> d = ExtendedEuclid(b, a % b);
	        return pair<long long, long long>(d.second, d.first - d.second * (a / b));
	    }
	}
	int inverse(int a, int n) { //a and n is coprime
	    pair<long long, long long> ret = ExtendedEuclid(a, n);
	    return ((ret.first % n) + n) % n;
	}
	// Discrete Log but a and m may not be co-prime
	int DiscreteLogNonCoprime(int a, int b, int m) {
	    if (m == 1) return 0;
	    if (b == 1) return 0;
	    if (__gcd(a, m) == 1) return DiscreteLog(a, b, m);
	    int g = __gcd(a, m);
	    if (b % g != 0)  return -1;
	    int p = inverse(a / g, m / g);
	    int nw = DiscreteLogNonCoprime(a, 1LL * b / g * p % (m / g), m / g);
	    if (nw == -1) return -1;
	    return nw + 1;
	}
	// returns any or all numbers x such that x ^ k = a (mod n)
	int DiscreteRoot(int k, int a, int m) {
	    if (a == 0) return 1;
	    int g = PrimitiveRoot(m);
	    int phi = totient(m);
	    // run baby step-giant step
	    int sq = (int) sqrt (m + .0) + 1;
	    vector<pair<int,int>> dec (sq);
	    for (int i = 1; i <= sq; ++i) dec[i - 1] = make_pair (powmod (g, 1LL * i * sq % phi * k % phi, m), i);
	    sort(dec.begin(), dec.end());
	    int any_ans = -1;
	    for (int i = 0; i < sq; ++i) {
	        int my = powmod (g, 1LL* i * k % phi, m) * 1LL * a % m;
	        auto  it = lower_bound (dec.begin(), dec.end(), make_pair (my, 0));
	        if (it != dec.end() && it->first == my) {
	            any_ans = it->second * sq - i;
	            break;
	        }
	    }
	    if (any_ans == -1) return -1; ///no solution
	    // for any answer
	    int delta = (m - 1) / __gcd (k, m - 1);
	    for (int cur = any_ans % delta; cur < m - 1; cur += delta) return powmod (g, cur, m);

	    //for all possible answers
	    //int delta = (m - 1) / __gcd(k, m - 1);
	    //vector<int> ans;
	    //for (int cur = any_ans % delta; cur < m-1; cur += delta) ans.push_back(powmod(g, cur, m));
	    //sort(ans.begin(), ans.end());
	    //return ans;
	}
};
using namespace DiscreteLogarithm;
int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t; cin >> t;
    while (t--) {
    	int a, b, m; cin >> a >> b >> m;
    	cout << DiscreteLogNonCoprime(a, b, m) << '\n';
    }
    return 0;
}
