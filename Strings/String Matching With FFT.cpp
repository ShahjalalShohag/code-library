// https://www.informatika.bg/resources/StringMatchingWithFFT.pdf
#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;
const int mod = 1e9 + 7;

const double PI = acos(-1);
struct base {
  double a, b;
  base(double a = 0, double b = 0) : a(a), b(b) {}
  const base operator + (const base &c) const {
    return base(a + c.a, b + c.b);
  }
  const base operator - (const base &c) const {
    return base(a - c.a, b - c.b);
  }
  const base operator * (const base &c) const {
    return base(a * c.a - b * c.b, a * c.b + b * c.a);
  }
};
void fft(vector<base> &p, bool inv = 0) {
  int n = p.size(), i = 0;
  for(int j = 1; j < n - 1; ++j) {
    for(int k = n >> 1; k > (i ^= k); k >>= 1);
    if(j < i) swap(p[i], p[j]);
  }
  for(int l = 1, m; (m = l << 1) <= n; l <<= 1) {
    double ang = 2 * PI / m;
    base wn = base(cos(ang), (inv ? 1. : -1.) * sin(ang)), w;
    for(int i = 0, j, k; i < n; i += m) {
      for(w = base(1, 0), j = i, k = i + l; j < k; ++j, w = w * wn) {
        base t = w * p[j + l];
        p[j + l] = p[j] - t;
        p[j] = p[j] + t;
      }
    }
  }
  if(inv) for(int i = 0; i < n; ++i) p[i].a /= n, p[i].b /= n;
}
vector<int> multiply(vector<int> &a, vector<int> &b) {
  int n = a.size(), m = b.size(), t = n + m - 1, sz = 1;
  while(sz < t) sz <<= 1;
  vector<base> x(sz), y(sz), z(sz);
  for(int i = 0 ; i < sz; ++i) {
    x[i] = i < a.size() ? base(a[i], 0) : base(0, 0);
    y[i] = i < b.size() ? base(b[i], 0) : base(0, 0);
  }
  fft(x), fft(y);
  for(int i = 0; i < sz; ++i) z[i] = x[i] * y[i];
  fft(z, 1);
  vector<int> ret(sz);
  for(int i = 0; i < sz; ++i) ret[i] = z[i].a + 0.5;
  while(ret.size() > 1 && ret.back() == 0) ret.pop_back();
  return ret;
}
//find occurrences of t in s where '?'s are automatically matched with any character
//res[i + m - 1] = sum_j=0 to m - 1_{s[i + j] * t[j] * (s[i + j] - t[j])^2
vector<int> string_matching(string &s, string &t) {
  int n = s.size(), m = t.size();
  vector<int> s1(n), s2(n), s3(n);
  for(int i = 0; i < n; i++) s1[i] = s[i] == '?' ? 0 : s[i] - 'a' + 1; //assign any non zero number for non '?'s
  for(int i = 0; i < n; i++) s2[i] = s1[i] * s1[i];
  for(int i = 0; i < n; i++) s3[i] = s1[i] * s2[i];
  vector<int> t1(m), t2(m), t3(m);
  for(int i = 0; i < m; i++) t1[i] = t[i] == '?' ? 0 : t[i] - 'a' + 1;
  for(int i = 0; i < m; i++) t2[i] = t1[i] * t1[i];
  for(int i = 0; i < m; i++) t3[i] = t1[i] * t2[i];
  reverse(t1.begin(), t1.end());
  reverse(t2.begin(), t2.end());
  reverse(t3.begin(), t3.end());
  vector<int> s1t3 = multiply(s1, t3);
  vector<int> s2t2 = multiply(s2, t2);
  vector<int> s3t1 = multiply(s3, t1);
  vector<int> res(n);
  for(int i = 0; i < n; i++) res[i] = s1t3[i] - s2t2[i] * 2 + s3t1[i];
  vector<int> oc;
  for(int i = m - 1; i < n; i++) if(res[i] == 0) oc.push_back(i - m + 1);
  return oc;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  int t;
  cin >> t;
  while(t--) {
    string a, b;
    cin >> a >> b;
    auto ans = string_matching(a, b);
    if(ans.empty()) cout << "Not Found\n";
    else {
      for(auto x : ans) cout << x + 1 << endl;
    }
    if(t) cout << endl;
  }
  return 0;
}
