#include<bits/stdc++.h>
using namespace std;
 
const int N = 1e5 + 9;
 
struct Line {
    int x, ymin, ymax, ty ;
    Line() {}
    Line(int _x, int _ym, int _ymx, int _ty) { x = _x ; ymin = _ym ; ymax = _ymx ; ty = _ty ; }
    bool operator < (const Line &e) const { return x < e.x ; }
};
Line all[N * 2];
long long t[N * 16], mp[N * 2], lazy[N * 16];
inline void push(int n, int b, int e) {
    if (!lazy[n]) return;
    t[n] = mp[e] - mp[b] - t[n];
    if (b != e) {
        lazy[n << 1] ^= lazy[n];
        lazy[n << 1 | 1] ^= lazy[n];
    }
    lazy[n] = 0;
}
void upd(int n, int b, int e, int i, int j, int ty) {
    push(n, b, e);
    if (mp[e] < i || mp[b] > j) return;
    if (mp[b] >= i && mp[e] <= j) {
        lazy[n] ^= ty;
        push(n, b, e);
        return;
    }
    if (b + 1 >= e) return;
    int mid = b + e >> 1, l = n << 1, r = l | 1;
    upd(l, b, mid, i, j, ty);
    upd(r, mid, e, i, j, ty);
    t[n] = t[l] + t[r];
}
int main() {
    int n; cin >> n;
    int id = 0, m = 0 ;
    for (int i = 0; i < n; i++) {
        int x1, y1, x2, y2; scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
        assert(x1 < x2); assert(y1 < y2);
        all[id++] = Line(x1, y1, y2, 1);
        all[id++] = Line(x2, y1, y2, 1);
        mp[++m] = y1 ; mp[++m] = y2 ;
    }
    n = id;
    sort(all, all + n);
    sort(mp + 1, mp + m + 1);
    m = unique(mp + 1, mp + m + 1) - mp - 1;
    upd(1, 1, m, all[0].ymin, all[0].ymax, all[0].ty);
    long long ans = 0 ; // sum of area covered by odd number of rectangles
    for (int i = 1; i < n; i++) {
        ans += 1LL * t[1] * (all[i].x - all[i - 1].x);
        upd(1, 1, m, all[i].ymin, all[i].ymax, all[i].ty);
    }
    cout << ans << '\n';
    return 0;
}
// https://vjudge.net/problem/Gym-101982F
