#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, mod = 998244353;

// s' $ s defines all subsets of s
namespace SOS {
    const int B = 20; // Every input vector must need to be of size 1<<B
    // z(f(s))=\sum_{s' $ s}{f(s')}
    // O(B * 2 ^ B)
    // zeta transform is actually SOS DP
    vector<int> zeta_transform(vector<int> f) {
        for (int i = 0; i < B; i++) {
            for (int mask = 0; mask < (1 << B); mask++) {
                if ((mask & (1 << i)) != 0) {
                    f[mask] += f[mask ^ (1 << i)];// you can change the operator from + to min/gcd to find min/gcd of all f[submasks]
                }
            }
        }
        return f;
    }
    // mu(f(s))=\sum_{s' $ s}{(-1)^|s\s'| * f(s')}
    // O(B * 2 ^ B)
    vector<int> mobius_transform(vector<int> f) {
        for (int i = 0; i < B; i++) {
            for (int mask = 0; mask < (1 << B); mask++) {
                if ((mask & (1 << i)) != 0) {
                    f[mask] -= f[mask ^ (1 << i)];
                }
            }
        }
        return f;
    }
    vector<int> inverse_zeta_transform(vector<int> f) {
        return mobius_transform(f);
    }
    vector<int> inverse_mobius_transform(vector<int> f) {
        return zeta_transform(f);
    }
    // z(f(s))=\sum_{s' is supermask of s}{f(s')}
    // O(B * 2 ^ B)
    // zeta transform is actually SOS DP
    vector<int> zeta_transform_for_supermasks(vector<int> f) {
        for (int i = 0; i < B; i++) {
            for (int mask=(1<<B)-1 ; mask>=0 ; mask--) {
                if ((mask&(1<<i)) == 0) f[mask] += f[mask^(1<<i)] ;
            }
        }
        return f;
    }
    // f*g(s)=sum_{s' $ s} {f(s')*g(s\s')}
    // O(B * B * 2 ^ B)
    vector<int> subset_sum_convolution(vector<int> f, vector<int> g) {
        vector< vector<int> > fhat(B+1, vector<int> (1<<B, 0));
        vector< vector<int> > ghat(B+1, vector<int> (1<<B, 0));
        // Make fhat[][] = {0} and ghat[][] = {0}
        for (int mask = 0; mask < (1 << B); mask++) {
            fhat[__builtin_popcount(mask)][mask] = f[mask];
            ghat[__builtin_popcount(mask)][mask] = g[mask];
        }
        // Apply zeta transform on fhat[][] and ghat[][]
        for (int i = 0; i <= B; i++) {
            for (int j = 0; j <= B; j++) {
                for (int mask = 0; mask < (1 << B); mask++) {
                    if ((mask & (1 << j)) != 0) {
                        fhat[i][mask] += fhat[i][mask ^ (1 << j)];
                        if (fhat[i][mask] >= mod) fhat[i][mask] -= mod;
                        ghat[i][mask] += ghat[i][mask ^ (1 << j)];
                        if (ghat[i][mask] >= mod) ghat[i][mask] -= mod;
                    }
                }
            }
        }
        vector< vector<int> > h(B+1, vector<int> (1<<B, 0));
        // Do the convolution and store into h[][] = {0}
        for (int mask = 0; mask < (1 << B); mask++) {
            for (int i = 0; i <= B; i++) {
                for (int j = 0; j <= i; j++) {
                    h[i][mask] += 1LL * fhat[j][mask] * ghat[i - j][mask] % mod;
                    if (h[i][mask] >= mod) h[i][mask] -= mod;
                }
            }
        }
        // Apply inverse SOS dp on h[][]
        for (int i = 0; i <= B; i++) {
            for (int j = 0; j <= B; j++) {
                for (int mask = 0; mask < (1 << B); mask++) {
                    if ((mask & (1 << j)) != 0) {
                        h[i][mask] -= h[i][mask ^ (1 << j)];
                        if (h[i][mask] < 0) h[i][mask] += mod;
                    }
                }
            }
        }
        vector<int> fog(1 << B, 0);
        for (int mask = 0; mask < (1 << B); mask++)  fog[mask] = h[__builtin_popcount(mask)][mask];
        return fog;
    }
};

int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n; cin >> n;
    vector<int> a(1 << 20, 0), b(1 << 20, 0);
    for (int i = 0; i < (1 << n); i++) cin >> a[i];
    for (int i = 0; i < (1 << n); i++) cin >> b[i];
    auto ans = SOS::subset_sum_convolution(a, b);
    for (int i = 0; i < (1 << n); i++) cout << ans[i] << ' '; cout << '\n';
    return 0;
}
