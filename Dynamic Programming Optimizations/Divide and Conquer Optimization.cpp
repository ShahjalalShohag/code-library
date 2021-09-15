#include<bits/stdc++.h>
using namespace std;

#define ll long long
const int N = 4010;

int sc() {
  int c = getchar();
  int x = 0;
  int neg = 0;
  for(; ((c < 48 || c > 57) && c != '-'); c = getchar());
  if(c == '-') {
    neg = 1;
    c = getchar();
  }
  for(; c > 47 && c < 58; c = getchar()) {
    x = (x << 1) + (x << 3) + c - 48;
  }
  if(neg) x = -x;
  return x;
}

// Divide 1,2,3...n people in k consecutive parts so that sum of cost of each individual part is minimum
int a[N][N], c[N][N], dp[810][N]; // dp[i][j]=minimum cost for dividing [1...j] into i parts
int cost(int i, int j) {
  if(i > j) return 0;
  return c[j][j] - c[i - 1][j] - c[j][i - 1] + c[i - 1][i - 1];
}
void yo(int i, int l, int r, int optl, int optr) {
  if(l > r) return;
  int mid = (l + r) / 2;
  dp[i][mid] = 2e9; // for maximum cost change it to 0
  int opt = -1;
  for(int k = optl; k <= min(mid, optr); k++) {
    int c = dp[i - 1][k] + cost(k + 1, mid);
    if(c < dp[i][mid]) { // for maximum cost just change < to > only and rest of the algo should not be changed
      dp[i][mid] = c;
      opt = k;
    }
  }
  // for opt[1..j]<=opt[1...j+1] i.e. cost(1,j)<=cost(1,j+1)
  yo(i, l, mid - 1, optl, opt);
  yo(i, mid + 1, r, opt, optr);
  // for opt[1...j]>=opt[1...j+1] i.e. cost(1,j)>=cost(1,j+1)
  // yo(i,l,mid-1,opt,optr);
  // yo(i,mid+1,r,optl,opt);

}
int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  int i, j, k, n, m;
  n = sc();
  k = sc();
  for(i = 1; i <= n; i++) for(j = 1; j <= n; j++) a[i][j] = sc();
  for(i = 1; i <= n; i++) {
    for(j = 1; j <= n; j++) {
      c[i][j] = a[i][j] + c[i - 1][j] + c[i][j - 1] - c[i - 1][j - 1];
    }
  }
  for(i = 1; i <= n; i++) dp[1][i] = cost(1, i);
  for(i = 2; i <= k; i++) yo(i, 1, n, 1, n);
  cout << dp[k][n] / 2 << endl;
  return 0;
}
// https://codeforces.com/contest/321/problem/E
