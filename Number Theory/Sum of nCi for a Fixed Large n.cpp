#pragma GCC optimize ("O3")
#pragma GCC target ("avx")

#include <cstdio>
#include <cassert>
#include <cmath>
#include <cstring>

#include <algorithm>
#include <iostream>
#include <vector>
#include <functional>

#ifdef __x86_64__
#define NTT64
#endif

#define _rep(_1, _2, _3, _4, name, ...) name
#define rep2(i, n) rep3(i, 0, n)
#define rep3(i, a, b) rep4(i, a, b, 1)
#define rep4(i, a, b, c) for (int i = int(a); i < int(b); i += int(c))
#define rep(...) _rep(__VA_ARGS__, rep4, rep3, rep2, _)(__VA_ARGS__)

using namespace std;

using i64 = long long;
using u32 = unsigned;
using u64 = unsigned long long;
using f80 = long double;

namespace ntt {

#ifdef NTT64
  using word_t = u64;
  using dword_t = __uint128_t;
#else
  using word_t = u32;
  using dword_t = u64;
#endif
static const int word_bits = 8 * sizeof(word_t);

template <word_t mod, word_t prim_root>
class Mod {
private:
  static constexpr word_t mul_inv(word_t n, int e=6, word_t x=1) {
    return e == 0 ? x : mul_inv(n, e-1, x*(2-x*n));
  }
public:
  static constexpr word_t inv = mul_inv(mod);
  static constexpr word_t r2 = -dword_t(mod) % mod;
  static constexpr int level = __builtin_ctzll(mod - 1);
  static_assert(inv * mod == 1, "invalid 1/M modulo 2^@.");

  Mod() {}
  Mod(word_t n) : x(init(n)) {};
  static word_t modulus() { return mod; }
  static word_t init(word_t w) { return reduce(dword_t(w) * r2); }
  static word_t reduce(const dword_t w) { return word_t(w >> word_bits) + mod - word_t((dword_t(word_t(w) * inv) * mod) >> word_bits); }
  static Mod omega() { return Mod(prim_root).pow((mod - 1) >> level); }
  Mod& operator += (Mod rhs) { this->x += rhs.x; return *this; }
  Mod& operator -= (Mod rhs) { this->x += 3 * mod - rhs.x; return *this; }
  Mod& operator *= (Mod rhs) { this->x = reduce(dword_t(this->x) * rhs.x); return *this; }
  Mod operator + (Mod rhs) const { return Mod(*this) += rhs; }
  Mod operator - (Mod rhs) const { return Mod(*this) -= rhs; }
  Mod operator * (Mod rhs) const { return Mod(*this) *= rhs; }
  word_t get() const { return reduce(this->x) % mod; }
  void set(word_t n) const { this->x = n; }
  Mod pow(word_t exp) const {
    Mod ret = Mod(1);
    for (Mod base = *this; exp; exp >>= 1, base *= base) if (exp & 1) ret *= base;
    return ret;
  }
  Mod inverse() const { return pow(mod - 2); }
  friend ostream& operator << (ostream& os, const Mod& m) { return os << m.get(); }
  static void debug() {
    printf("%llu %llu %llu %llu\n", mod, inv, r2, omega().get());
  }
  word_t x;
};

const int size = 1 << 16;

#ifdef NTT64
  using m64_1 = ntt::Mod<709143768229478401, 31>;
  using m64_2 = ntt::Mod<711416664922521601, 19>; // <= 712e15 (sub.D = 3)
  m64_1 f1[size], g1[size];
  m64_2 f2[size], g2[size];
#else
  using m32_1 = ntt::Mod<138412033, 5>;
  using m32_2 = ntt::Mod<155189249, 6>;
  using m32_3 = ntt::Mod<163577857, 23>; // <=  16579e4 (sub.D = 3)
  m32_1 f1[size], g1[size];
  m32_2 f2[size], g2[size];
  m32_3 f3[size], g3[size];
#endif

template <typename mod_t>
void convolve(mod_t* A, int s1, mod_t* B, int s2, bool cyclic=false) {
  int s = (cyclic ? max(s1, s2) : s1 + s2 - 1);
  int size = 1;
  while (size < s) size <<= 1;
  mod_t roots[mod_t::level] = { mod_t::omega() };
  rep(i, 1, mod_t::level) roots[i] = roots[i - 1] * roots[i - 1];
  fill(A + s1, A + size, 0); ntt_dit4(A, size, 1, roots);
  if (A == B && s1 == s2) {
    rep(i, size) A[i] *= A[i];
  } else {
    fill(B + s2, B + size, 0); ntt_dit4(B, size, 1, roots);
    rep(i, size) A[i] *= B[i];
  }
  ntt_dit4(A, size, -1, roots);
  mod_t inv = mod_t(size).inverse();
  rep(i, cyclic ? size : s) A[i] *= inv;
}

template <typename mod_t>
void rev_permute(mod_t* A, int n) {
  int r = 0, nh = n >> 1;
  rep(i, 1, n) {
    for (int h = nh; !((r ^= h) & h); h >>= 1);
    if (r > i) swap(A[i], A[r]);
  }
}

template <typename mod_t>
void ntt_dit4(mod_t* A, int n, int sign, mod_t* roots) {
  rev_permute(A, n);
  int logn = __builtin_ctz(n);
  assert(logn <= mod_t::level);

  if (logn & 1) rep(i, 0, n, 2) {
    mod_t a = A[i], b = A[i + 1];
    A[i] = a + b; A[i + 1] = a - b;
  }
  mod_t imag = roots[mod_t::level - 2];
  if (sign < 0) imag = imag.inverse();

  mod_t one = mod_t(1);
  rep(e, 2 + (logn & 1), logn + 1, 2) {
    const int m = 1 << e;
    const int m4 = m >> 2;
    mod_t dw = roots[mod_t::level - e];
    if (sign < 0) dw = dw.inverse();

    const int block_size = min(n, max(m, (1 << 15) / int(sizeof(A[0]))));
    rep(k, 0, n, block_size) {
      mod_t w = one, w2 = one, w3 = one;
      rep(j, m4) {
        rep(i, k + j, k + block_size, m) {
          mod_t a0 = A[i + m4 * 0] * one, a2 = A[i + m4 * 1] * w2;
          mod_t a1 = A[i + m4 * 2] * w,   a3 = A[i + m4 * 3] * w3;
          mod_t t02 = a0 + a2, t13 = a1 + a3;
          A[i + m4 * 0] = t02 + t13; A[i + m4 * 2] = t02 - t13;
          t02 = a0 - a2, t13 = (a1 - a3) * imag;
          A[i + m4 * 1] = t02 + t13; A[i + m4 * 3] = t02 - t13;
        }
        w *= dw; w2 = w * w; w3 = w2 * w;
      }
    }
  }
}

} // namespace ntt

using R = int;
using R64 = i64;

class poly {
public:
#ifdef NTT64
  static const int ntt_threshold = 900; // deg(f * g)
#else
  static const int ntt_threshold = 1500; // deg(f * g)
#endif

  static R add_mod(R a, R b) { return int(a += b - mod) < 0 ? a + mod : a; }
  static R sub_mod(R a, R b) { return int(a -= b) < 0 ? a + mod : a; }
  static R64 sub_mul_mod(R64 a, R b, R c) {
    i64 t = i64(a) - i64(int(b)) * int(c);
    return t < 0 ? t + lmod : t;
  }
  static R mul_mod(R a, R b) { return R64(a) * b % fast_mod; }
  static R pow_mod(R a, int e) {
    R ret = 1 % fast_mod;
    for (; e; e >>= 1, a = mul_mod(a, a)) {
      if (e & 1) ret = mul_mod(ret, a);
    }
    return ret;
  }
  static R mod_inv(R a) {
    R b = mod, s = 1, t = 0;
    while (b > 0) {
      swap(s -= t * (a / b), t);
      swap(a %= b, b);
    }
    if (a > 1) { fprintf(stderr, "Error: invalid modular inverse\n"); exit(1); };
    return int(s) < 0 ? s + mod : s;
  }
  inline static void vec_add(R64* res, int s, const R* f, R c) {
    rep(i, s) res[i] = sub_mul_mod(res[i], mod - c, f[i]);
  }
  inline static void vec_sub(R64* res, int s, const R* f, R c) {
    rep(i, s) res[i] = sub_mul_mod(res[i], c, f[i]);
  }

#ifdef NTT64
  struct fast_div {
    using u128 = __uint128_t;
    fast_div() {}
    fast_div(u64 n) : m(n) {
      s = (n == 1) ? 0 : 127 - __builtin_clzll(n - 1);
      x = ((u128(1) << s) + n - 1) / n;
    }
    friend u64 operator / (u64 n, fast_div d) { return u128(n) * d.x >> d.s; }
    friend u64 operator % (u64 n, fast_div d) { return n - n / d * d.m; }
    u64 m, s, x;
  };
#else
  struct fast_div {
    fast_div() {}
    fast_div(u32 n) : m(n) {}
    friend u32 operator % (u64 n, fast_div d) { return n % d.m; }
    u32 m;
  };
#endif

public:
  poly() {}
  poly(int n) : coefs(n) {}
  poly(int n, int c) : coefs(n, c % mod) {}
  poly(const R* ar, int s) : coefs(ar, ar + s) {}
  poly(const vector<R>& v) : coefs(v) {}
  poly(const poly& f, int beg, int end=-1) {
    if (end < 0) end = beg, beg = 0;
    resize(end - beg);
    rep(i, beg, end) if (i < f.size()) coefs[i - beg] = f[i];
  }

  static int ilog2(u64 n) {
    return 63 - __builtin_clzll(n);
  }
  int size() const { return coefs.size(); }
  void resize(int s) { coefs.resize(s); }
  void push_back(R c) { coefs.push_back(c); }

  const R* data() const { return coefs.data(); }
  R* data() { return coefs.data(); }
  const R& operator [] (int i) const { return coefs[i]; }
  R& operator [] (int i) { return coefs[i]; }

  void reverse() { std::reverse(coefs.begin(), coefs.end()); }

  poly operator - () {
    poly ret = *this;
    rep(i, ret.size()) ret[i] = (ret[i] == 0 ? 0 : mod - ret[i]);
    return ret;
  }
  poly& operator += (const poly& rhs) {
    if (size() < rhs.size()) resize(rhs.size());
    rep(i, rhs.size()) coefs[i] = add_mod(coefs[i], rhs[i]);
    return *this;
  }
  poly& operator -= (const poly& rhs) {
    if (size() < rhs.size()) resize(rhs.size());
    rep(i, rhs.size()) coefs[i] = sub_mod(coefs[i], rhs[i]);
    return *this;
  }
  poly& operator *= (const poly& rhs) { return *this = *this * rhs; }

  poly& rev_add(const poly& rhs) {
    if (size() < rhs.size()) {
      int s = size();
      resize(rhs.size());
      rep(i, s) coefs[size() - 1 - i] = coefs[s - 1 - i];
      rep(i, size() - s) coefs[i] = 0;
    }
    rep(i, rhs.size()) coefs[size() - 1 - i] = \
      add_mod(coefs[size() - 1 - i], rhs.coefs[rhs.size() - 1 - i]);
    return *this;
  }

  poly operator + (const poly& rhs) const { return poly(*this) += rhs; }
  poly operator - (const poly& rhs) const { return poly(*this) -= rhs; }
  poly operator * (const poly& rhs) const { return this->mul(rhs); }

  static void set_mod(R m, int N=2) {
    mod = m;
    lmod = R64(m) << 32;
    N = max(2, N);
    fast_mod = fast_div(mod);
    invs.assign(N + 1, 1);
    facts.assign(N + 1, 1);
    ifacts.assign(N + 1, 1);
    invs[1] = 1;
    rep(i, 2, N + 1) {
      invs[i] = mul_mod(invs[mod % i], mod - mod / i);
      facts[i] = mul_mod(facts[i - 1], i);
      ifacts[i] = mul_mod(ifacts[i - 1], invs[i]);
    }
  }

private:

#ifdef NTT64
  static poly mul_crt(int beg, int end) {
    using namespace ntt;
    auto inv = m64_2(m64_1::modulus()).inverse();
    auto mod1 = m64_1::modulus() % fast_mod;
    poly ret(end - beg);
    rep(i, ret.size()) {
      u64 r1 = f1[i + beg].get(), r2 = f2[i + beg].get();
      ret[i] = (r1 + (m64_2(r2 + m64_2::modulus() - r1) * inv).get() % fast_mod * mod1) % fast_mod;
    }
    return ret;
  }

  static void mul2(const poly& f, const poly& g, bool cyclic=false) {
    using namespace ntt;
    if (&f == &g) {
      rep(i, f.size()) f1[i] = f[i];
      convolve(f1, f.size(), f1, f.size(), cyclic);
      rep(i, f.size()) f2[i] = f[i];
      convolve(f2, f.size(), f2, f.size(), cyclic);
    } else {
      rep(i, f.size()) f1[i] = f[i]; rep(i, g.size()) g1[i] = g[i];
      convolve(f1, f.size(), g1, g.size(), cyclic);
      rep(i, f.size()) f2[i] = f[i]; rep(i, g.size()) g2[i] = g[i];
      convolve(f2, f.size(), g2, g.size(), cyclic);
    }
  }
#else
  static poly mul_crt(int beg, int end) {
    using namespace ntt;
    auto m1 = m32_1::modulus();
    auto m2 = m32_2::modulus();
    auto m3 = m32_3::modulus();
    auto m12 = u64(m1) * m2;

    poly ret(end - beg);
    u32 m12m = m12 % mod;
    u32 inv1 = m32_2(m1).inverse().get();
    u32 inv12 = m32_3(m12 % m3).inverse().get();

    rep(i, ret.size()) {
      u32 r1 = f1[i + beg].get(), r2 = f2[i + beg].get(), r3 = f3[i + beg].get();
      u64 r = r1 + u64(r2 + m2 - r1) * inv1 % m2 * m1;
      ret[i] = (r + u64(r3 + m3 - r % m3) * inv12 % m3 * m12m) % mod;
    }
    return ret;
  }

  static void mul2(const poly& f, const poly& g, bool cyclic=false) {
    using namespace ntt;
    if (&f == &g) {
      rep(i, f.size()) f1[i] = f[i] % m32_1::modulus();
      convolve(f1, f.size(), f1, f.size(), cyclic);
      rep(i, f.size()) f2[i] = f[i] % m32_2::modulus();
      convolve(f2, f.size(), f2, f.size(), cyclic);
      rep(i, f.size()) f3[i] = f[i] % m32_3::modulus();
      convolve(f3, f.size(), f3, f.size(), cyclic);
    } else {
      rep(i, f.size()) f1[i] = f[i] % m32_1::modulus();
      rep(i, g.size()) g1[i] = g[i] % m32_1::modulus();
      convolve(f1, f.size(), g1, g.size(), cyclic);
      rep(i, f.size()) f2[i] = f[i] % m32_2::modulus();
      rep(i, g.size()) g2[i] = g[i] % m32_2::modulus();
      convolve(f2, f.size(), g2, g.size(), cyclic);
      rep(i, f.size()) f3[i] = f[i] % m32_3::modulus();
      rep(i, g.size()) g3[i] = g[i] % m32_3::modulus();
      convolve(f3, f.size(), g3, g.size(), cyclic);
    }
  }
#endif

public:
  static void amul(const R* f, int s1, const R* g, int s2, R* res) {
    int s = s1 + s2 - 1;
    tmp64.assign(s, 0);
    rep(i, s2) if (g[i]) vec_add(tmp64.data() + i, s1, f, g[i]);
    rep(i, s) res[i] = tmp64[i] % fast_mod;
  }

  poly mul_basecase(const poly& g) const {
    const auto& f = *this;
    int s = size() + g.size() - 1;
    poly ret(s);
    amul(f.data(), f.size(), g.data(), g.size(), ret.data());
    return ret;
  }

  // 1.0 * M(n)
  poly mul(const poly& g) const {
    const auto& f = *this;
    if (f.size() == 0 || g.size() == 0) return poly();
    if (f.size() + g.size() <= ntt_threshold) {
      return f.mul_basecase(g);
    } else {
      mul2(f, g, false);
      return mul_crt(0, f.size() + g.size() - 1);
    }
  }

  // 1.0 * M(n)
  poly middle_product(const poly& g) const {
    const poly& f = *this;
    if (f.size() == 0 || g.size() == 0) return poly();
    mul2(f, g, true);
    return mul_crt(f.size(), g.size());
  }

  void print() const {
    printf("[");
    if (size()) {
      printf("%u", coefs[0]);
      rep(i, 1, size()) printf(", %u", coefs[i]);
    }
    puts("]");
  }

public:
  vector<R> coefs;
  static vector<R> tmp32;
  static vector<R64> tmp64;
  static vector<R> invs, facts, ifacts;
  static R mod;
  static R64 lmod;
  static fast_div fast_mod;
};
R poly::mod;
R64 poly::lmod;
poly::fast_div poly::fast_mod;
vector<R> poly::tmp32;
vector<R64> poly::tmp64;
vector<R> poly::invs, poly::facts, poly::ifacts;

int pow_mod(int b, int e, int mod) {
  int ret = 1;
  for (; e; e >>= 1, b = i64(b) * b % mod) {
    if (e & 1) ret = i64(ret) * b % mod;
  }
  return ret;
}

int binomial_sum_mod_p(int N, int K, int mod) {
  if (K == 0) return 1 % mod;
  if (N <= K) return pow_mod(2, N, mod);
  if (i64(K) * 2 > N) {
    return (pow_mod(2, N, mod) + i64(mod) - binomial_sum_mod_p(N, N - K - 1, mod)) % mod;
  }
  assert(N < mod);

  const int sqrt_K = sqrt(K);
  poly::set_mod(mod, sqrt_K);

  auto mod_invs = [&] (vector<int>& f) {
    int n = f.size();
    vector<int> ret(f);
    if (n > 0) {
      rep(i, 1, n) ret[i] = i64(ret[i - 1]) * ret[i] % mod;
      int inv = poly::mod_inv(ret[n - 1]);
      for (int i = n - 1; i > 0; --i) {
        ret[i] = i64(ret[i - 1]) * inv % mod;
        inv = i64(inv) * f[i] % mod;
      }
      ret[0] = inv;
    }
    return ret;
  };

  auto conv = [&] (vector<int>& f) -> poly {
    int n = f.size();
    const auto& ifacts = poly::ifacts;
    auto g = poly(f);
    rep(i, n) {
      int d = i64(ifacts[i]) * ifacts[(n - 1) - i] % mod;
      if ((n - 1 - i) & 1) d = mod - d;
      g[i] = i64(g[i]) * d % mod;
    }
    return g;
  };

  auto shift = [&] (const poly& cf, const poly& f, i64 dx) {
    if ((dx %= mod) < 0) dx += mod;
    const int n = f.size();
    const int a = i64(dx) * poly::mod_inv(sqrt_K) % mod;

    auto g = poly(2 * n);
    rep(i, g.size()) g[i] = (i64(mod) + a + i - n) % mod;
    rep(i, g.size()) if (g[i] == 0) g[i] = 1;

    g.coefs = mod_invs(g.coefs);
    auto ret = cf.middle_product(g);
    int prod = 1;
    rep(i, n) prod = i64(prod) * (i64(mod) + a + n - 1 - i) % mod;
    for (int i = n - 1; i >= 0; --i) {
      ret[i] = i64(ret[i]) * prod % mod;
      prod = i64(prod) * g[n + i] % mod * (i64(mod) + a + i - n) % mod;
    }
    if (dx % sqrt_K == 0) {
      int k = n - dx / sqrt_K;
      rep(i, k) ret[i] = f[n - k + i];
    }
    return ret.coefs;
  };

  using Pair = pair< vector<int>, vector<int> >;
  function< Pair(int) > rec = [&] (int n) -> Pair {
    if (n == 1) {
      return Pair({N, N - sqrt_K}, {1, sqrt_K + 1});
    }
    int nh = n >> 1;
    auto res = rec(nh);
    auto& f11 = res.first, &g11 = res.second;

    auto f = conv(f11), g = conv(g11);
    auto g12 = shift(g, g11, nh);
    auto g21 = shift(g, g11, i64(sqrt_K) * nh);
    auto g22 = shift(g, g11, i64(sqrt_K) * nh + nh);

    auto f12 = shift(f, f11, N - nh * i64(sqrt_K + 2));
    auto f21 = shift(f, f11, i64(sqrt_K) * nh);
    auto f22 = shift(f, f11, N - i64(2) * nh * (sqrt_K + 1));
    rep(i, nh + 1) {
      g11[i] = (i64(g11[i]) * f12[nh - i] + i64(g12[i]) * f11[i]) % mod;
    }
    rep(i, 1, nh + 1) {
      g11.push_back( (i64(g21[i]) * f22[nh - i] + i64(g22[i]) * f21[i]) % mod );
    }

    f12 = shift(f, f11, nh);
    f22 = shift(f, f11, i64(sqrt_K) * nh + nh);

    rep(i, nh + 1) f11[i] = i64(f11[i]) * f12[i] % mod;
    rep(i, 1, nh + 1) f11.push_back(i64(f21[i]) * f22[i] % mod);

    if (n & 1) {
      rep(i, n) {
        g11[i] = (i64(g11[i]) + f11[i]) * (n + i64(i) * sqrt_K) % mod;
      }
      rep(i, n) {
        f11[i] = i64(f11[i]) * (i64(N) + mod - sqrt_K * i - n + 1) % mod;
      }
      vector<int> vals(n);
      rep(i, n) vals[i] = (i64(sqrt_K) * n + i + 1) % mod;
      if (i64(sqrt_K + 1) * n < mod) {
        int prod = 1;
        rep(i, n) prod = i64(prod) * vals[i] % mod;
        auto invs = mod_invs(vals);
        i64 s = 0;
        rep(i, n) {
          s += prod;
          prod = i64(prod) * invs[i] % mod * (i64(N) + mod - i64(sqrt_K) * n - i) % mod;
        }
        g11.push_back(s % mod);
        f11.push_back(prod);
      } else {
        g11.push_back(0);
        f11.push_back(0);
      }
    }
    return {f11, g11};
  };

  auto res = rec(sqrt_K);
  auto &f1 = res.first, &g1 = res.second;
  auto f2 = shift(conv(f1), f1, N - i64(sqrt_K) * (sqrt_K + 1));
  reverse(f2.begin(), f2.end());
  f2.resize(f2.size() - 1);
  f2 = mod_invs(f2);

  i64 ret = 0;
  rep(i, sqrt_K) {
    ret = (ret * f1[sqrt_K - 1 - i] + g1[sqrt_K - 1 - i]) % mod;
    ret = ret * f2[sqrt_K - 1 - i] % mod;
  }

  int prod = 1;
  rep(i, sqrt_K) {
    prod = i64(prod) * f1[i] % mod * f2[i] % mod;
  }

  const int rest = max(0, K - sqrt_K * sqrt_K);
  ret += prod;
  vector<int> invs(rest);
  rep(i, rest) invs[i] = i + 1 + sqrt_K * sqrt_K;
  invs = mod_invs(invs);
  rep(i, rest) {
    prod = i64(prod) * (N - sqrt_K * sqrt_K - i) % mod * invs[i] % mod;
    ret += prod;
  }
  ret %= mod;
  return ret;
}

// credit: Min_25
void solve() {
  const u32 p = u32(-1) >> 1; // 2^31
  printf("%d\n", binomial_sum_mod_p(2e9, 1e9, p));
}

int main() {
  //clock_t beg = clock();
  solve();
  //clock_t end = clock();
  //fprintf(stderr, "%.3f sec\n", double(end - beg) / CLOCKS_PER_SEC);
  return 0;
}
// https://discuss.codechef.com/t/how-to-calculate-sum-of-binomial-coefficients-efficiently/12846/4

