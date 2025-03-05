#include <bits/stdc++.h>
using namespace std;

#define int long long

struct Factorizer
{
    //    Factorizer factorizer(prec_n, sp_bound, rng_seed);
    //    prec_n is the bound for sieve (inclusive)
    //    all numbers will first be checked on primes <= sp_bound (if prec_n >= sp_bound)
    //    factorization for one number ~1e18 takes ~13ms

    vector<int> min_prime;
    vector<int> primes;
    int prec_n;
    int sp_bound;

    Factorizer(int prec_n = 100, int sp_bound = 100, int64_t rng_seed = -1) : prec_n(max(prec_n, 3ll)),
                                                                              sp_bound(sp_bound),
                                                                              rng(rng_seed != -1 ? rng_seed : chrono::steady_clock::now().time_since_epoch().count())
    {
        min_prime.assign(prec_n + 1, -1);
        for (int i = 2; i <= prec_n; ++i)
        {
            if (min_prime[i] == -1)
            {
                min_prime[i] = i;
                primes.push_back(i);
            }
            int k = min_prime[i];
            for (int j : primes)
            {
                if (j * i > prec_n)
                    break;
                min_prime[i * j] = j;
                if (j == k)
                    break;
            }
        }
    }

    bool is_prime(int64_t n, bool check_small = true)
    {
        if (n <= prec_n)
            return min_prime[n] == n;

        if (check_small)
        {
            for (int p : primes)
            {
                if (p > sp_bound || (int64_t)p * p > n)
                    break;
                if (n % p == 0)
                    return false;
            }
        }

        int s = 0;
        int64_t d = n - 1;
        while (d % 2 == 0)
        {
            ++s;
            d >>= 1;
        }
        for (int64_t a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022})
        {
            if (a >= n)
                break;
            int64_t x = mpow_long(a, d, n);
            if (x == 1 || x == n - 1)
                continue;
            bool composite = true;
            for (int i = 0; i < s - 1; ++i)
            {
                x = mul_mod(x, x, n);
                if (x == 1)
                    return false;
                if (x == n - 1)
                {
                    composite = false;
                    break;
                }
            }
            if (composite)
                return false;
        }
        return true;
    }

    vector<pair<int64_t, int>> factorize(int64_t n, bool check_small = true)
    {
        vector<pair<int64_t, int>> res;
        if (check_small)
        {
            for (int p : primes)
            {
                if (p > sp_bound)
                    break;
                if ((int64_t)p * p > n)
                    break;
                if (n % p == 0)
                {
                    res.emplace_back(p, 0);
                    while (n % p == 0)
                    {
                        n /= p;
                        res.back().second++;
                    }
                }
            }
        }

        if (n == 1)
            return res;
        if (is_prime(n, false))
        {
            res.emplace_back(n, 1);
            return res;
        }

        if (n <= prec_n)
        {
            while (n != 1)
            {
                int d = min_prime[n];
                if (res.empty() || res.back().first != d)
                    res.emplace_back(d, 0);
                res.back().second++;
                n /= d;
            }
            return res;
        }

        int64_t d = get_divisor(n);
        auto a = factorize(d, false);
        for (auto &[div, cnt] : a)
        {
            cnt = 0;
            while (n % div == 0)
            {
                n /= div;
                ++cnt;
            }
        }
        auto b = factorize(n, false);

        int ia = 0, ib = 0;
        while (ia < a.size() || ib < b.size())
        {
            bool choosea;
            if (ia == a.size())
                choosea = false;
            else if (ib == b.size())
                choosea = true;
            else if (a[ia].first <= b[ib].first)
                choosea = true;
            else
                choosea = false;

            res.push_back(choosea ? a[ia++] : b[ib++]);
        }
        return res;
    }

private:
    mt19937_64 rng;
    int64_t rnd(int64_t l, int64_t r)
    {
        return uniform_int_distribution<int64_t>(l, r)(rng);
    }

    int64_t mpow_long(int64_t a, int64_t p, int64_t mod)
    {
        int64_t res = 1;
        while (p)
        {
            if (p & 1)
                res = mul_mod(res, a, mod);
            p >>= 1;
            a = mul_mod(a, a, mod);
        }
        return res;
    }

    int64_t mul_mod(int64_t a, int64_t b, int64_t mod)
    {
        int64_t res = a * b - mod * (int64_t)((long double)1 / mod * a * b);
        if (res < 0)
            res += mod;
        if (res >= mod)
            res -= mod;
        return res;
    }

    int64_t get_divisor(int64_t n)
    {
        auto f = [&](int64_t x) -> int64_t
        {
            int64_t res = mul_mod(x, x, n) + 1;
            if (res == n)
                res = 0;
            return res;
        };

        while (true)
        {
            int64_t x = rnd(1, n - 1);
            int64_t y = f(x);
            while (x != y)
            {
                int64_t d = gcd(n, abs(x - y));
                if (d == 0)
                    break;
                else if (d != 1)
                    return d;
                x = f(x);
                y = f(f(y));
            }
        }
    }
};
// https://github.com/maksim1744/programming-library/blob/master/factorizer.cpp
signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    Factorizer factorizer(1e6, 1e4);
    int n;
    cin >> n;
    auto res = factorizer.factorize(n);
    for (auto [p, k] : res)
    {
        cout << p << "^" << k << " ";
    }
    cout << '\n';
    return 0;
}
