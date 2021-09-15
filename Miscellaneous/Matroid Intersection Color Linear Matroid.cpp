#include <bits/stdc++.h>

using namespace std;

// credit: Alpha_Q
typedef long long ll;

const int L = 62;
const int N = 6010;

ll a[N];
bool taken[N];
vector <int> ans;
vector <ll> basis, without[L];
int n, m, tot, col[N], where[N], par[N];

vector <ll> computeBasis (vector <ll> vec) {
  for (int i = 0; i < vec.size(); ++i) {
    for (int j = i + 1; j < vec.size(); ++j) {
      if (vec[j] > vec[i]) swap(vec[i], vec[j]);
    }
    for (int j = i + 1; j < vec.size(); ++j) {
      vec[j] = min(vec[j], vec[j] ^ vec[i]);
    }
  }
  while (!vec.empty() and !vec.back()) vec.pop_back();
  return vec;
}

void prepare() {
  basis.clear();
  for (int i = 0; i < ans.size(); ++i) {
    basis.emplace_back(a[ans[i]]);
    without[i].clear();
    for (int j = 0; j < ans.size(); ++j) if (j ^ i) {
      without[i].emplace_back(a[ans[j]]);
    }
    without[i] = computeBasis(without[i]);
  }
  basis = computeBasis(basis);
  for (int i = 0; i < n; ++i) taken[i] = 0;
  for (int i : ans) taken[col[i]] = 1;
}

inline bool colorOK (int i) {
  return !taken[col[i]];
}

inline bool colorOK (int out, int in) {
  return !taken[col[in]] or col[in] == col[out];
}

bool vectorOK (int i) {
  ll x = a[i];
  for (ll &v : basis) x = min(x, x ^ v);
  return x;
}

bool vectorOK (int out, int in) {
  ll x = a[in];
  for (ll &v : without[where[out]]) x = min(x, x ^ v);
  return x;
}

bool augment() {
  prepare();
  queue <int> q;
  for (int i = 0; i < tot; ++i) {
    par[i] = where[i] = -1;
    if (colorOK(i)) q.emplace(i), par[i] = -69;
  }
  for (int i = 0; i < ans.size(); ++i) {
    where[ans[i]] = i;
  }
  int sink = -1;
  while (!q.empty()) {
    int u = q.front(); q.pop();
    if (~where[u]) {
      for (int v = 0; v < tot; ++v) if (par[v] == -1 and colorOK(u, v)) {
        par[v] = u, q.emplace(v);
      }
    } else {
      if (vectorOK(u)) {
        sink = u;
        break;
      }
      for (int v : ans) if (par[v] == -1 and vectorOK(v, u)) {
        par[v] = u, q.emplace(v);
      }
    }
  }
  if (sink == -1) return 0;
  for (int i : ans) where[i] = 1;
  while (sink != -69) where[sink] *= -1, sink = par[sink];
  ans.clear(); 
  for (int i = 0; i < tot; ++i) if (where[i] == 1) ans.emplace_back(i);
  return 1;
}

int main() {
  cin >> n;
  for (int i = 0; i < n; ++i) {
    ll x;
    scanf("%lld", &x);
    a[tot] = x, col[tot++] = i;
  }
  cin >> m;
  for (int i = 0; i < m; ++i) {
    int k; ll x; 
    cin >> k;
    while (k--) {
      scanf("%lld", &x);
      a[tot] = x, col[tot++] = n + i;
    }
  }
  swap(n, m); n += m; while (augment());
  if (ans.size() < n) {
    cout << "-1\n";
    return 0;
  }
  for (int i : ans) if (col[i] >= m) printf("%lld\n", a[i]);
  return 0;
}
// https://codeforces.com/gym/102156/submission/109295539
