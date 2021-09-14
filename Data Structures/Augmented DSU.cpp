/* Augmented Dsu
Application:- used for maintaining a system of equations of the form ( y-x = d ) along
with  their consistencial queries dynamically using disjoint set union and find data structure.
Inspired by the problem http://www.spoj.com/problems/CHAIN/  which utilises this concept, which can extended for solving
problems of kind as explained above.
Credit:Arpit Gupta(@alphawizard)
*/
#include<bits/stdc++.h>
using namespace std;
const int N = 10000;
int flaw;     //counting numbers of inconsistent assertions
int pot[N], prec[N];

void initialize(int n) {
  flaw = 0;
  for(int i = 1; i <= n; ++i) {
    prec[i] = i;
    pot[i] = 0;
  }

}
int find(int x) {
  if(prec[x] == x) return x;
  int rx = find(prec[x]);  // rx is the root of x
  pot[x] = pot[prec[x]] + pot[x]; //add all potentials along the path,i.e.,potential calculated wrt root
  prec[x] = rx;
  return rx;
}
void merge(int a, int b, int d) {
  int ra = find(a);
  int rb = find(b);
  if(ra == rb && pot[a] - pot[b] != d) flaw++;
  else if(ra != rb) {
    pot[ra] = d + pot[b] - pot[a];
    prec[ra] = rb;
  }
}
int main() {
  int n;   //no. of variables
  cin >> n;
  int m;   // no. of equations
  cin >> m;
  initialize(n);
  for(int i = 1; i <= m; ++i) { //consider 1-based indexing of variables
    int a, b, d;         //asserting a-b=d;
    cin >> a >> b >> d;
    merge(a, b, d);
  }
  cout << "No. of inconsistencies= " << flaw;
  //queries of type y-x=? can be given through pot[y]-pot[x] (only when then are in same component
  //i.e., can be extracted from the information so far )
  return 0;
}
