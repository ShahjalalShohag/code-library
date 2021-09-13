#include<bits/stdc++.h>
using namespace std;

#define ll long long
#define eb emplace_back
#define nl '\n'
#define deb(x) cerr << #x" = " << x << nl
#define in() ( { int a ; scanf("%d", &a); a; } )

const int N = 210;
const int mod = 1e9 + 7;

template <int32_t MOD>
struct modint {
  int32_t value;
  modint() = default;
  modint(int32_t value_) : value(value_) {}
  inline modint<MOD> operator + (modint<MOD> other) const { int32_t c = this->value + other.value; return modint<MOD>(c >= MOD ? c - MOD : c); }
  inline modint<MOD> operator - (modint<MOD> other) const { int32_t c = this->value - other.value; return modint<MOD>(c <    0 ? c + MOD : c); }
  inline modint<MOD> operator * (modint<MOD> other) const { int32_t c = (int64_t)this->value * other.value % MOD; return modint<MOD>(c < 0 ? c + MOD : c); }
  inline modint<MOD> & operator += (modint<MOD> other) { this->value += other.value; if (this->value >= MOD) this->value -= MOD; return *this; }
  inline modint<MOD> & operator -= (modint<MOD> other) { this->value -= other.value; if (this->value <    0) this->value += MOD; return *this; }
  inline modint<MOD> & operator *= (modint<MOD> other) { this->value = (int64_t)this->value * other.value % MOD; if (this->value < 0) this->value += MOD; return *this; }
  inline modint<MOD> operator - () const { return modint<MOD>(this->value ? MOD - this->value : 0); }
  modint<MOD> pow(uint64_t k) const {
    modint<MOD> x = *this, y = 1;
    for (; k; k >>= 1) {
      if (k & 1) y *= x;
      x *= x;
    }
    return y;
  }
  modint<MOD> inv() const { return pow(MOD - 2); }  // MOD must be a prime
  inline modint<MOD> operator /  (modint<MOD> other) const { return *this *  other.inv(); }
  inline modint<MOD> operator /= (modint<MOD> other)       { return *this *= other.inv(); }
  inline bool operator == (modint<MOD> other) const { return value == other.value; }
  inline bool operator != (modint<MOD> other) const { return value != other.value; }
};
template <int32_t MOD> modint<MOD> operator * (int64_t value, modint<MOD> n) { return modint<MOD>(value) * n; }
template <int32_t MOD> modint<MOD> operator * (int32_t value, modint<MOD> n) { return modint<MOD>(value % MOD) * n; }
template <int32_t MOD> ostream & operator << (ostream & out, modint<MOD> n) { return out << n.value; }
using mint = modint<mod>;

mint comb[N][N];
void pre() {
  for(int i = 0; i < N; i++) for(int j = 0; j < N; j++) comb[i][j] = 0;
  for(int i=0;i<N;i++) comb[i][0]=1;
  for(int i=1;i<N;i++){
    for(int j=1;j<N;j++) comb[i][j]=comb[i-1][j-1]+comb[i-1][j];
  }
}
bool vis[N][N];
mint dp[N][N], C[N], G[N];
mint yo(int n, int k) {
  if(k > n) return mint(0);
  if(k == n) return mint(1);
  if(k == 0) return 0;
  if(vis[n][k]) return dp[n][k];
  vis[n][k] = 1;
  mint ans = 0;
  for(int s = 1; s <= n; s++){
    ans += (comb[n - 1][s - 1] * C[s]) * yo(n - s, k - 1);
  }
  return dp[n][k] = ans;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  pre();
  for(int i = 1; i < N ; i++) G[i] = mint(2).pow(i * (i - 1) / 2);
  for(int n = 1; n < N; n++){
    mint ans = 0;
    for(int k = 1; k < n; k++){
      ans += (mint(k) * comb[n][k]) * (C[k] * G[n - k]);
    }
    ans /= n;
    C[n] = G[n] - ans;
  }
  int t; cin >> t;
  while(t--){
    int n, k ; cin >> n >> k;
    cout << yo(n, k).value << nl;
  }
  return 0;
}
// https://cp-algorithms.com/combinatorics/counting_labeled_graphs.html
