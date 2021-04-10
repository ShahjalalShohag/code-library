#include<bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
using vi = vector<int>;

// credit- ksun
std::ostream& operator << (std::ostream& o, const vi& v) {
  o << "(";
  for (int i = int(v.size())-1; i >= 0; i--) {
    int c = v[i];
    if (c == 0) continue;
    if (i < int(v.size()) - 1) {
      o << (c > 0 ? '+' : '-');
    } else {
      assert(c == 1);
    }
    c = abs(c);
    if (i == 0) {
      o << c;
    } else {
      if (c != 1) o << c;
      o << "x";
      if (i > 1) o << "^" << i;
    }
  }
  o << ")";
  return o;
}


// multiply by x^e - 1
void mul(vi &a, int e) {
  int n = int(a.size());
  a.insert(a.begin(), e, 0); 
  for (int i = 0; i < n; i++) {
    a[i] -= a[i+e];
    if (a[i] < 0) a[i] += MOD;
  }
}

// divide by x^e - 1
void div(vi &a, int e) {
  int n = int(a.size());
  for (int i = n-1-e; i >= 0; i--) {
    a[i] += a[i+e];
    if (a[i] >= MOD) a[i] -= MOD;
  }
  for (int i = 0; i < e; i++) {
    assert(a[i] == 0);
  }
  a.erase(a.begin(), a.begin() + e);
}

const int MAXN = 1.1e5;
bool isPrime[MAXN];
int mu[MAXN];
void precomp() {
  for (int i = 1; i < MAXN; i++) {
    isPrime[i] = true;
    mu[i] = 1;
  }
  isPrime[1] = false;
  for (int p = 2; p < MAXN; p++) {
    if (!isPrime[p]) continue;
    assert(mu[p] == 1);
    mu[p] *= -1;
    for (int j = p + p; j < MAXN; j += p) {
      isPrime[j] = false;
      if (j / p % p == 0) mu[j] = 0;
      else mu[j] *= -1;
    }
  }
}
vi cache[MAXN];
void get(int N) { // compute the cyclotomic polynomial phi_N(x)
  if (!cache[N].empty()) return;
  cache[N] = vi({1});
  for (int d = 1; d <= N; d++) {
    if (N % d) continue;
    if (mu[N/d] == 1) {
      mul(cache[N], d);
    }
  }
  for (int d = 1; d <= N; d++) {
    if (N % d) continue;
    if (mu[N/d] == -1) {
      div(cache[N], d);
    }
  }
  for (int& i : cache[N]) {
    if (MOD - i < i) {
      i -= MOD;
    }
  }
}

// compute the factorization of (x^N - 1)
int main() {
  ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
  precomp();
  int T; cin >> T;
  while (T--) {
    int N; cin >> N;
    vector<int> inds;
    for (int d = 1; d <= N; d++) {
      if (N % d) continue;
      get(d);
      inds.push_back(d);
    }
    sort(inds.begin(), inds.end(), [&](int a, int b) -> bool {
        if (cache[a].size() != cache[b].size()) {
          return cache[a].size() < cache[b].size();
        }
        for (int i = int(cache[a].size()) - 1; i >= 0; i--) {
          if (cache[a][i] != cache[b][i]) {
            return cache[a][i] < cache[b][i];
          }
        }
        assert(a == b);
        return false;
    });
    for (int d : inds) {
      cout << cache[d];
    }
    cout << '\n';
  }

  return 0;
}
// https://codeforces.com/gym/102114/submission/50549745