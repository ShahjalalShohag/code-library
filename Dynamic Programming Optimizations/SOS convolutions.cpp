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
    for (int mask = (1 << B) - 1 ; mask >= 0 ; mask--) {
      if ((mask & (1 << i)) == 0) f[mask] += f[mask ^ (1 << i)] ;
    }
  }
  return f;
}
// f*g(s)=sum_{s' $ s} {F(s')*G(s\s')}
// O(B * B * 2 ^ B)
vector<int> subset_sum_convolution(vector<int> F, vector<int> G) {
  vector<vector<int>> f(B + 1, vector<int> (1 << B, 0));
  vector<vector<int>> g(B + 1, vector<int> (1 << B, 0));

  for (int mask = 0; mask < (1 << B); mask++) {
    f[__builtin_popcount(mask)][mask] = F[mask];
    g[__builtin_popcount(mask)][mask] = G[mask];
  }

  for (int k = 0; k <= B; k++) {
    // do SOS -> f[k][mask] will be = number of submasks of mask such that |submask| = k 
    // same for g[k][mask]
    for (int i = 0; i <= B; i++) {
      for (int mask = 0; mask < (1 << B); mask++) {
        if (mask >> i & 1) {
          f[k][mask] += f[k][mask ^ (1 << i)];
          if (f[k][mask] >= mod) f[k][mask] -= mod;
          g[k][mask] += g[k][mask ^ (1 << i)];
          if (g[k][mask] >= mod) g[k][mask] -= mod;
        }
      }
    }
  }

  vector<vector<int>> h(B + 1, vector<int> (1 << B, 0));
  // h[k][mask] = number of pairs (u, v) such that u belongs to F
  // and v belongs to G and (u | v) is a submask of mask and |u| + |v| = k
  for (int mask = 0; mask < (1 << B); mask++) {
    for (int k = 0; k <= B; k++) {
      for (int i = 0; i <= k; i++) {
        h[k][mask] += 1LL * f[i][mask] * g[k - i][mask] % mod;
        if (h[k][mask] >= mod) h[k][mask] -= mod;
      }
    }
  }

  for (int k = 0; k <= B; k++) {
    // do inverse SOS -> h[k][mask] will be = same as above but now (u | v) = mask
    for (int i = 0; i <= B; i++) {
      for (int mask = 0; mask < (1 << B); mask++) {
        if (mask >> i & 1) {
          h[k][mask] -= h[k][mask ^ (1 << i)];
          if (h[k][mask] < 0) h[k][mask] += mod;
        }
      }
    }
  }

  // and, when u | v = mask and |u| + |v| = |mask|, that means u & v = 0 and we are done!
  vector<int> ans(1 << B, 0);
  for (int mask = 0; mask < (1 << B); mask++)  ans[mask] = h[__builtin_popcount(mask)][mask];
  return ans;
}
};

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n;
  cin >> n;
  vector<int> a(1 << 20, 0), b(1 << 20, 0);
  for (int i = 0; i < (1 << n); i++) cin >> a[i];
  for (int i = 0; i < (1 << n); i++) cin >> b[i];
  auto ans = SOS::subset_sum_convolution(a, b);
  for (int i = 0; i < (1 << n); i++) cout << ans[i] << ' ';
  cout << '\n';
  return 0;
}
