#include<bits/stdc++.h>
using namespace std;

const int LG = 20;
// an example static DS which takes multiples integers
// and in the end it processes those data once in >= O(n)
// then it can query each time fast enough
struct ds_static {
  ds_static() {}
  vector<int> v;
  void insert(int x) {
    v.push_back(x);
  }

  void process() {
    sort(v.begin(), v.end());
  }

  // how many numbers <= x
  int query(int x) {
    return upper_bound(v.begin(), v.end(), x) - v.begin();
  }

  void clear() {
    v.clear();
  }
};

// now we can make this static ds into a dynamic ds
// means, it can query anytime (not necessarily only at the end)
// we can do it by only adding another log factor
struct ds_dynamic {
  vector<int> li[LG];
  ds_static ds[LG];

  void clear() {
    for(int i = 0; i < LG; i++) {
      li[i].clear();
      ds[i].clear();
    }
  }

  ds_dynamic() {
    clear();
  }

  void insert(int x) {
    int pos = 0;
    for(int l = 0; l < LG; l++) {
      if(li[l].empty()) {
        pos = l;
        break;
      }
    }

    li[pos].push_back(x);
    ds[pos].insert(x);

    for(int bef = 0; bef < pos; bef++) {
      ds[bef].clear();
      for(int x2 : li[bef]) {
        li[pos].push_back(x2);
        ds[pos].insert(x2);
      }

      li[bef].clear();
    }

    ds[pos].process();
  }

  // how many numbers <= x
  int query(int x) {
    int ans = 0;
    for(int l = 0; l < LG; l++) {
      ans += ds[l].query(x);
    }
    return ans;
  }
};

ds_dynamic ds;

int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n = 100000;
  long long res_sum = 0;
  for (int i = 1; i <= n; i++) {
    int x = rand();
    ds.insert(x);
    res_sum += ds.query(rand());
  }
  cout << res_sum << '\n';
  return 0;
}
