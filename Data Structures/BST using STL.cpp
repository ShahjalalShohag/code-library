#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

//the code returns a BST which will create if we add the values one by one
//here nodes are indicated by values and every node must be distinct
set<int>se;
map<int, int>l, r; //l contains the left child of the node, r contains right child of the node
int main() {
  int n;
  cin >> n;
  int k;
  cin >> k; //root of the tree
  se.insert(k);
  for(int i = 1; i < n; i++) {
    int k;
    cin >> k;
    auto it = se.upper_bound(k);
    if(it != se.end() && l.find(*it) == l.end()) l[*it] = k;
    else --it, r[*it] = k;
    se.insert(k);
  }
  for(int i = 1; i <= n; i++) cout << l[i] << ' ' << r[i] << '\n';
  return 0;
}
