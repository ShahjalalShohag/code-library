#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

// returns the longest proper prefix array of pattern p
// where lps[i]=longest proper prefix which is also suffix of p[0...i]
vector<int> build_lps(string p) {
  int sz = p.size();
  vector<int> lps;
  lps.assign(sz + 1, 0);
  int j = 0;
  lps[0] = 0;
  for(int i = 1; i < sz; i++) {
    while(j >= 0 && p[i] != p[j]) {
      if(j >= 1) j = lps[j - 1];
      else j = -1;
    }
    j++;
    lps[i] = j;
  }
  return lps;
}
vector<int>ans;
// returns matches in vector ans in 0-indexed
void kmp(vector<int> lps, string s, string p) {
  int psz = p.size(), sz = s.size();
  int j = 0;
  for(int i = 0; i < sz; i++) {
    while(j >= 0 && p[j] != s[i])
      if(j >= 1) j = lps[j - 1];
      else j = -1;
    j++;
    if(j == psz) {
      j = lps[j - 1];
      // pattern found in string s at position i-psz+1
      ans.push_back(i - psz + 1);
    }
    // after each loop we have j=longest common suffix of s[0..i] which is also prefix of p
  }
}

int main() {
  int i, j, k, n, m, t;
  cin >> t;
  while(t--) {
    string s, p;
    cin >> s >> p;
    vector<int>lps = build_lps(p);
    kmp(lps, s, p);
    if(ans.empty()) cout << "Not Found\n";
    else {
      cout << ans.size() << endl;
      for(auto x : ans) cout << x << ' ';
      cout << endl;
    }
    ans.clear();
    cout << endl;
  }
  return 0;
}
