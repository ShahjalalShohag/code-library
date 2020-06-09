#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;
/*
Sums of arithmetic progressions.
mod_sum(n, a, d, m) = \sum_{i = 0}^{n - 1}{(a + d * i) % m}.
floor_sum(n, a, d, m) = \sum_{i = 0}^{n - 1}{(a + d * i) / m}.
log(m), with a large constant.
*/
long long sumsq(long long n) {
    return n / 2 * ((n - 1) | 1);
}
long long floor_sum(long long a, long long d, long long m, long long n) {
    long long res = d / m * sumsq(n) + a / m * n;
    d %= m; a %= m;
    if (!d) return res;
    long long to = (n * d + a) / m;
    return res + (n - 1) * to - floor_sum(m - 1 - a, m, d, to);
}
long long mod_sum(long long a, long long d, long long m, long long n) {
    a = ((a % m) + m) % m;
    d = ((d % m) + m) % m;
    return n * a + d * sumsq(n) - m * floor_sum(a, d, m, n);
}
int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t; cin >> t;
    while (t--) {
    	int n, m, a, b; cin >> n >> m >> a >> b;
    	cout << floor_sum(b, a, m, n) << '\n';
    }
    return 0;
}
