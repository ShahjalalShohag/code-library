#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;

#define pii pair<int,int>
#define pll pair<ll,ll>
#define eb emplace_back
#define ll long long
#define nl '\n'
#define deb(x) cerr<<#x" = "<<x<<nl
#define in() ( { int a ; scanf("%d",&a); a; } )

const int N = 3e3 + 9;
const int mod = 1e9 + 7;


vector<int> d[N];
int cnt[N], ans[N];
int32_t main() {
  for(int i = 1; i < N; i++) for(int j = i; j < N; j += i) d[j].eb(i);
  int n = 72;
  vector<int> di = d[n];
  int s = di.size();
  map<int, int> id;
  for(int i = 0; i < s; i++) id[di[i]] = i;
  for(int i = 0; i < s; i++) ans[i] = rand(), cnt[i] = ans[i];
  for(int i = 0; i < s; i++) cout << ans[i] << ' ';
  cout << nl;
  vector<int> P = {2, 3};
  for(auto k : P) {
    for(int i = 0; i < s; i++) {
      if(di[i] % k == 0) {
        ans[id[di[i] / k]] -= ans[i];
        //assert(id[di[i]/x.F]<i);
      }
    }
  }
  for(int i = s - 1; i >= 0; i--) {
    for(int j = i + 1; j < s; j++) if(di[j] % di[i] == 0) cnt[i] -= cnt[j];
  }
  for(int i = 0; i < s; i++) cout << ans[i] << ' ';
  cout << nl;
  for(int i = 0; i < s; i++) cout << cnt[i] << ' ';
  cout << nl;
  return 0;
}
