#include<bits/stdc++.h>
using namespace std;

const int N = 3e4 + 9;
int sgn(long long x) { return (x > 0) - (x < 0); }
struct P {
    long long x, y;
    P() { x = 0, y = 0; }
    P(long long x, long long y) : x(x), y(y) {}
    P(const P &p) : x(p.x), y(p.y)    {}
    P operator + (const P &a) const { return P(x + a.x, y + a.y); }
    P operator - (const P &a) const { return P(x - a.x, y - a.y); }
    bool operator == (P a) const { return sgn(a.x - x) == 0 && sgn(a.y - y) == 0; }
    bool operator < (P a) const { return sgn(a.x - x) == 0 ? y < a.y : x < a.x; }
    long long norm2() { return x * x + y * y; }
    long long cross(P p) const { return x * p.y - y * p.x; }
	long long cross(P a, P b) const { return (a - *this).cross(b - *this); }
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
P arb(LLONG_MAX,LLONG_MAX); // not equal to any other point
struct Quad {
    bool mark; Q o, rot; P p;
    P F() { return r() -> p; }
    Q r() { return rot -> rot; }
    Q prev() { return rot -> o -> rot; }
    Q next() { return r() -> prev();}
};
bool circ(P p, P a, P b, P c) { // is p in the circumcircle?
    i128 p2 = p.norm2(), A = a.norm2() - p2,
        B = b.norm2() - p2, C = c.norm2() - p2;
    return p.cross(a, b) * C + p.cross(b, c) * A + p.cross(c, a) * B > 0;
}
Q make_edge(P orig,  P dest) {
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
pair<Q,Q> rec(const vector<P>& s) {
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

vector<P> delaunay(vector<P> p) {
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


const double eps = 1e-9;
const double PI = acos(-1.0);
int sign(double x) { return (x > eps) - (x < -eps); }
struct PT {
    double x, y;
    PT() { x = 0, y = 0; }
    PT(double x, double y) : x(x), y(y) {}
    PT(const PT &p) : x(p.x), y(p.y)    {}
    PT operator + (const PT &a) const { return PT(x + a.x, y + a.y); }
    PT operator - (const PT &a) const { return PT(x - a.x, y - a.y); }
    PT operator * (const double a) const { return PT(x * a, y * a); }
    friend PT operator * (const double &a, const PT &b) { return PT(a * b.x, a * b.y); }
    PT operator / (const double a) const { return PT(x / a, y / a); }
    bool operator == (PT a) const { return sign(a.x - x) == 0 && sign(a.y - y) == 0; }
    bool operator != (PT a) const { return !(*this == a); }
    bool operator < (PT a) const { return sign(a.x - x) == 0 ? y < a.y : x < a.x; }
    bool operator > (PT a) const { return sign(a.x - x) == 0 ? y > a.y : x > a.x; }
    double norm() { return sqrt(x * x + y * y); }
    double norm2() { return x * x + y * y; }
    PT perp() { return PT(-y, x); }
    double arg() { return atan2(y, x); }
    PT truncate(double r) { // returns a vector with norm r and having same direction
        double k = norm();
        if (!sign(k)) return *this;
        r /= k;
        return PT(x * r, y * r);
    }
};
inline double dot(PT a, PT b) { return a.x * b.x + a.y * b.y; }
inline double dist2(PT a, PT b) { return dot(a - b, a - b); }
inline double dist(PT a, PT b) { return sqrt(dot(a - b, a - b)); }
inline double cross(PT a, PT b) { return a.x * b.y - a.y * b.x; }
inline int orientation(PT a, PT b, PT c) { return sign(cross(b - a, c - a)); }
PT perp(PT a) { return PT(-a.y, a.x); }
PT rotateccw90(PT a) { return PT(-a.y, a.x); }
PT rotatecw90(PT a) { return PT(a.y, -a.x); }
PT rotateccw(PT a, double t) { return PT(a.x * cos(t) - a.y * sin(t), a.x * sin(t) + a.y * cos(t)); }
PT rotatecw(PT a, double t) { return PT(a.x * cos(t) + a.y * sin(t), -a.x * sin(t) + a.y * cos(t)); }
double SQ(double x) { return x * x; }
double rad_to_deg(double r) { return (r * 180.0 / PI); }
double deg_to_rad(double d) { return (d * PI / 180.0); }
double get_angle(PT a, PT b) {
    double costheta = dot(a, b) / a.norm() / b.norm();
    return acos(max((double)-1.0, min((double)1.0, costheta)));
}
bool is_point_in_angle(PT b, PT a, PT c, PT p) { // does point p lie in angle <bac
    assert(orientation(a, b, c) != 0);
    if (orientation(a, c, b) < 0) swap(b, c);
    return orientation(a, c, p) >= 0 && orientation(a, b, p) <= 0;
}
bool half(PT p) {
    return p.y > 0.0 || (p.y == 0.0 && p.x < 0.0);
}
void polar_sort(vector<PT> &v) { // sort points in counterclockwise
    sort(v.begin(), v.end(), [](PT a,PT b) {
        return make_tuple(half(a), 0.0, a.norm2()) < make_tuple(half(b), cross(a, b), b.norm2());
    });
}
// minimum distance point from point c to segment ab that lies on segment ab
PT project_from_point_to_seg(PT a, PT b, PT c) {
    double r = dist2(a, b);
    if (fabs(r) < eps) return a;
    r = dot(c - a, b - a) / r;
    if (r < 0.L) return a;
    if (r > 1.L) return b;
    return a + (b - a) * r;
}
PT line_line_intersection(PT a, PT b, PT c, PT d) {
    double a1 = a.y - b.y, b1 = b.x - a.x, c1 = cross(a, b);
    double a2 = c.y - d.y, b2 = d.x - c.x, c2 = cross(c, d);
    double det = a1 * b2 - a2 * b1;
    return PT((b1 * c2 - b2 * c1) / det, (c1 * a2 - a1 * c2) / det);
}
PT circle(PT a, PT b, PT c) { // radius of the circumcircle
    b = (a + b) * 0.5;
    c = (a + c) * 0.5;
    auto p = line_line_intersection(b, b + rotatecw90(a - b), c, c + rotatecw90(a - c));
    return p;
}
// returns true if  point p is on line segment ab
bool is_point_on_seg(PT a, PT b, PT p) {
    if (fabs(cross(p - b, a - b)) < eps) {
        if (p.x < min(a.x, b.x) || p.x > max(a.x, b.x)) return false;
        if (p.y < min(a.y, b.y) || p.y > max(a.y, b.y)) return false;
        return true;
    }
    return false;
}
bool is_point_on_polygon(vector<PT> &p, const PT& z) {
    int n = p.size();
    for (int i = 0; i < n; i++) {
    	if (is_point_on_seg(p[i], p[(i + 1) % n], z)) return 1;
    }
    return 0;
}
// returns 1e9 if the point is on the polygon 
int winding_number(vector<PT> &p, const PT& z) { // O(n)
    if (is_point_on_polygon(p, z)) return 1e9;
    int n = p.size(), ans = 0;
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        bool below = p[i].y < z.y;
        if (below != (p[j].y < z.y)) {
            auto orient = orientation(z, p[j], p[i]);
            if (orient == 0) return 0;
            if (below == (orient > 0)) ans += below ? 1 : -1;
        }
    }
    return ans;
}
// -1 if strictly inside, 0 if on the polygon, 1 if strictly outside
int is_point_in_polygon(vector<PT> &p, const PT& z) { // O(n)
    int k = winding_number(p, z);
    return k == 1e9 ? 0 : k == 0 ? 1 : -1;
}
double ori(PT a, PT b, PT c) {
	return cross(b - a, c - a);
}
// intersection point between segment ab and segment cd assuming unique intersection exists
bool seg_seg_intersection(PT a, PT b, PT c, PT d, PT &ans) {
    double oa = ori(c, d, a), ob = ori(c, d, b);
    double oc = ori(a, b, c), od = ori(a, b, d);
    if (oa * ob < -eps && oc * od < -eps){
        ans = (a * ob - b * oa) / (ob - oa);
        return 1;
    }
    else return 0;
}
// projection point c onto line through a and b  assuming a != b
PT project_from_point_to_line(PT a, PT b, PT c) {
    return a + (b - a) * dot(c - a, b - a) / (b - a).norm2();
}
map<int, int> ccw[N];

double ans;
vector<PT> p;
double compute(PT a, vector<PT> &can) {
	double ret = 1e100;;
	for (auto x: can) {
		ret = min(ret, dist2(a, x));
	}
	return ret;
}
inline void try_voronoi_vertex(PT a, vector<PT> &can) {
	int k = is_point_in_polygon(p, a);
	if (k <= 0) {
		ans = max(ans, compute(a, can));
	} 
}
inline void try_voronoi_edge(PT a, PT b, vector<PT> &can) {
	if (a == b) return;
	int n = p.size();
	for (int i = 0; i < n; i++) {
		PT t;
		int k = seg_seg_intersection(p[i], p[(i + 1) % n], a, b, t);
		if (k) {
			ans = max(ans, compute(t, can));
		}
	}
}
inline void yo(PT a, vector<PT> &can) {
	for (int i = 0; i < p.size(); i++) {
		ans = max(ans, compute(project_from_point_to_seg(p[i], p[(i + 1) % p.size()], a), can));
	}
}
int32_t main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	int n; cin >> n;
	vector<P> tmp(n);
	p = vector<PT> (n);
	map<P, int> mp;
	for (int i = 0; i < n; i++) {
		cin >> tmp[i].x >> tmp[i].y;
		p[i].x = tmp[i].x; p[i].y = tmp[i].y;
		mp[tmp[i]] = i;
	}
	auto v = delaunay(tmp);
	for (int i = 0; i < v.size(); i += 3) {
		for (int j = 0; j < 3; j++) {
			ccw[mp[v[i + j]]][mp[v[i + (j + 1) % 3]]] = mp[v[i + (j + 2) % 3]];
		}
	} 
	for (int i = 0; i < n; i++) {
		auto nw = ccw[i];
		for (auto x: nw) {
			int u = i, v = x.first;
			int w = x.second;
			if (ccw[v].find(u) != ccw[v].end()) {
				int z = ccw[v][u];
				vector<PT> can({p[u], p[v], p[w], p[z]});
				PT c1 = circle(p[u], p[v], p[w]);
				PT c2 = circle(p[u], p[v], p[z]);
				try_voronoi_vertex(c1, can);
				try_voronoi_vertex(c2, can);
				try_voronoi_edge(c1, c2, can);
			}
			else {
				PT c1 = circle(p[u], p[v], p[w]);
				vector<PT> can = p;
				try_voronoi_vertex(c1, can);
				PT c2 = project_from_point_to_seg(p[u], p[v], c1);
				try_voronoi_edge(c1, c2, can);
				c2 = project_from_point_to_seg(p[v], p[w], c1);
				try_voronoi_edge(c1, c2, can);				
				c2 = project_from_point_to_seg(p[u], p[w], c1);
				try_voronoi_edge(c1, c2, can);
			}
		}
	}
	cout << fixed << setprecision(10) << sqrt(ans) << '\n';
    return 0;
}
// https://icpc.kattis.com/problems/pandapreserve
