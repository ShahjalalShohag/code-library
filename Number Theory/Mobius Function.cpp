#include<bits/stdc++.h>
using namespace std;

const int N = 5e5 + 9;

int mob[N];
void mobius() {
  mob[1] = 1;
  for (int i = 2; i < N; i++){
    mob[i]--;
    for (int j = i + i; j < N; j += i) {
      mob[j] -= mob[i];
    }
  }
}
bool vis[N];
vector<int> d[N];
int mul[N];
void add(int x, int k) {
  for (auto y: d[x]) {
    mul[y] += k;
  }
}
int query(int x) {
  int ans = 0;
  for (auto y: d[x]) {
    ans += mul[y] * mob[y];
  }
  return ans;
}
int a[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  mobius();
  for (int i = 1; i < N; i++) {
    if (mob[i]) {
      for (int j = i; j < N; j += i) {
        d[j].push_back(i);
      }
    }
  }
  int n, q; cin >> n >> q;
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }
  long long ans = 0;
  while (q--) {
    int i; cin >> i;
    if (vis[i]) {
      ans -= query(a[i]);
      ans += a[i] == 1;
      add(a[i], -1);
    }
    else {
      ans += query(a[i]);
      add(a[i], 1);
    }
    vis[i] ^= 1;
    cout << ans << '\n';
  }
  return 0;
}
// https://codeforces.com/contest/547/problem/C