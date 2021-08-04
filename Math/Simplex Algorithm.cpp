#include<bits/stdc++.h>
using namespace std;

const int N=510;
const int mod=1e9+7;
using ll = long long;
using ld = long double;
const ld eps=1e-9;
const ld PI=acos(-1.0);

ll n,k,a[N],dis[N][N];
vector <ll> g[N],cost[N];
bool ok[N][N];

void dfs(ll u,ll p,ll d,ll cur) {
  dis[cur][u] = d;
  ll len = g[u].size(), i;
  for(int i=0;i<len;i++) {
    ll v = g[u][i];
    if(v == p )continue;
    dfs(v,u,d+cost[u][i],cur);
  }
}

#define MAXC 1010
#define MAXV 1010
#define EPS 1e-13

#define MINIMIZE -1
#define MAXIMIZE +1
#define LESSEQ -1
#define EQUAL 0
#define GREATEQ 1
#define INFEASIBLE -1
#define UNBOUNDED 666


/***
1.  Simplex Algorithm for Linear Programming
2.  Maximize or minimize f0*x0 + f1*x1 + f2*x2 + ... + fn-1*xn-1 subject to some constraints
3.  Constraints are of the form, c0x0 + c1x1 + c2x2 + ... + cn-1xn-1 (<= or >= or =) lim
4.  m is the number of constraints indexed from 1 to m, and n is the number of variables indexed from 0 to n-1
5.  ar[0] contains the objective function f, and ar[1] to ar[m] contains the constraints, ar[i][n] = lim_i
6.  It is assumed that all variables satisfies non-negativity constraint, i.e, xi >= 0
7.  If non-negativity constraint is not desired for a variable x, replace each occurrence
  by difference of two new variables r1 and r2 (where r1 >= 0 and r2 >= 0, handled automatically by simplex).
  That is, replace every x by r1 - r2 (Number of variables increases by one, -x, +r1, +r2)
8.  solution_flag = INFEASIBLE if no solution is possible and UNBOUNDED if no finite solution is possible
9.  Returns the maximum/minimum value of the linear equation satisfying all constraints otherwise
10. After successful completion, val[] contains the values of x0, x1 .... xn for the optimal value returned

*** If ABS(X) <= M in constraints, Replace with X <= M and -X <= M

*** Fractional LP:
  max/min
    3x1 + 2x2 + 4x3 + 6
    -------------------
    3x1 + 3x2 + 2x3 + 5

    constraint:
    2x1 + 3x2 + 5x3 ≥ 23
    3x2 + 5x2 + 4x3 <= 30
    x1, x2, x3 ≥ 0

  Replace with:
  max/min
    3y1 + 2y2 + 4y3 + 6t

    constraint:
    3y1 + 3y2 + 2y3 + 5t = 1
    2y1 + 3y2 + 53 - 23t ≥ 0
    3y1 + 5y2 + 4y3- 30t <= 0
    y1, y2, y3, t ≥ 0
***/

//Complexity: O(n^3) or much faster
//MAXV=1e5,MAXC=2 works within 80ms
//MAXV=1e5,MAXC=200 works within 1.5s

namespace lp {
  long double val[MAXV], ar[MAXC][MAXV];
  int m, n, solution_flag, minmax_flag, basis[MAXC], index[MAXV];

  /// nvars = number of variables, f = objective function, flag = MINIMIZE or MAXIMIZE
  inline void init(int nvars, long double f[], int flag){
    solution_flag = 0;
    ar[0][nvars] = 0.0;
    m = 0, n = nvars, minmax_flag = flag;
    for (int i = 0; i < n; i++){
      ar[0][i] = f[i] * minmax_flag; /// Negating sign of objective function when minimizing
    }
  }

  /// C[] = co-efficients of the constraints (LHS), lim = limit in RHS
  /// cmp = EQUAL for C[] = lim, LESSEQ for C[] <= lim, GREATEQ for C[] >= lim
  inline void add_constraint(long double C[], long double lim, int cmp){
    m++, cmp *= -1;
    if (cmp == 0){
      for (int i = 0; i < n; i++) ar[m][i] = C[i];
      ar[m++][n] = lim;
      for (int i = 0; i < n; i++) ar[m][i] = -C[i];
      ar[m][n] = -lim;
    }
    else{
      for (int i = 0; i < n; i++) ar[m][i] = C[i] * cmp;
      ar[m][n] = lim * cmp;
    }
  }

  inline void init(){ /// Initialization
    for (int i = 0; i <= m; i++) basis[i] = -i;
    for (int j = 0; j <= n; j++){
      ar[0][j] = -ar[0][j], index[j] = j, val[j] = 0;
    }
  }

  inline void pivot(int m, int n, int a, int b){ /// Pivoting and exchanging a non-basic variable with a basic variable
    for (int i = 0; i <= m; i++){
      if (i != a){
        for (int j = 0; j <= n; j++){
          if (j != b){
            ar[i][j] -= (ar[i][b] * ar[a][j]) / ar[a][b];
          }
        }
      }
    }
    for (int j = 0; j <= n; j++){
      if (j != b) ar[a][j] /= ar[a][b];
    }
    for (int i = 0; i <= m; i++){
      if (i != a) ar[i][b] = -ar[i][b] / ar[a][b];
    }
    ar[a][b] = 1.0 / ar[a][b];
    swap(basis[a], index[b]);
  }
  inline long double solve(){ /// simplex core
    init();
    int i, j, k, l;
    for (; ;){
      for (i = 1, k = 1; i <= m; i++){
        if ((ar[i][n] < ar[k][n]) || (ar[i][n] == ar[k][n] && basis[i] < basis[k] && (rand() & 1))) k = i;
      }
      if (ar[k][n] >= -EPS) break;

      for (j = 0, l = 0; j < n; j++){
        if ((ar[k][j] < (ar[k][l] - EPS)) || (ar[k][j] < (ar[k][l] - EPS) && index[i] < index[j] && (rand() & 1))){
          l = j;
        }
      }
      if (ar[k][l] >= -EPS){
        solution_flag = INFEASIBLE; /// No solution is possible
        return -1.0;
      }
      pivot(m, n, k, l);
    }
    for (; ;){
      for (j = 0, l = 0; j < n; j++){
        if ((ar[0][j] < ar[0][l]) || (ar[0][j] == ar[0][l] && index[j] < index[l] && (rand() & 1))) l = j;
      }
      if (ar[0][l] > -EPS) break;
      for (i = 1, k = 0; i <= m; i++){
        if (ar[i][l] > EPS && (!k || ar[i][n] / ar[i][l] < ar[k][n] / ar[k][l] - EPS || (ar[i][n] / ar[i][l] < ar[k][n] / ar[k][l] + EPS && basis[i] < basis[k]))){
          k = i;
        }
      }
      if (ar[k][l] <= EPS){
        solution_flag = UNBOUNDED; /// Solution is infinity, no finite solution exists
        return -666.0;
      }
      pivot(m, n, k, l);
    }
    for (i = 1; i <= m; i++){
      if (basis[i] >= 0) val[basis[i]] = ar[i][n];
    }
    solution_flag = 1; /// Successful completion
    return (ar[0][n] * minmax_flag); /// Negate the output for MINIMIZE since the objective function was negated
  }
}
long double obj[N],cons[N];

int32_t main() {
  ll black = 0;
  cin >> n >> k;
  for(int i = 0; i < n; i++) cin >> a[i], black += a[i];
  for(int i = 1; i < n; i++) {
    ll x, y, w; cin >> x >> y >> w;
    x--; y--;
    g[x].push_back(y);
    g[y].push_back(x);
    cost[x].push_back(w);
    cost[y].push_back(w);
  }
  for(int i = 0; i < n; i++) dfs(i,-1,0,i);
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      if(dis[i][j] <= k )ok[i][j] = 1;
    }
  }
  for(int i = 0; i < n; i++) obj[i] = (long double)1 - a[i];
  lp::init(n,obj,MINIMIZE);
  for(int i = 0; i < n; i++)cons[i] = 1.0;
  lp::add_constraint(cons,black,EQUAL);
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      cons[j] = (long double)ok[i][j];
    }
    lp::add_constraint(cons,1.0,GREATEQ);
  }
  long double ret = lp::solve();
  ll ans = round(ret);
  if(ans < 0)ans = -1;
  cout<<ans<< '\n';
  return 0;
}
// https://codeforces.com/problemset/problem/375/E
