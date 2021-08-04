#include<bits/stdc++.h>
using namespace std;

const int N = 1010;

double p[N];
int b[N], c[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n; cin >> n;
  int tot = 0;
  for (int i = 1; i <= n; i++) {
    cin >> b[i];
    tot += b[i];
  }
  int sum = 0;
  for (int i = 1; i <= n; i++) {
    cin >> c[i];
    sum += c[i];
  }
  for (int i = 1; i <= n; i++) {
    p[i] = 1.0 * c[i] / sum;
  }
  double l = 0, r = 2e6;
  int it = 100;
  while (it--) {
    double mid = (l + r) * 0.5;
    double sum = 0;
    for (int i = 1; i <= n; i++) {
      sum += sqrt(p[i] * b[i] / mid);
    }
    if (sum <= 1) {
      r = mid;
    }
    else {
      l = mid;
    }
  }
  cout << fixed << setprecision(10) << tot << ' ' << l << '\n';
  for (int i = 1; i <= n; i++) {
    cout << fixed << setprecision(10) << sqrt(p[i] * b[i] / l) << ' ';
  }
  return 0;
}
// https://toph.co/p/betting-business