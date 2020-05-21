#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

int sign(long long x) { return (x > 0) - (x < 0); }
struct PT {
    long long x, y;
    PT() { x = 0, y = 0; }
    PT(long long x, long long y) : x(x), y(y) {}
    PT(const PT &p) : x(p.x), y(p.y)    {}
    PT operator + (const PT &a) const { return PT(x + a.x, y + a.y); }
    PT operator - (const PT &a) const { return PT(x - a.x, y - a.y); }
    bool operator == (PT a) const { return sign(a.x - x) == 0 && sign(a.y - y) == 0; }
    bool operator < (PT a) const { return sign(a.x - x) == 0 ? y < a.y : x < a.x; }
    long long norm2() { return x * x + y * y; }
    long long cross(PT p) const { return x * p.y - y * p.x; }
	long long cross(PT a, PT b) const { return (a - *this).cross(b - *this); }
};
// Each circumcircle contains none of the input points.
// There must be no duplicate points.
// If all points are on a line, no triangles will be returned.
// Should work for doubles as well, though there may be precision issues in 'circ'.
// returns a vector v where v[i], v[i + 1], v[i + 2] form a triangulation in ccw order
// such that i % 3 == 0
// O(n log n) in the worst case
typedef struct Quad* Q;
typedef __int128_t i128; // (can be ll if coords are < 2e4)
PT arb(LLONG_MAX,LLONG_MAX); // not equal to any other point
struct Quad {
    bool mark; Q o, rot; PT p;
    PT F() { return r() -> p; }
    Q r() { return rot -> rot; }
    Q prev() { return rot -> o -> rot; }
    Q next() { return r() -> prev();}
};
bool circ(PT p, PT a, PT b, PT c) { // is p in the circumcircle?
    i128 p2 = p.norm2(), A = a.norm2() - p2,
        B = b.norm2() - p2, C = c.norm2() - p2;
    return p.cross(a, b) * C + p.cross(b, c) * A + p.cross(c, a) * B > 0;
}
Q make_edge(PT orig,  PT dest) {
    Q q[] = {new Quad{0, 0, 0, orig}, new Quad{0, 0, 0, arb}, 
        new Quad{0, 0, 0, dest}, new Quad{0, 0, 0, arb}
    };
    for (int i = 0; i < 4; i++)
    q[i] -> o = q[-i & 3], q[i] -> rot = q[(i+1) & 3];
    return *q;
}
void splice(Q a, Q b) {
    swap(a -> o -> rot -> o, b -> o -> rot -> o);
    swap(a -> o, b -> o);
}
Q connect(Q a, Q b) {
    Q q = make_edge(a -> F(), b -> p);
    splice(q, a -> next());
    splice(q -> r(), b);
    return q;
}
pair<Q,Q> rec(const vector<PT>& s) {
    if (s.size() <= 3) {
        Q a = make_edge(s[0], s[1]), b = make_edge(s[1], s.back());
        if (s.size() == 2) return {a, a -> r()};
        splice(a -> r(), b);
        auto side = s[0].cross(s[1], s[2]);
        Q c = side ? connect(b, a) : 0;
        return {side < 0 ? c -> r() : a, side < 0 ? c : b -> r() };
    }
	#define H(e) e -> F(), e -> p
	#define valid(e) (e -> F().cross(H(base)) > 0)
    Q A, B, ra, rb;
    int half = s.size() / 2;
    tie(ra, A) = rec({s.begin(), s.end() - half});
    tie(B, rb) = rec({s.size() - half + s.begin(), s.end()});
    while ((B -> p.cross(H(A)) < 0 && (A = A -> next())) ||
            (A -> p.cross(H(B)) > 0 && (B = B -> r() -> o)));
    Q base = connect(B -> r(), A);
    if (A -> p == ra -> p) ra = base -> r();
    if (B -> p == rb -> p) rb = base;
	#define del(e, init, dir) Q e = init -> dir; if (valid(e)) \
		while (circ(e -> dir -> F(), H(base), e -> F())) { \
			Q t = e -> dir; \
			splice(e, e -> prev()); \
			splice(e -> r(), e -> r() -> prev()); \
			e = t; \
		}
    for (;;) {
        del(lc, base -> r(), o);
        del(rc, base, prev());
        if (!valid(lc) && !valid(rc)) break;
        if (!valid(lc) || (valid(rc) && circ(H(rc), H(lc)))) base = connect(rc, base -> r());
        else base = connect(base -> r(), lc -> r());
    }
    return {ra, rb};
}

vector<PT> delaunay(vector<PT> p) {
    sort(p.begin(), p.end());
    assert(unique(p.begin(), p.end()) == p.end());
    if (p.size() < 2) return {};
    Q e = rec(p).first;
    vector<Q> q = {e};
    int qi = 0;
    while (e -> o -> F().cross(e -> F(), e -> p) < 0) e = e -> o;
	#define ADD { Q c = e; do { c -> mark = 1; p.push_back(c -> p); \
	q.push_back(c -> r()); c = c -> next(); } while (c != e); }
    ADD;
    p.clear();
    while (qi < q.size()) if (!(e = q[qi++]) -> mark) ADD;
    return p;
}
struct dPT {
    double x, y;
    dPT() { x = 0, y = 0; }
    dPT(double x, double y) : x(x), y(y) {}
    dPT(const dPT &p) : x(p.x), y(p.y)    {}
    dPT operator + (const dPT &a) const { return dPT(x + a.x, y + a.y); }
    dPT operator - (const dPT &a) const { return dPT(x - a.x, y - a.y); }
    dPT operator * (const double a) const { return dPT(x * a, y * a); }
    double norm2() { return x * x + y * y; }
    double cross(dPT p) const { return x * p.y - y * p.x; }
	double cross(dPT a, dPT b) const { return (a - *this).cross(b - *this); }
};
inline double dot(dPT a, dPT b) { return a.x * b.x + a.y * b.y; }
inline double dist2(dPT a, dPT b) { return dot(a - b, a - b); }
inline double dist(dPT a, dPT b) { return sqrt(dot(a - b, a - b)); }
inline double cross(dPT a, dPT b) { return a.x * b.y - a.y * b.x; }
dPT rotatecw90(dPT a) { return dPT(a.y, -a.x); }
dPT line_line_intersection(dPT a, dPT b, dPT c, dPT d) {
    double a1 = a.y - b.y, b1 = b.x - a.x, c1 = a.cross(b);
    double a2 = c.y - d.y, b2 = d.x - c.x, c2 = c.cross(d);
    double det = a1 * b2 - a2 * b1;
    return dPT((b1 * c2 - b2 * c1) / det, (c1 * a2 - a1 * c2) / det);
}
double circle(PT A, PT B, PT C) { // radius of the circumcircle
    dPT a(A.x, A.y), b(B.x, B.y), c(C.x, C.y);
    b = (a + b) * 0.5;
    c = (a + c) * 0.5;
    auto p = line_line_intersection(b, b + rotatecw90(a - b), c, c + rotatecw90(a - c));
    return dist(a, p);
}
int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n; cin >> n;
    vector<PT> p(n);
    for (int i = 0; i < n; i++) {
    	cin >> p[i].x >> p[i].y;
    }
    auto ans = delaunay(p);
    assert(ans.size() % 3 == 0);
    double res = 0;
    for (int i = 0; i < ans.size(); i += 3) {
    	res = max(res, circle(ans[i], ans[i + 1], ans[i + 2]));
    }
    cout << fixed << setprecision(10) << res << '\n';
    return 0;
}
// https://dmoj.ca/problem/cco08p6