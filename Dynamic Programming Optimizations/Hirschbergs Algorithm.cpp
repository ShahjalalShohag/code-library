#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <functional>
#include <string>
using namespace std;

template<class T>
void amin(T &a, T b) { if (a > b) a = b; }

// credit:  KokiYmgch
int main() {
        int h, w;
        scanf("%d%d", &h, &w);
        vector<int> u(h), v(w);
        for (int i = 0; i < h; i ++) scanf("%d", &u[i]);
        for (int i = 0; i < w; i ++) scanf("%d", &v[i]);
        function<long long (int, int)> get_val = [&](int i, int j) {
                return (long long) (u[i] + j + 1) ^ (long long) (v[j] + i + 1);
        };
        //Hirschberg
        vector<pair<int, int>> pos;
        function<void (int, int, int, int)> Hirschberg = [&](int li, int lj, int ri, int rj) {
                int mid = (lj + rj) / 2;
                int height = ri - li + 1;
                if (rj - lj < 1) return;
                if (height == 1) {
                        pos.emplace_back(mid, li);
                        Hirschberg(li, lj, li, mid);
                        Hirschberg(li, mid + 1, li, rj);
                        return;
                }
                //left
                int w_left = mid - lj + 1;
                vector<vector<long long>> dp(2, vector<long long> (height));
                dp[0][0] = get_val(li, lj);
                for (int i = 1; i < height; i ++) {
                        dp[0][i] = dp[0][i - 1] + get_val(li + i, lj);
                }
                bool f = 1;
                for (int j = 1; j < w_left; j ++) {
                        for (int i = 0; i < height; i ++) {
                                dp[f][i] = 1LL << 60;
                        }
                        for (int i = 0; i < height; i ++) {
                                int val = get_val(li + i, lj + j);
                                amin(dp[f][i], dp[!f][i] + val);
                                if (i - 1 >= 0) amin(dp[f][i], dp[f][i - 1] + val);
                        }
                        f = !f;
                }
                f = !f;
                vector<long long> m1(height);
                for (int i = 0; i < height; i ++) {
                        m1[i] = dp[f][i];
                }
                //right
                int w_right = rj - mid;
                dp[0][0] = get_val(ri, rj);
                for (int i = 1; i < height; i ++) {
                        dp[0][i] = dp[0][i - 1] + get_val(ri - i, rj);
                }
                f = 1;
                for (int j = 1; j < w_right; j ++) {
                        for (int i = 0; i < height; i ++) {
                                dp[f][i] = 1LL << 60;
                        }
                        for (int i = 0; i < height; i ++) {
                                long long val = get_val(ri - i, rj - j);
                                amin(dp[f][i], dp[!f][i] + val);
                                if (i - 1 >= 0) amin(dp[f][i], dp[f][i - 1] + val);
                        }
                        f = !f;
                }
                f = !f;
                vector<long long> m2(height);
                for (int i = 0; i < height; i ++) {
                        m2[height - i - 1] = dp[f][i];
                }
                //
                long long mi = 1LL << 60;
                int res = -1;
                for (int i = 0; i < height; i ++) {
                        long long sum = m1[i] + m2[i];
                        if (sum < mi) {
                                mi = sum;
                                res = i;
                        }
                }
                res += li;
                pos.emplace_back(mid, res);
                Hirschberg(li, lj, res, mid);
                Hirschberg(res, mid + 1, ri, rj);
        };
        Hirschberg(0, 0, h - 1, w - 1);
        //
        sort(pos.begin(), pos.end());
        int y = 0, x = 0;
        string ans = "";
        while (true) {
                if (x == w - 1) {
                        while (y != h - 1) {
                                ans += "D";
                                y ++;
                        }
                        break;
                }
                if (pos[x].second == y) {
                        x ++;
                        ans += "R";
                } else {
                        y ++;
                        ans += "D";
                }
        }
        cout << ans << endl;
        return 0;
}
// https://codeforces.com/blog/entry/57512