#include<bits/stdc++.h>
using namespace std;

const int mod = 1e9 + 7;
//for Q assign operation it takes Qlogn time in total
template<class T>
struct interval_set {
  map<pair<int, int>, T> value;//{r,l}=val

  void init(int n) {
    value[ {n, 1}] = (T)0;    //initial value
  }
  //assign a[i]=val for l<=i<=r
  //returns affected ranges before performing this assign operation
  vector<pair<pair<int, int>, T> > assign(int l, int r, T val) {
    auto bg = value.lower_bound({l, 0})->first;
    if(bg.second != l) {
      T val = value[bg];
      value.erase(bg);
      value[ {l - 1, bg.second}] = val;
      value[ {bg.first, l}] = val;
    }

    auto en = value.lower_bound({r, 0})->first;
    if(en.first != r) {
      T val = value[en];
      value.erase(en);
      value[ {en.first, r + 1}] = val;
      value[ {r, en.second}] = val;
    }

    vector<pair<pair<int, int>, T> > ret;
    auto itt = value.lower_bound({l, 0});
    while(true) {
      if(itt == value.end() || itt->first.first > r) break;
      ret.push_back({{itt->first.second, itt->first.first}, itt->second});
      ++itt;
    }

    for(auto it : ret)
      value.erase({it.first.second, it.first.first});

    value[ {r, l}] = val;
    return ret;
  }
};
interval_set<int>se;
//assign a value in range in each query
//in the end print the sum of the array elements
int32_t main() {
  int i, j, k, n, m, q, l, r, v;
  cin >> n >> q;
  se.init(n);
  while(q--) {
    cin >> l >> r >> v;
    se.assign(l, r, v);
  }
  int ans = 0;
  for(auto x : se.value) {
    ans += 1LL * ((x.first.first - x.first.second + 1) % mod) * (x.second % mod) % mod;
    ans %= mod;
  }
  cout << ans << endl;
  return 0;
}
