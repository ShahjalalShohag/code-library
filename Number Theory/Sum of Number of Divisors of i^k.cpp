#include<bits/stdc++.h>
using namespace std;

using uint64 = unsigned long long;
using uint32 = unsigned int;
/*
 * In Chinese, this is called ZhouGe's Sieve
 * given a multiplicative function $f(n)$, where $f(p^c)$ is a polynomial of $p$.
 * Find the value of $F(n) = \sum\limits_{1 \le x \le n} f(x)$
 *
 * $F(n) = \sum\limits_{\substack{1 \le x \le n \\ \not \exists p > \sqrt{n}, p \mid x}} f(x) (1 + \sum\limits_{\sqrt{n} < p \le \lfloor \frac{n}{x} \rfloor}f(p))$
 *
 * $F(n)=f(1, n)+\sum\limits_{1 \le x \le \sqrt{n}}f(x)(g(\lfloor \frac{n}{x} \rfloor)-g(\sqrt{n}))$, where $g(n)=\sum\limits_{1 \le p \le n}f(p)$, $f(1,n)=\sum\limits_{\substack{1 \le x \le n \\ \not \exists p > p_m, p \mid x}} f(x)$ and $p_m$ is the largest prime less than or equal to $\sqrt{n}$.
 *
 * $f(i,n)=\sum\limits_{\substack{1 \le x \le n \\ \not \exists p > p_m \text{or } p < p_{i}, p \mid x}} f(x)$, $f(i,n)=f(i+1,n)+\sum\limits_{c \ge 1}f(i+1, \lfloor \frac{n}{p_i^c}\rfloor)$.
 *
 * the following program is an example of \sum_{i=1}^{n} \sigma_0(i^m)
 */
constexpr uint32 N = 7e5; //this should be twice as \sqrt{n}

uint64 e[N], g[N], sigma[N];
uint32 ps[N], pcnt;

uint64 lcnt[N], scnt[N];
uint64 fval[N], fsum[N];
int fsq[N];

uint64 solve(uint64 n, uint64 m) {
  const uint32 sn = sqrt(n);
  auto sieve = [sn, m] () {
    sigma[1] = 1, pcnt = 0;
    memset(ps + 1, 0, sizeof(*ps) * sn);
    for (uint32 i = 2; i <= sn; ++i) {
      if (!ps[i]) {
        ps[pcnt++] = i;
        sigma[i] = e[i] = m + 1;
        g[i] = 1;
      }
      for (uint64 j = 0, u = sn / i; j < pcnt && ps[j] <= u; ++j) {
        uint64 p = ps[j], v = i * p;
        ps[v] = 1;
        if (i % p == 0) {
          g[v] = g[i], e[v] = e[i] + m;
          sigma[v] = g[v] * e[v];
        } else {
          g[v] = sigma[i], e[v] = m + 1;
          sigma[v] = g[v] * e[v];
        }
      }
    }
  };
  auto calc_g = [sn] (uint64 n, uint64 m) {
    for (uint32 i = 1; i <= sn; ++i) {
      lcnt[i] = n / i - 1;
      scnt[i] = i - 1;
    }
    for (uint32 p = 2; p <= sn; ++p) {
      if (scnt[p] == scnt[p - 1]) continue;
      uint64 pcnt = scnt[p - 1];
      uint64 q = (uint64)p * p;
      uint32 ed = min<uint64>(sn, n / q);
      for (uint32 i = 1; i <= ed; ++i) {
        uint64 d = (uint64)i * p;
        if (d <= sn) lcnt[i] -= lcnt[d] - pcnt;
        else lcnt[i] -= scnt[n / d] - pcnt;
      }
      for (uint32 i = sn; i >= q; --i) scnt[i] -= scnt[i / p] - pcnt;
    }
    for (uint32 i = 1; i <= sn; ++i) {
      lcnt[i] = (lcnt[i] - scnt[sn]) * (m + 1);
      scnt[i] *= m + 1;
    }
  };
  auto calc_f = [sn] (uint64 n, uint64 m) {
    for (uint32 i = 1; i <= sn; ++i) {
      fval[i - 1] = i;
      fval[sn - i + sn] = n / i;
      fsum[i - 1] = 1;
      fsum[sn - i + sn] = 1;
    }
    for (uint32 i = 0, j = 0; i < sn * 2; ++i) {
      while (j < pcnt && (uint64)ps[j] * ps[j] <= fval[i]) ++j;
      fsq[i] = j - 1;
    }
    for (int i = pcnt - 1, bound = sn * 2 - 1; i >= 0; --i) {
      while (bound >= 0 && fsq[bound] >= i) --bound;
      uint64 p = ps[i];
      for (int j = sn * 2 - 1; j > bound; --j) {
        uint64 x = fval[j], pe = p, y = x / pe;
        for (int c = 1; y; ++c, pe *= p, y /= p) {
          int k = y <= sn ? y - 1 : 2 * sn - n / y;
          int l = ps[max<int>(i, fsq[k])], r = min<uint64>(y, sn);
          fsum[j] += (fsum[k] + (l < r ? scnt[r] - scnt[l] : 0)) * (m * c + 1);
        }
      }
    }
    return fsum[sn * 2 - 1];
  };
  sieve();
  calc_g(n, m);
  uint64 ret = calc_f(n, m);
  for (uint32 i = 1; i <= sn; ++i) {
    ret += sigma[i] * lcnt[i];
  }
  return ret;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t; cin >> t;
  while (t--) {
    long long n, m; cin >> n >> m;
    auto ans = solve(n, m);
    string s = "";
    while (ans > 0) {
      s += char('0' + ans % 10);
      ans /= 10;
    }
    reverse(s.begin(), s.end());
    cout << s << '\n';
  }
  return 0;
}
// https://www.spoj.com/problems/DIVCNTK/en/