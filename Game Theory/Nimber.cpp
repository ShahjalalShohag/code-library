#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

using T = unsigned long long;
T _power[64][64]; // nim product for two powers of 2
T power(int x, int y) {
    if (_power[x][y]) return _power[x][y];
    if (!(x & y)) return _power[x][y] = 1ULL << (x ^ y);
    int z = (x & y) & -(x & y);
    return _power[x][y] = power(x ^ z, y) ^ power((x ^ z) | (z - 1), (y ^ z) | (x & (z - 1)));
}
struct Nimber {
    T x;
    Nimber(T x = 0) : x(x) {}
    Nimber operator + (const Nimber& a) const {
        return Nimber(x ^ a.x);
    }
    Nimber operator * (const Nimber& a) const {
        T ans = 0;
        for(int i = 0; i < 64; i++)	if (x >> i & 1) {
            for(int j = 0; j < 64; j++)	if (a.x >> j & 1) {
               ans ^= power(i, j);
            }
        }
        return Nimber(ans);
    }
    Nimber inv() const {
        Nimber ans(1), a(x);
        T b = -2;
        while (b) {
            if (b & 1) ans = ans * a;
            a = a * a;
            b >>= 1;
        }
        return ans;
    }
};
int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t; cin >> t;
	while (t--) {
		T a, b; cin >> a >> b;
		cout << (Nimber(a) * Nimber(b)).x << '\n';
	}
    return 0;
}
// https://judge.yosupo.jp/problem/nim_product_64