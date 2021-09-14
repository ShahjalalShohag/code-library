#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;
vector<int> v;
bitset<N>bs[26], oc;
int main() {
  int i, j, k, n, q, l, r;
  string s, p;
  cin >> s;
  for(i = 0; s[i]; i++) bs[s[i] - 'a'][i] = 1;
  cin >> q;
  while(q--) {
    cin >> p;
    oc.set();
    for(i = 0; p[i]; i++) oc &= (bs[p[i] - 'a'] >> i);
    cout << oc.count() << endl; // number of occurences
    int ans = N, sz = p.size();
    int pos = oc._Find_first();
    v.push_back(pos);
    pos = oc._Find_next(pos);
    while(pos < N) {
      v.push_back(pos);
      pos = oc._Find_next(pos);
    }
    for(auto x : v) cout << x << ' '; // position of occurences
    cout << endl;
    v.clear();
    cin >> l >> r; // number of occurences from l to r,where l and r is 1-indexed
    if(sz > r - l + 1) cout << 0 << endl;
    else cout << (oc >> (l - 1)).count() - (oc >> (r - sz + 1)).count() << endl;
  }
  return 0;
}
