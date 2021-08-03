#include<bits/stdc++.h>
using namespace std;

const int N = 2010;

//n = number of equations, m = number of variables
int Gauss(int n, int m, vector<bitset<N>> a, bitset<N> &ans) {
  //reversing for lexocgraphically largest solution
  for (int i = 0; i < n; i++) {
      bitset<N> tmp;
      for (int j = 0; j < m; j++) tmp[j] = a[i][m - j - 1];
      tmp[m] = a[i][m];
      a[i] = tmp;
  }
  int rank = 0, det = 1;
  vector<int> pos(N, -1);
  for(int col = 0, row = 0; col < m && row < n; ++col) {
    int mx = row;
    for(int i = row; i < n; ++i) if(a[i][col]) { mx = i; break; }
    if(!a[mx][col]) { det = 0; continue; }
    swap(a[mx], a[row]);
    if(row != mx) det = det == 0 ? 0 : 1;
    det &= a[row][col];
    pos[col] = row;
    //forward elimination
    for(int i = row + 1; i < n; ++i) if (i != row && a[i][col]) a[i] ^= a[row];
    ++row, ++rank;
  }
  ans.reset();
  //backward substituition
  for (int i = m - 1; i >= 0; i--) {
      if (pos[i] == -1) ans[i] = true;
      else {
        int k = pos[i];
        for (int j = i + 1; j < m; j++) if (a[k][j]) ans[i] = ans[i] ^ ans[j];
        ans[i] = ans[i] ^ a[k][m];
      }
  }
  for(int i = rank; i < n; ++i) if(a[i][m]) return -1; //no solution
  //reversing again beacuse we reversed earlier
  bitset<N> tmp;
  for (int j = 0; j < m; j++) tmp[j] = ans[m - j - 1];
  ans = tmp;
  int free_var = 0;
  for(int i = 0; i < m; ++i) if(pos[i] == -1) free_var++;
  return free_var; //has solution
} 
string read() {
   string t;
   if(!(cin >> t)) return "";
   if(t.empty() || t == "and") return "";
   while(t[0] == '(') t.erase(t.begin());
   while(t.back() == ')') t.pop_back();
   return t;
}
bool is_var(string t) { return t.size() > 0 && t[0] == 'x'; }
int get_var(string t) { return atoi(t.substr(1).c_str()) - 1; }
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m; cin >> n >> m;
  vector<bitset<N>> bs(n, bitset<N>(0));
  for(int i = 0; i < n; i++) {
     string s;
     bool eq = 1;
     while((s = read()).size() > 0) {
        if(is_var(s)) {
          int x = get_var(s);
          bs[i][x] = bs[i][x] ^ 1;
        }
        else if(s == "not") eq ^= 1;
     }
     bs[i][m] = eq;
  }
  bitset<N> ans; 
  int ok = Gauss(n, m, bs, ans);
  if (ok == -1) cout << "impossible\n";
  else {
    for (int i = 0; i < m; i++) cout << "FT"[ans[i]]; cout << '\n';
  }
   return 0;
}
//https://codeforces.com/gym/101908/problem/M
