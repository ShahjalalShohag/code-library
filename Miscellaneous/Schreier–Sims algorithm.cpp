#include<bits/stdc++.h>
using namespace std;

using ll = long long;

// time complexity : O(n^2 log^3 |G| + t n log |G|)
// memory complexity : O(n^2 log |G| + tn)
// t : number of generators
// |G| : group size, obviously <= (n!)

vector<int> inv(vector<int>& p){
  vector<int> ret(p.size());
  for (int i = 0; i < p.size(); i++) ret[p[i]] = i;
  return ret;
}
vector<int> operator * (vector<int>& a, vector<int>& b ){
  vector<int> ret(a.size());
  for (int i = 0 ; i < a.size(); i++) ret[i] = b[a[i]];
  return ret;
}
// a group contains all subset products of generators
struct Group {
  int n, m;
  vector<vector<int>> lookup;
  vector<vector<vector<int>>> buckets, ibuckets;
  int yo(vector<int> p, bool add_to_group = 1){
    n = buckets.size();
    for (int i = 0; i < n ; i++){
      int res = lookup[i][p[i]];
      if (res == -1 ){
        if (add_to_group){
          buckets[i].push_back(p);
          ibuckets[i].push_back(inv(p));
          lookup[i][p[i]] = buckets[i].size() - 1;
        }
        return i;
      }
      p = p * ibuckets[i][res];
    }
    return -1;
  }
  ll size() {
    ll ret = 1;
    for (int i = 0; i < n; i++) ret *= buckets[i].size();
    return ret;
  }
  bool in_group(vector<int> g) { return yo(g, false) == -1; }
  Group(vector<vector<int>> &gen, int _n){
    n = _n, m = gen.size(); // m permutations of size n, 0 indexed
    lookup.resize(n);
    buckets.resize(n);
    ibuckets.resize(n);
    for (int i = 0; i < n ; i++){
      lookup[i].resize(n);
      fill(lookup[i].begin(), lookup[i].end(), -1);
    }
    vector<int> id(n);
    for (int i = 0; i < n ; i++) id[i] = i;
    for (int i = 0; i < n ; i++){
      buckets[i].push_back(id);
      ibuckets[i].push_back(id);
      lookup[i][i] = 0;
    }
    for (int i = 0; i < m ; i++) yo(gen[i]);
    queue<pair<pair<int, int>,pair<int, int>>> q;
    for (int i = 0; i < n ; i++) {
      for (int j = i; j < n ; j++) {
        for (int k = 0; k < buckets[i].size(); k++) {
          for (int l = 0; l < buckets[j].size(); l++) {
            q.push({pair<int, int>(i, k), pair<int, int>(j, l)});
          }
        }
      }
    }
    while(!q.empty()) {
      pair<int, int> a = q.front().first;
      pair<int, int> b = q.front().second;
      q.pop();
      int res = yo(buckets[a.first][a.second] * buckets[b.first][b.second]);
      if (res == -1) continue;
      pair<int, int> cur(res, (int)buckets[res].size() - 1);
      for (int i = 0; i < n; i ++) {
        for (int j = 0; j < (int)buckets[i].size(); ++j){
          if (i <= res) q.push(make_pair(pair<int, int>(i , j), cur));
          if (res <= i) q.push(make_pair(cur, pair<int, int>(i, j)));
        }
      }
    }
  }
};
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int k, n; cin >> k >> n;
  vector<vector<int>> a;
  while (k--) {
   vector<int> v;
   for  (int i = 0; i < n; i++) {
     int x; cin >> x;
     v.push_back(x - 1);
   }
   a.push_back(v);
  }
  Group g(a, n);
  cout << g.size() << '\n';
  return 0;
}
// https://codeforces.com/blog/entry/21335?locale=ru#comment-260437
// http://opencup.ru/files/ocg/gp5/problems1-e.pdf
// problem H