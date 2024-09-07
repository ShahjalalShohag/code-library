#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

using ll = long long;

// credit: unknown

int mod_inv(int a, int mod) {
  int b = mod, s = 1, t = 0, old_a = a;
  while (b) {
    int q = a / b;
    swap(a %= b, b);
    swap(s -= t * q, t);
  }
  if (a != 1) {
    fprintf(stderr,
        "Error: %d^{-1} mod %d does not exist.\n\n", old_a, mod);
    assert(0);
  }
  return s < 0 ? s + mod : s;
}

vector<int> extended(int n,
           const vector< vector<int> >& coeffs, const vector<int>& terms, int mod) {

  vector<int> ret(max<int>(n + 1, terms.size()));
  copy(terms.begin(), terms.end(), ret.begin());
  const int order = coeffs.size() - 1;
  const int deg = coeffs[0].size() - 1;
  assert((int) terms.size() >= order);
  for (int m = terms.size(); m <= n; ++m) {
    int s = 0;
    for (int i = 1; i <= order; ++i) {
      int k = m - i, t = ret[k];
      for (int d = 0; d <= deg; ++d) {
        s = (s + ll(t) * coeffs[i][d]) % mod;
        t = ll(t) * k % mod;
      }
    }
    int denom = 0, mpow = 1;
    for (int d = 0; d <= deg; ++d) {
      denom = (denom + ll(mpow) * coeffs[0][d]) % mod;
      mpow = ll(mpow) * m % mod;
    }
    ret[m] = ll(mod - s) * mod_inv(denom, mod) % mod;
  }
  return ret;
}

vector< vector<int> > find_recurrence_relation(
  const vector<int>& terms, const int deg, const int mod, bool verify=true) {

  const int n = terms.size();
  const int B = (n + 2) / (deg + 2); // number of blocks
  const int C = B * (deg + 1); // number of columns
  const int R = n - (B - 1); // number of rows
  assert(B >= 2);
  assert(R >= C - 1);

  auto mul = [mod] (int a, int b) {
    return ll(a) * b % mod;
  };
  auto fixed = [mod] (int a) {
    return (a %= mod) < 0 ? a + mod : a;
  };
  auto error = [] (int order, int deg) {
    fprintf(stderr,
        "Error: Could not find a recurrence relation "
        "of order <= %d and degree <= %d.\n\n",
        order, deg);
    assert(0);
  };

  vector< vector<int> > mat(R, vector<int>(C));
  for (int y = 0; y < R; ++y) {
    for (int b = 0; b < B; ++b) {
      for (int d = 0, v = fixed(terms[y + b]); d <= deg; ++d) {
        mat[y][b * (deg + 1) + d] = v;
        v = mul(v, y + b);
      }
    }
  }

  int rank = 0;
  for (int x = 0; x < C; ++x) {
    int pivot = -1;
    for (int y = rank; y < R; ++y) if (mat[y][x] != 0) {
        pivot = y;
        break;
      }
    if (pivot < 0) break;
    if (pivot != rank) swap(mat[rank], mat[pivot]);
    int inv = mod_inv(mat[rank][x], mod);
    for (int x2 = x; x2 < C; ++x2) mat[rank][x2] = mul(mat[rank][x2], inv);
    for (int y = rank + 1; y < R; ++y) if (mat[y][x]) {
        int c = mod - mat[y][x];
        for (int x2 = x; x2 < C; ++x2) {
          mat[y][x2] = (ll(mat[y][x2]) + ll(c) * mat[rank][x2]) % mod;
        }
      }
    ++rank;
  }

  if (rank == C) error(B - 1, deg);

  for (int y = rank - 1; y >= 0; --y) if (mat[y][rank]) {
      assert(mat[y][y] == 1);
      int c = mod - mat[y][rank];
      for (int y2 = 0; y2 < y; ++y2) {
        mat[y2][rank] = (mat[y2][rank] + ll(c) * mat[y2][y]) % mod;
      }
    }

  int order = rank / (deg + 1);
  vector< vector<int> > ret(order + 1, vector<int>(deg + 1));
  ret[0][rank % (deg + 1)] = 1;
  for (int y = rank - 1; y >= 0; --y) {
    int k = order - y / (deg + 1), d = y % (deg + 1);
    ret[k][d] = (mod - mat[y][rank]) % mod;
  }

  if (verify) {
    auto extended_terms = extended(n - 1, ret,
                     vector<int>(terms.begin(), terms.begin() + order), mod);
    for (int i = 0; i < (int) terms.size(); ++i) {
      if (fixed(terms[i] - extended_terms[i]) != 0) error(B - 1, deg);
    }
  }

  auto verbose = [&] {
    int last = verify ? n - 1 : order + R - 1;
    fprintf(stderr,
        "[ Found a recurrence relation ]\n"
        "- order %d\n"
        "- degree %d\n"
        "- verified up to a(%d) (number of non-trivial terms: %d)\n",
        order, deg, last, (last + 1) - ((deg + 2) * (order + 1) - 2)
         );
    fprintf(stderr, "{\n");
    for (int k = 0; k <= order; ++k) {
      fprintf(stderr, "  {");
      for (int d = 0; d <= deg; ++d) {
        if (d) fprintf(stderr, ", ");
        fprintf(stderr, "%d", ret[k][d] <= mod / 2 ? ret[k][d] : ret[k][d] - mod);
      }
      fprintf(stderr, "}%s\n", k == order ? "" : ",");
    }
    fprintf(stderr, "}\n\n");
  };
  verbose();

  return ret;
}

void show_extended_sequence(int n, const vector<int>& terms, int degree, int mod) {
  auto coeffs = find_recurrence_relation(terms, degree, mod);
  auto extended_terms = extended(n, coeffs, terms, mod);
  for (int i = 0; i < (int) extended_terms.size(); ++i) {
    printf("%d %d\n", i, extended_terms[i]);
  }
  puts("");
}

// transition -> for(i = 0; i < k; i++) f[n] += tr[i] * f[n-i-1]
// here tr[i] is a polynomial. order = k and degree = degree of tr[i]
// given first few values of f, it will extract the recurrence
int main() {
  const int mod = 1e9 + 7;

  // Ones
  //show_extended_sequence(10, {1, 1, 1, 1, 1}, 0, mod);

  // Factorials
  // show_extended_sequence(10, {1, 2, 6, 24, 120}, 1, mod);

  // Catalan numbers
   show_extended_sequence(10, {1, 1, 2, 5, 14, 42}, 1, mod);

  // Subfactorials
  // show_extended_sequence(10, {1, 0, 1, 2, 9, 44, 265}, 1, mod);

  // Motzkin numbers
  // show_extended_sequence(10, {1, 1, 2, 4, 9, 21, 51}, 1, mod);

  // Large Schröder numbers
  // show_extended_sequence(10, {1, 2, 6, 22, 90, 394, 1806}, 1, mod);

  // Error: (n + 1) a_n \equiv 0 (mod 2)
  // show_extended_sequence(10, {0, 1, 0, 1, 0, 1, 0, 1, 0, 1}, 1, 2);

  // Hertzsprung's problem: order 4, degree 1
  // show_extended_sequence(20, {
  //   1, 1, 0, 0, 2,
  //   14, 90, 646, 5242, 47622,
  //   479306, 5296790, 63779034, 831283558, 661506141
  // }, 1, mod);

  // yukicoder No.93: order 13, degree 2
  // show_extended_sequence(100, {
  //   1, 1, 2, 2, 8,
  //   28, 152, 952, 7208, 62296,
  //   605864, 6522952, 76951496, 986411272, 647501133,
  //   653303042, 170637030, 248109503, 700583494, 619914523,
  //   682935856, 443753916, 423068688, 507501942, 315541972,
  //   110825117, 848156395, 798418282, 920964362, 23823302,
  //   114894774, 279365223, 992413784, 833179437, 785518302,
  //   524368220, 42214454, 140345871, 188150268, 808714798,
  //   718376249, 732000901, 955005007, 139255097, 484615744,
  //   615066955, 726914809, 856989248, 460819998, 321277105,
  //   536397091, 555447300, 597473569, 217709372, 24981477,
  //   143561526, 171000806, 137649694, 749333590, 700935246,
  //   916763337, 762367836, 296796066, 236278263, 398507715,
  // }, 2, mod);

  // binom(3 * i, i) ^ 2 + binom(2 * i, i + 1): order 8, degree 5
  // show_extended_sequence(128, {
  //   1, 10, 229, 7071, 245081,
  //   9018219, 344622888, 521041312, 917599501, 328470621,
  //   920199271, 726809819, 712906773, 531692419, 688496750,
  //   140388924, 514070772, 712606107, 333670208, 549905369,
  //   504023887, 34217948, 890190161, 703909490, 6403597,
  //   623962638, 685637246, 126160387, 956873888, 9766247,
  //   864866393, 563563889, 613532405, 710746029, 182520210,
  //   914377932, 648461424, 715143730, 918800735, 503145605,
  //   27402642, 282029583, 635728688, 91880493, 896737996,
  //   773282006, 625726102, 992524580, 494071629, 82874383,
  //   536460288, 218839718, 406647024, 248185000, 360613817,
  //   546217158, 925224608, 482921337, 928327434, 372559325,
  //   614987117, 601351833, 765504201, 230666863, 98348380,
  // }, 5, mod);
  return 0;
}
// https://min-25.hatenablog.com/entry/2018/05/10/212805
