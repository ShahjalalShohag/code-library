#include <iostream>
#include <vector>
using namespace std;
using ll = long long;
const int N = 1e5;

// credit: mango_lassi
int arr[N + 1];
int u[N + 1];
int cnt[N + 1];

vector<int> d[N + 1];
bool b[N + 1];

bool coprime(int x) {
  int ret = 0;
  for (int i : d[x]) ret += cnt[i] * u[i];
  return ret;
}

void update(int x, int a) {
  for (int i : d[x]) cnt[i] += a;
}

int main() {
  for (int i = 1; i <= N; i++) {
    for (int j = i; j <= N; j += i) d[j].push_back(i);
    if (i == 1) u[i] = 1;
    else if ((i / d[i][1]) % d[i][1] == 0) u[i] = 0;
    else u[i] = -u[i / d[i][1]];
  }

  int n;
  cin >> n;

  ll ans = 0;
  for (int i = 0; i < n; i++) {
    int a;
    cin >> a;
    ans = max(ans, (ll)a);
    b[a] = 1;
  }
  for (int i = 1; i <= N; ++i) {
    for (int j = 2; i * j <= N; ++j) b[i] |= b[i * j];
  }

  vector<int> s;
  for (int i = N; i > 0; --i) {
    if (! b[i]) continue;
    while(coprime(i)) {
      ans = max(ans, (ll)i * s.back());
      update(s.back(), -1);
      s.pop_back();
    }
    update(i, 1);
    s.push_back(i);
  }
  cout << ans << '\n';
}
// https://codeforces.com/contest/1285/submission/68564248
