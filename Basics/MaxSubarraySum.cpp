#include <bits/stdc++.h>
using namespace std;

int main(){
  int n; cin >> n;
    vector<int> v(n);
    for(auto&i:v) cin >> i;
 
    int ans = v[0], sum = 0;
 
    for(int i = 0 ; i < n ; i++){
        sum += v[i];
        ans = max(ans, sum);
        sum = max(sum, 0LL); 
        }
  cout << ans << endl;

}
