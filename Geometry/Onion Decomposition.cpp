#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

struct PT {
    long long x,y;
    PT operator - (PT p) const { return {x - p.x, y- p.y}; }
    long long cross(PT p) const { return x * p.y- y * p.x; }
    long long dot(PT p) const { return x * p.x + y * p.y; }
    bool operator < (PT p) const { if (y != p.y) return y < p.y; return x < p.x; }
    bool operator == (PT p) const { return x == p.x && y == p.y; }
    PT operator - () const { return {-x, -y}; }
};
long long cross(PT a, PT b, PT c) {
    return (b - a).cross(c - a);
}
// Decremental Convex Hull
// it only supports efficient (O(log^2n)) deletion of points, 
// which is enough to compute nested convex hulls in O(nlog^2n)
// it assumes that all points are distinct
struct LeftHull {
    vector<PT> ps;
    struct node {
        int bl, br;
        int L, R;
        int lc, rc;
    };
    vector<node> nodes;
    int root;
    bool isleaf(int w) {
        return nodes[w].lc == -1 && nodes[w].rc == -1;
    }
    void pull(int w) {
        assert(!isleaf(w));
        int l = nodes[w].lc, r = nodes[w].rc;
        long long split_y = ps[nodes[r].L].y;
        while(!isleaf(l) || !isleaf(r)) {
            int a = nodes[l].bl, b = nodes[l].br, 
                c = nodes[r].bl, d = nodes[r].br;
            if (a != b  &&  cross(ps[a], ps[b], ps[c]) > 0) {
                l = nodes[l].lc;
            } 
            else if (c != d  &&  cross(ps[b], ps[c], ps[d]) > 0) {
                r = nodes[r].rc;
            } 
            else if (a == b) {
                r = nodes[r].lc;
            } 
            else if (c == d) {
                l = nodes[l].rc;
            } 
            else {
                long long s1 = cross(ps[a], ps[b], ps[c]);
                long long s2 = cross(ps[b], ps[a], ps[d]);
                assert(s1 + s2 >= 0);
                if (s1 + s2  ==  0 || s1 * ps[d].y + s2 * ps[c].y<split_y * (s1 + s2)) {
                    l = nodes[l].rc;
                } else {
                    r = nodes[r].lc;
                }
            }
        }
        nodes[w].bl = nodes[l].L;
        nodes[w].br = nodes[r].L;
    }
    void build(int w, int L, int R) {
        nodes[w].L = L;
        nodes[w].R = R;
        if (R - L  ==  1) {
            nodes[w].lc = nodes[w].rc = -1;
            nodes[w].bl = nodes[w].br = L;
        } else {
            int M = (L + R) / 2;
            nodes[w].lc = w + 1;
            nodes[w].rc = w + 2 * (M-L);
            build(nodes[w].lc, L, M);
            build(nodes[w].rc, M, R);
            pull(w);
        }
    }
    int erase(int w, int L, int R) {
        if (R <= nodes[w].L || L >= nodes[w].R) return w;
        if (L <= nodes[w].L && R >= nodes[w].R) return -1;
        nodes[w].lc = erase(nodes[w].lc, L, R);
        nodes[w].rc = erase(nodes[w].rc, L, R);
        if (nodes[w].lc  ==  -1) return nodes[w].rc;
        if (nodes[w].rc  ==  -1) return nodes[w].lc;
        pull(w);
        return w;
    }
    //only works for whole hull
    void get_hull(int w, int l, int r, vector<int>& res) {
        if (isleaf(w)) {
            res.push_back(nodes[w].L);
        } 
        else if (r <= nodes[w].bl) {
            get_hull(nodes[w].lc, l, r, res);
        } 
        else if (l >= nodes[w].br) {
            get_hull(nodes[w].rc, l, r, res);
        } 
        else {
            assert(l <= nodes[w].bl && nodes[w].br <= r);
            get_hull(nodes[w].lc, l, nodes[w].bl, res);
            get_hull(nodes[w].rc, nodes[w].br, r, res);
        }
    }
    LeftHull(const vector<PT>& ps): ps(ps), nodes(ps.size() * 2), root(0) {
        build(0, 0, ps.size());
    }
    vector<int> get_hull() {
        if (root == -1) return {};
        vector<int> res;
        get_hull(root, 0, ps.size() - 1, res);
        return res;
    }
    void erase(int L) {
        root = erase(root, L, L + 1);
    }
};

vector<PT> pts;
map<PT, int> id;
int layer[N], ans[N];
int main() {
    int n; scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        int x, y; scanf("%d %d", &x, &y);
        pts.push_back({x, y});
        id[{x, y}] = i;
    }
    sort(pts.begin(), pts.end());
    LeftHull left(pts);
    reverse(pts.begin(), pts.end());
    for (auto& p:pts) p = -p;
    LeftHull right(pts);
    for (auto& p:pts) p = -p;
    reverse(pts.begin(), pts.end());
    for (int id = 1, cnt = 0; cnt < n; id++) {
        set<int> hull;
        for (int i: left.get_hull()) hull.insert(i);
        for (int i: right.get_hull()) hull.insert(n - 1 - i);
        for (int i: hull) {
            assert(!layer[i]);
            cnt++; layer[i] = id;
            left.erase(i);
            right.erase(n - 1 - i);
        }
    }
    for (int i = 0; i < n; i++) ans[id[pts[i]]] = layer[i];
    for (int i = 0; i < n; i++) printf("%d\n", ans[i]);
}
// https://codeforces.com/blog/entry/75929
