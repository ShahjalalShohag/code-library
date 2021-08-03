#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

//O(n^3 logn)
int Gauss(vector<vector<int>> a, const int mod) {
  int n = (int)a.size(); if (n != a[0].size()) return 0;
  int det = 1;
  for(int col = 0, row = 0; col < n && row < n; ++col) {
    int mx = row;
    for(int i = row; i < n; i++) if(a[i][col] > a[mx][col]) mx = i;
    if(a[mx][col] == 0) return 0;
    for(int i = col; i < n; i++) swap(a[row][i], a[mx][i]);
    if (row != mx) det = det == 0 ? 0: mod - det;
    for(int i = row + 1; i < n; i++) {
      while (a[row][col]) {
        int t = a[i][col] / a[row][col];
        for (int j = col; j < n; j++) {
          a[i][j] -= 1LL * a[row][j] * t % mod;
          if (a[i][j] < 0) a[i][j] += mod;
          swap(a[i][j], a[row][j]);
        }
        det = det == 0 ? 0: mod - det;
      }
      for (int j = col; j < n; j++) swap(a[row][j], a[i][j]);
      det = det == 0 ? 0: mod - det;            
    }
    det = 1LL * det * a[row][col] % mod;
    ++row;
  }
  return det;
}
int32_t main() {
  int n, mod;
  while (cin >> n >> mod) {
    vector<vector<int>> a(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        int k; cin >> k;
        k %= mod; if (k < 0) k += mod;
        a[i][j] = k;
      }
    }
    cout << Gauss(a, mod) << '\n';
  }
  return 0;
}
//https://www.spoj.com/problems/DETER3/en/
