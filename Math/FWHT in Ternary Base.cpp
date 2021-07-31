#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, mod = 1e9 + 7;

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

struct cmplx {
  mint a, b;
  cmplx(int _a = 0, int _b = 0) { a = _a, b = _b; }
  cmplx(mint _a, mint _b) { a = _a, b = _b; }
  cmplx operator+(cmplx x) { return cmplx(a + x.a, b + x.b); }
  cmplx operator*(int k) { return cmplx(a * k, b * k); }
  cmplx operator*(mint k) { return cmplx(a * k, b * k); }
  cmplx operator*(cmplx x) {
    mint na = a * x.a - b * x.b;
    mint nb = a * x.b + b * x.a - b * x.b;
    return cmplx(na, nb);
  }
} w(0, 1), w2(mod - 1, mod - 1);

//(mod + 1) should be divisible by 3
typedef vector<cmplx> poly;
struct FWHT {
  const int BIT = 11; // log2(MAX VALUE)
  const int M = 177147; //3^BIT, it must be a power of 3
  void fwht(poly &coefs, poly &vals, bool invert = false) {
    vals = coefs;
    int n = vals.size();
    for (int len = 1; len < n; len *= 3) {
      int pitch = len * 3;
      int len2 = len * 2;
      for (int i = 0; i < n; i += pitch) {
        for (int j = 0; j < len; j++) {
          cmplx a = vals[i + j];
          cmplx b = vals[i + j + len];
          cmplx c = vals[i + j + len2];
          vals[i + j] = a + b + c;
          vals[i + j + len] = a + b * w + c * w2;
          vals[i + j + len2] = a + b * w2 + c * w;
          if(invert) swap(vals[i + j + len], vals[i + j + len2]);
        }
      }
    }
    int inv3 = (mod + 1) / 3;
    mint inv = 1;
    for(int i = 1; i < n; i *= 3) inv *= inv3;
    if (invert) for (int i = 0; i < n; i++) vals[i]= vals[i] * inv;
    return;
  }
  cmplx power(cmplx x, long long n) {
    cmplx ret(1);
    while (n) {
      if(n & 1) ret = ret * x;
      x = x * x;
      n >>= 1;
    }
    return ret;
  }
  //converts the binary representation of x to ternary
  //5 (101 in binary) is transformed to 10 (101 in ternary)
  int tobase3(int x) {
    int ret = 0;
    for(int i = BIT - 1; i >= 0; i--){
      ret *= 3;
      if(x & (1 << i)) ret++;
    }
    return ret;
  }
  poly convert(int n, vector<int> &a){
    poly p(M);
    for(int i = 0; i < n; i++){
      int x = tobase3(i);
      assert(x < M);
      p[x].a += a[i];
    }
    return p;
  }
  //O(3^BIT * BIT)
  vector<int> multiply(int n, vector<int> &a, vector<int> &b) {
    poly p = convert(n, a), q = convert(n, b);
    fwht(p, p); fwht(q, q);
    for(int i = 0; i < M; i++) p[i] = p[i] * q[i];
    fwht(p, p, true);
    vector<int> ans;
    for(int i = 0; i < M; i++) ans.push_back(p[i].a.value);
    return ans;
  }
  vector<int> pow(int n, vector<int> &a, long long k) {
    poly p = convert(n, a);
    fwht(p, p);
    for(int i = 0; i < M; i++) p[i] = power(p[i], k);
    fwht(p, p, true);
    vector<int> ans;
    for(int i = 0; i < M; i++) ans.push_back(p[i].a.value);
    return ans;
  }
}t;

int32_t main() {
  
  return 0;
}
// https://www.codechef.com/problems/MDSWIN
