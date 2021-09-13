#include<bits/stdc++.h>
using namespace std;

const int N = 105;
const double eps = 1e-9;

double determinant(vector<vector<double>>a) {
  int n = a.size();
  double det = 1;
  for (int i = 0; i < n; ++i) {
    int k = i;
    for (int j = i + 1; j < n; ++j)
      if (abs (a[j][i]) > abs (a[k][i]))
        k = j;
    if (abs (a[k][i]) < eps) {
      det = 0;
      break;
    }
    swap (a[i], a[k]);
    if (i != k)
      det = -det;
    det *= a[i][i];
    for (int j = i + 1; j < n; ++j)
      a[i][j] /= a[i][i];
    for (int j = 0; j < n; ++j)
      if (j != i && abs (a[j][i]) > eps)
        for (int k = i + 1; k < n; ++k)
          a[j][k] -= a[i][k] * a[j][i];
  }

  return det;
}
//count number of arborescences i.e. directed rooted trees in the graph
int g[N][N], in[N];
int32_t main() {
  int n;
  while(cin >> n && n) {
    memset(g, 0, sizeof g);
    memset(in, 0, sizeof in);
    for(int i = 0; i < n; i++) {
      string s;
      cin >> s;
      for(int j = 0; j < n; j++) if(s[j] == '1') g[i][j]++, in[j]++;
    }
    for(int i = 0; i < n; i++) {
      for(int j = 0; j < n; j++) {
        if(i == j) g[i][j] = in[i];
        else g[i][j] *= -1;
      }
    }
    int ans = 0;
    for(int r = 0; r < n; r++) {
      //number of arborescences with root r is the  determinant of the matrix produced by deleting the r-th row and column from g.
      vector< vector<double> > mat;
      for(int i = 0; i < n; i++) {
        if(i == r) continue;
        vector<double> v;
        for(int j = 0; j < n; j++) {
          if(j != r) v.push_back(g[i][j]);
        }
        mat.push_back(v);
      }
      double nw = determinant(mat);
      ans += (int) round(nw);
    }
    cout << ans << '\n';
  }
  return 0;
}
//https://www.spoj.com/problems/DAGCNT/

