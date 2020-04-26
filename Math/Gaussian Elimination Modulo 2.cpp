#include<bits/stdc++.h>
using namespace std;

const int N = 1010;

int Gauss(vector<bitset<N>> a, bitset<N> &ans) {
   int n = a.size(), m = (int)a[0].size() - 1;
   int rank = 0, det = 1;
   vector<int> pos(N, -1);
   for(int col = 0, row = 0; col < m && row < n; ++col) {
      int k = row;
      for(int i = row; i < n; ++i) if(a[i][col]) { k = i; break; }
      if(!a[k][col]) { det = 0; continue; }
      swap(a[k], a[row]);
      if(row != k) det = det == 0 ? 0 : 1;
      det &= a[row][col];
      pos[col] = row;
      for(int i = 0; i < n; ++i) if (i != row && a[i][col] > 0) a[i] ^= a[row];
      ++row, ++rank;
   }
   for(int i = 0; i < m; ++i) ans[i] = (pos[i] == -1) ? 0 : a[pos[i]][m];
   for(int i = rank; i < n; ++i) if(a[i][m]) return -1; //no solution
   int free_var = 0;
   for(int i = 0; i < m; ++i) if(pos[i] == -1) free_var++;
   return free_var; //has solution
}
int32_t main() {
    
    return 0;
}