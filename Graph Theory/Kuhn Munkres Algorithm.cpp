#include<bits/stdc++.h>
using namespace std;

const int N = 405;

// Complexity: O(n^3) but optimized
// It finds maximum cost matching, not necessarily with maximum matching
// 1-indexed
struct KuhnMunkres {
    const long long inf = 1LL << 60;
    long long G[N][N], hl[N], hr[N], dt[N];
    int l[N], r[N], vl[N], vr[N], pre[N], q[N], ql, qr, n;
   	KuhnMunkres() {}
    KuhnMunkres(int nl, int nr) {
        n = max(nl, nr);
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= n; ++j) G[i][j] = 0;
        }
    }
    void add_edge(int u, int v, long long w) {
        G[u][v] = max(G[u][v], w);
    }
    int check(int i) {
        if (vl[i] = 1, l[i] != -1) return vr[q[qr++] = l[i]] = 1;
        while (i != -1) swap(i, r[l[i] = pre[i]]);
        return 0;
    }
    void bfs(int s) {
        for (int i = 1; i <= n; ++i) vl[i] = vr[i] = 0, dt[i] = inf;
        for (vr[q[ql = 0] = s] = qr = 1;;) {
            for (long long d; ql < qr;) {
                for (int i = 1, j = q[ql++]; i <= n; ++i) {
                    if (!vl[i] && dt[i] >= (d = hl[i] + hr[j] - G[i][j])) {
                        if (pre[i] = j, d) dt[i] = d;
                        else if (!check(i)) return;
                    }
                }
            }
            long long d = inf;
            for (int i = 1; i <= n; ++i) {
                if (!vl[i] && d > dt[i]) d = dt[i];
            }
            for (int i = 1; i <= n; ++i) {
                if (vl[i]) hl[i] += d;
                else dt[i] -= d;
                if (vr[i]) hr[i] -= d;
            }
            for (int i = 1; i <= n; ++i) {
                if (!vl[i] && !dt[i] && !check(i)) return;
            }
        }
    }
    long long solve() {
        for (int i = 1; i <= n; ++i) l[i] = r[i] = -1, hr[i] = 0;
        for (int i = 1; i <= n; ++i) hl[i] = *max_element(G[i] + 1, G[i] + 1 + n);
        for (int i = 1; i <= n; ++i) bfs(i);
        long long ans = 0;
        for (int i = 1; i <= n; ++i) {
        	if (G[i][l[i]]) ans += G[i][l[i]];
            else l[i] = 0;
        }
        return ans;
    }
};
int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
 	int n1, n2, m; cin >> n1 >> n2 >> m;
	KuhnMunkres M(n1, n2);
    for (int i = 1; i <= m; i++) {
        int u, v, w; cin >> u >> v >> w;
        M.add_edge(u, v, w);
    }
    cout << M.solve() << '\n';
    for (int i = 1; i <= n1; i++) cout << M.l[i] << ' ';
    return 0;
}
