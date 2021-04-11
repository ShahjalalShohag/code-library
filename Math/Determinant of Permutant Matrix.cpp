#include <bits/stdc++.h>
using namespace std; 

constexpr int P = 1000000007;
using Poly = vector<int>;
int power(int a, int b) {
    int res = 1;
    for (; b; b >>= 1, a = 1ll * a * a % P)
        if (b & 1)
            res = 1ll * res * a % P;
    return res;
}
Poly operator%(Poly a, const Poly &b) {
    assert(!b.empty());
    int m = b.size() - 1, n = a.size() - 1;
    int inv = power(b[m], P - 2);
    while (n >= m) {
        int x = 1ll * inv * (P - a[n]) % P;
        for (int j = 0; j < m; ++j)
            a[n - m + j] = (a[n - m + j] + 1ll * x * b[j]) % P;
        a.pop_back();
        while (!a.empty() && a.back() == 0)
            a.pop_back();
        n = a.size() - 1;
    }
    return a;
}
// Let n = deg(A), m = deg(B). Treat deg(0) = 0.
// a[n] != 0, b[m] != 0
// R(A, B) := b[m]^n prod_{j=0..m-1} A(b_roots[j]) // counted with multiplicities
// = (-1)^(mn) a[n]^m prod_{i=0..n-1} B(a_roots[i])
// O(n * m)
int resultant(const Poly &a, const Poly &b) {
    if (b.empty())
        return 0;
    if (b.size() == 1)
        return power(b[0], a.size() - 1);
    auto c = a % b;
    return 1ll * power(b.back(), a.size() - c.size()) * (((a.size() & 1) | (b.size() & 1)) ? 1 : P - 1) % P * resultant(b, c) % P;
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<int> a(n), p(n); // for cyclic matrix set p accordingly
    for (int i = 0; i < n; ++i)
        cin >> a[i];
    for (int i = 0; i < n; ++i) {
        cin >> p[i];
        --p[i];
    }
    for (int i = 0, x = 0; i < n - 1; ++i) {
        x = p[x];
        if (x == 0) { // if permutation has more than one cycle than ans is 0
            cout << 0 << "\n";
            return 0;
        }
    }
    vector<int> q(n);
    for (int i = n - 1, x = 0; i >= 0; --i) {
        q[i] = x;
        x = p[x];
    }
    vector<int> vis(n);
    int coef = (n & 1) ? P - 1 : 1;
    for (int i = 0; i < n; ++i) {
        if (vis[i])
            continue;
        for (int j = i; !vis[j]; j = q[j])
            vis[j] = 1;
        coef = P - coef;
    }
    vector<int> b(n);
    for (int i = 0; i < n; ++i)
        b[i] = a[q[i]];
    a.assign(n + 1, 0);
    a[0] = P - 1;
    a[n] = 1;
    int ans = resultant(a, b);
    ans = 1ll * ans * coef % P;
    cout << ans << "\n";
    return 0;
}
// https://codeforces.com/gym/102129/problem/G
