#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

const int N = 102;
const int LG = 3330;
const int MOD = 998244353;
const ll INV = 1 + MOD >> 1;

struct field { // a . 1 + b . sqrt(5)
  ll a, b;

  field (ll a = 0, ll b = 0) : a(a), b(b) {}

  bool operator == (const field &oth) const {
    return a == oth.a and b == oth.b;
  }

  field operator + (const field &oth) const {
    return field((a + oth.a) % MOD, (b + oth.b) % MOD);
  }

  field operator - (const field &oth) const {
    return field((a - oth.a + MOD) % MOD, (b - oth.b + MOD) % MOD);
  }

  field operator * (const field &oth) const {
    return field((a * oth.a + 5 * b * oth.b) % MOD, (a * oth.b + b * oth.a) % MOD);
  }
};

ll bigMod (ll a, ll e) {
  if (e == -1) e = MOD - 2;
  ll ret = 1;
  while (e) {
    if (e & 1) ret = ret * a % MOD;
    a = a * a % MOD, e >>= 1;
  }
  return ret;
}

field bigMod (field a, ll e) {
  field ret(1);
  while (e) {
    if (e & 1) ret = ret * a;
    a = a * a, e >>= 1;
  }
  return ret;
}

string binPower;
int t, n, st, diff, inp[N][N];
vector <field> reduced[N], two[LG], twoSum[LG];
field base[N][N], pwr[N][N][N], parts[2][N][N];

void copy (field a[N][N], field b[N][N]) {
  for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
    b[i][j] = a[i][j];
  }
}

void mult (field a[N][N], field b[N][N], field c[N][N]) {
  for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
    c[i][j] = field();
    for (int k = 0; k < n; ++k) c[i][j] = c[i][j] + a[i][k] * b[k][j];
  }
}

string getBinary (string decimal) {
  string ret = "";
  while (!decimal.empty() and !(decimal.size() == 1 and decimal.back() == '0')) {
    ret += (char) ('0' + (decimal.back() & 1));
    string to = ""; int carry = 0;
    for (char c : decimal) {
      carry = 10 * carry + c - '0';
      if (!to.empty() or carry > 1) to += (char) ('0' + carry / 2);
      carry &= 1;
    }
    decimal = to;
  } return ret;
}

vector <field> mult (vector <field> a, vector <field> b) {
  vector <field> ret(n + n, field());
  for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
    ret[i + j] = ret[i + j] + a[i] * b[j];
  }
  for (int i = 0; i < n; ++i) {
    field co = ret[i + n]; if (co == field()) continue;
    for (int j = 0; j < n; ++j) ret[j] = ret[j] + co * reduced[i][j];
  }
  ret.resize(n); return ret;
}

void fuck (int idx) {
  field alpha = idx ? field(INV, MOD - INV) : field(INV, INV);
  field alpha_power = bigMod(alpha, diff);
  for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
    base[i][j] = field(inp[i][j]) * alpha_power;
  }
  for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
    pwr[0][i][j] = field(i == j);
  }
  for (int i = 1; i <= n; ++i) mult(pwr[i - 1], base, pwr[i]);
  vector <field> trace(n + 1), char_poly(n);
  for (int it = 1; it <= n; ++it) {
    trace[it] = field();
    for (int i = 0; i < n; ++i) trace[it] = trace[it] + pwr[it][i][i];
    field coeff = trace[it];
    for (int j = 1; j < it; ++j) coeff = coeff + trace[j] * char_poly[it - j - 1];
    char_poly[it - 1] = coeff * field(bigMod(it, -1));
    char_poly[it - 1] = field() - char_poly[it - 1];
  }
  for (field &x : char_poly) x = field() - x;
  reverse(char_poly.begin(), char_poly.end());
  reduced[0] = char_poly;
  for (int i = 1; i < n; ++i) {
    reduced[i] = reduced[i - 1];
    field last = reduced[i].back();
    for (int j = n - 1; j; --j) reduced[i][j] = reduced[i][j - 1]; reduced[i][0] = field();
    for (int j = 0; j < n; ++j) reduced[i][j] = reduced[i][j] + last * reduced[0][j];
  }
  two[0] = vector <field> (n, field()); 
  if (n == 1) two[0] = reduced[0]; else two[0][1] = field(1);
  for (int i = 1; i < binPower.size(); ++i) two[i] = mult(two[i - 1], two[i - 1]);
  twoSum[0] = vector <field> (n, field()); twoSum[0][0] = field(1);
  for (int i = 0; i + 1 < binPower.size(); ++i) {
    vector <field> cur = two[i]; cur[0] = cur[0] + field(1);
    twoSum[i + 1] = mult(twoSum[i], cur);
  }
  vector <field> ans(n, field());
  for (int i = 0; i < binPower.size(); ++i) if (binPower[i] == '1') {
    ans = mult(ans, two[i]);
    for (int j = 0; j < n; ++j) ans[j] = ans[j] + twoSum[i][j];
  }
  for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) parts[idx][i][j] = field();
  for (int it = 0; it < n; ++it) {
    field co = ans[it];
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
      parts[idx][i][j] = parts[idx][i][j] + co * pwr[it][i][j];
    }
  }
  alpha_power = bigMod(alpha, st);
  for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
    parts[idx][i][j] = parts[idx][i][j] * alpha_power;
  }
}

int main() {
  cin >> t;
  while (t--) {
    cin >> n >> st >> diff >> binPower; 
    binPower = getBinary(binPower);
    int at = 0; while (at < binPower.size() and binPower[at] == '1') {
      binPower[at++] = '0';
    }
    if (at == binPower.size()) binPower += '1'; else binPower[at] = '1';
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
      scanf("%d", inp[i] + j);
    }
    fuck(0); fuck(1); field extra = field(0, bigMod(5, -1));
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        field cur = (parts[0][i][j] - parts[1][i][j]) * extra;
        assert(cur.b == 0); printf("%lld ", cur.a);
      } puts("");
    }
  }
  return 0;
}
