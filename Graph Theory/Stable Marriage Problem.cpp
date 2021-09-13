#include<bits/stdc++.h>
using namespace std;

const int N = 1010;

/*Given n men and n women, where each person has ranked all members of the opposite sex
in order of preference, marry the men and women together such that
there are no two people of opposite sex who would both rather have
each other than their current partners. When there are no such pairs of people,
the set of marriages is deemed stable. There is always a solution
m[][],w[][] lists partners in order of their preferences
it always yields the one that is best for all men among all stable matchings, and worst for all women. complexity O(n^2)*/

int m[N][N], w[N][N], id[N], marry[N], ans[N], pref[N][N];
void SM(int n) {
  queue<int>q;
  for(int i = 1; i <= n; i++) {
    q.push(i);
    id[i] = 1;
  }
  memset(marry, -1, sizeof marry);
  for(int i = 1; i <= n; i++) {
    for(int j = 1; j <= n; j++) pref[i][m[i][j]] = j;
  }
  while(!q.empty()) {
    int cw = q.front();
    q.pop();
    int cm = w[cw][id[cw]];
    if(marry[cm] != -1) {
      if(pref[cm][marry[cm]] > pref[cm][cw]) {
        id[marry[cm]]++;
        q.push(marry[cm]);
        marry[cm] = cw;
      } else {
        id[cw]++;
        q.push(cw);
      }
    } else marry[cm] = cw;
  }
  for(int i = 1; i <= n; i++) ans[i] = marry[i];
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  int t;
  cin >> t;
  while(t--) {
    int n;
    cin >> n;
    for(int i = 1; i <= n; i++) {
      int k;
      cin >> k;
      for(int j = 1; j <= n; j++) cin >> w[i][j];
    }
    for(int i = 1; i <= n; i++) {
      int k;
      cin >> k;
      for(int j = 1; j <= n; j++) cin >> m[i][j];
    }
    SM(n);
    for(int i = 1; i <= n; i++) cout << i << ' ' << ans[i] << '\n';
  }
  return 0;
}
