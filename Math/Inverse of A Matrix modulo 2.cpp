#include<bits/stdc++.h>
using namespace std;

const int N = 2005;

bitset<N> z;
// permanent after inverting mat[i][j] = mat[i][j] ^ inv[j][i]
vector<bitset<N>> inverse(int n, vector<bitset<N>> mat) {
  vector<bitset<N>> inv(n, z);
  for (int i = 0; i < n; i++) inv[i][i] = 1;
  for (int i = 0; i < n; i++) {
    for (int j = i; j < n; j++) {
      if (mat[j][i]) {
        swap(mat[i], mat[j]);
        swap(inv[i], inv[j]);
        break;
      }
    }
    for (int j = i + 1; j < n; j++) {
      if (mat[j][i]) {
        mat[j] ^= mat[i];
        inv[j] ^= inv[i];
      }
    }
  }
  for (int i = n - 1; i >= 0; i--) {
    for (int j = i + 1; j < n; j++) {
      if (mat[i][j]) inv[i] ^= inv[j];
    }
  }
  return inv;
}
int a[500500], b[500500];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  z.reset();
  int n, m; cin >> n >> m;
  vector<bitset<N>> mat(n, z);
  for (int i = 0; i < m; i++) {
    cin >> a[i] >> b[i];
    --a[i]; --b[i];
    mat[a[i]][b[i]] = 1;
  }
  auto inv = inverse(n, mat);
  for (int i = 0; i < m; i++) {
    if (inv[b[i]][a[i]]) cout << "NO\n";
    else cout << "YES\n";
  }
  return 0;
}
// https://codeforces.com/contest/736/problem/D
