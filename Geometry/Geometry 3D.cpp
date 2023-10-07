#include<bits/stdc++.h>
using namespace std;

// https://victorlecomte.com/cp-geo.pdf
const double inf = 1e100;
const double eps = 1e-9;
const double PI = acos((double)-1.0);
int sign(double x) { return (x > eps) - (x < -eps); }

struct PT {
    double x, y;
    PT() { x = 0, y = 0; }
    PT(double x, double y) : x(x), y(y) {}
    PT(const PT &p) : x(p.x), y(p.y)    {}
    void scan() { cin >> x >> y; }
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
struct p3 {
    double x, y, z;
    p3() { x = 0, y = 0; z = 0; }
    p3(double x, double y, double z) : x(x), y(y), z(z) {}
    p3(const p3 &p) : x(p.x), y(p.y), z(p.z)    {}
    void scan() { cin >> x >> y >> z; }
    p3 operator + (const p3 &a) const { return p3(x + a.x, y + a.y, z + a.z); }
    p3 operator - (const p3 &a) const { return p3(x - a.x, y - a.y, z - a.z); }
    p3 operator * (const double a) const { return p3(x * a, y * a, z * a); }
    friend p3 operator * (const double &a, const p3 &b) { return p3(a * b.x, a * b.y, a * b.z); }
    p3 operator / (const double a) const { return p3(x / a, y / a, z / a); }
    bool operator == (p3 a) const { return sign(a.x - x) == 0 && sign(a.y - y) == 0 && sign(a.z - z) == 0; }
    bool operator != (p3 a) const { return !(*this == a); }
    double abs() { return sqrt(x * x + y * y + z * z); }
    double sq() { return x * x + y * y + z * z; }
    p3 unit() { return *this / abs(); }
}zero(0, 0, 0);
double operator | (p3 v, p3 w) { //dot product
    return v.x * w.x + v.y * w.y + v.z * w.z;
}
p3 operator * (p3 v, p3 w) { //cross product
    return {v.y * w.z - v.z * w.y, v.z * w.x - v.x * w.z, v.x * w.y - v.y * w.x};
}
double sq(p3 v) { return v | v; }
double abs(p3 v) { return sqrt(sq(v)); }
p3 unit(p3 v) { return v / abs(v); }
inline double dot(p3 a, p3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline double dist2(p3 a, p3 b) { return dot(a - b, a - b); }
inline double dist(p3 a, p3 b) { return sqrt(dot(a - b, a - b)); }
inline p3 cross(p3 a, p3 b) { return p3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
// if s is on the same side of the plane pqr as the vector pq * pr then it will be positive
// otherwise negative or 0 if on the plane
double orient(p3 p, p3 q, p3 r, p3 s) { return (q - p) * (r - p) | (s - p); }
// returns orientation of p to q to r on the plane perpendicular to n
// assuming p, q, r are on the plane
double orient_by_normal(p3 p, p3 q, p3 r, p3 n) { return (q - p) * (r - p) | n; }
double get_angle(p3 a, p3 b) {
    double costheta = dot(a, b) / a.abs() / b.abs();
    return acos(max((double)-1.0, min((double)1.0, costheta)));
}
double small_angle(p3 v, p3 w) {
    return acos(min(fabs(v | w) / abs(v) / abs(w), (double)1.0));
}

struct plane {
    // n is the perpendicular normal vector to the plane
    p3 n; double d; // (n | p) = d
    // From normal n and offset d
    plane(p3 n, double d) : n(n), d(d) {}
    // From normal n and point P
    plane(p3 n, p3 p) : n(n), d(n | p) {}
    // From three non-collinear points P,Q,R
    plane(p3 p, p3 q, p3 r) : plane((q - p) * (r - p), p) {}
    // positive if on the same side as the normal, negative if on the opposite side, 0 if on the plane
    double side(p3 p) { return (n | p) - d; }
    // distance from point p to plane
    double dist(p3 p) { return fabs(side(p)) / abs(n); }
    // translate the plane by vector t
    plane translate(p3 t) { return {n, d + (n | t)}; }
    // shift the plane perpendicular to n by distance dist
    plane shiftUp(double dist) { return {n, d + dist * abs(n)}; }
    // orthogonal projection of point p onto plane
    p3 proj(p3 p) { return p - n * side(p) / sq(n); }
    // orthogonal reflection of point p onto plane
    p3 refl(p3 p) { return p - n * 2 * side(p) / sq(n); }
    pair<p3, p3> get_two_points_on_plane() {
        assert(sign(n.x) != 0 || sign(n.y) != 0 || sign(n.z) != 0);
        if (sign(n.x) == 0 && sign(n.y) == 0) return {p3(1, 0, d/n.z),  p3(0, 1, d/n.z)};
        if (sign(n.y) == 0 && sign(n.z) == 0) return {p3(d/n.x, 1, 0),  p3(d/n.x, 0, 1)};
        if (sign(n.z) == 0 && sign(n.x) == 0) return {p3(1, d/n.y, 0),  p3(0, d/n.y, 1)};
        if (sign(n.x) == 0) return {p3(1, d/n.y, 0),  p3(0, 0, d/n.z)};
        if (sign(n.y) == 0) return {p3(0, 1, d/n.z),  p3(d/n.x, 0, 0)};
        if (sign(n.z) == 0) return {p3(d/n.x, 0, 1),  p3(0, d/n.y, 0)};
        if (sign(d)!=0) return {p3(d/n.x, 0, 0),  p3(0, d/n.y, 0)};
        return {p3(n.y, -n.x, 0),  p3(-n.y, n.x, 0)};
    }
};

struct coords {
    // coordinate system for coplanar points
    // o is the origin, dx, dy, dz are unit vectors similar to normal 3D system
    // but dx and dy are on the plane
    p3 o, dx, dy, dz;
    // From three points P, Q, R on the plane
    coords(p3 p, p3 q, p3 r) : o(p) {
        dx = unit(q - p);
        dz = unit(dx * (r - p));
        dy = dz * dx;
    }
    // From four points P,Q,R,S: take directions PQ, PR, PS as is
    // it allows us to keep using integer coordinates but has some pitfalls
    // e.g. distances and angles are not preserved but relative positions are (convex hull works)
    coords(p3 p, p3 q, p3 r, p3 s) :
                o(p), dx(q - p), dy(r - p), dz(s - p) {}
    // 2D position vector of point p in this coordinate system centered at o
    // p must be on the plane
    PT pos2d(p3 p) {
        return {(p - o) | dx, (p - o) | dy};
    }
    // returns the 3D position vector of point p in this new coordinate system
    // p can be outside the plane
    p3 pos3d(p3 p) {
        return {(p - o) | dx, (p - o) | dy, (p - o) | dz};
    }
    // given 2D position vector p centered at o, return the original 3D position vector
    p3 pos3d(PT p){
        return o + dx * p.x + dy * p.y;
    }
};

struct line3d {
    // d is the direction vector of the line
    p3 d, o; // p = o + k * d (k is a real parameter)
    line3d() {}
    // From two points P, Q
    line3d(p3 p, p3 q) : d(q - p), o(p) {}
    // From two planes p1, p2
    // assuming they are not parallel
    line3d(plane p1, plane p2) {
        d = p1.n * p2.n;
        o = (p2.n * p1.d - p1.n * p2.d) * d / sq(d);
        // o is actually the closest point on the line to the origin
    }
    double dist2(p3 p) { return sq(d * (p - o)) / sq(d); }
    double dist(p3 p) { return sqrt(dist2(p)); }
    // compare points by their projection on the line
    // so you can sort points on the line using this
    bool cmp_proj(p3 p, p3 q) { return (d | p) < (d | q); }   
    // orthogonal projection of point p onto line
    p3 proj(p3 p) { return o + d * (d|(p - o)) / sq(d); }
    // orthogonal reflection of point p onto line
    p3 refl(p3 p) { return proj(p) * 2 - p; }
    // returns the intersection point of the line with plane p
    // assuming plane and line are not parallel
    p3 inter(plane p) { 
        // assert((d | p.n) != 0); // no intersection if parallel
        return o - d * p.side(o) / (d | p.n); 
    }
};

// smallest distance between two lines
double dist(line3d l1, line3d l2) {
    p3 n = l1.d * l2.d;
    if (n == zero) return l1.dist(l2.o); // parallel
    return fabs((l2.o - l1.o) | n) / abs(n);
}
// closest point from line l2 to line l1
p3 closest_on_l1(line3d l1, line3d l2) {
    p3 n2 = l2.d * (l1.d * l2.d);
    return l1.o + l1.d * ((l2.o - l1.o) | n2) / (l1.d | n2);
}
// small angle between direction vectors of two lines
double get_angle(line3d l1, line3d l2) {
    return small_angle(l1.d, l2.d);
}
bool is_parallel(line3d l1, line3d l2) {
    return l1.d * l2.d == zero;
}
bool is_perpendicular(line3d l1, line3d l2) {
    return sign((l1.d | l2.d)) == 0;
}
// small angle between normal vectors of two planes
double get_angle(plane p1, plane p2) {
    return small_angle(p1.n, p2.n);
}
bool is_parallel(plane p1, plane p2) {
    return p1.n * p2.n == zero;
}
bool is_perpendicular(plane p1, plane p2) {
    return sign((p1.n | p2.n)) == 0;
}
double get_angle(plane p, line3d l) {
    return PI / 2 - small_angle(p.n, l.d);
}
bool is_parallel(plane p, line3d l) {
    return sign((p.n | l.d)) == 0;
}
bool is_perpendicular(plane p, line3d l) {
    return p.n * l.d == zero;
}
// returns the line perpendicular to plane p and passing through point o
line3d perp_through(plane p, p3 o) {return line3d(o, o + p.n);}
// returns the plane perpendicular to line l and passing through point o
plane perp_through(line3d l, p3 o) {return plane(l.d, o);}

// returns two points on intesection line of two planes formed by points
// a1, b1, c1 and a2, b2, c2 respectively
pair<p3, p3> plane_plane_intersection(p3 a1, p3 b1, p3 c1, p3 a2, p3 b2, p3 c2) {
    p3 n1 = (b1 - a1) * (c1 - a1);
    p3 n2 = (b2 - a2) * (c2 - a2);
    double d1 = n1 | a1, d2 = n2 | a2;
    p3 d = n1 * n2;
    if (d == zero) return make_pair(zero, zero);
    p3 o = (n2 * d1 - n1 * d2) * d / (d | d);
    return make_pair(o, o + d);
}
// returns center of circle passing through three
// non-colinear and co-planer points a, b and c
p3 circle_center(p3 a, p3 b, p3 c) {
    p3 v1 = b - a, v2 = c - a;
    double v1v1 = v1 | v1, v2v2 = v2 | v2, v1v2 = v1 | v2;
    double base = 0.5 / (v1v1 * v2v2 - v1v2 * v1v2);
    double k1 = base * v2v2 * (v1v1 - v1v2);
    double k2 = base * v1v1 * (v2v2 - v1v2);
    return a + v1 * k1 + v2 * k2;
}
// segment ab to point c
double distance_from_segment_to_point(p3 a, p3 b, p3 c) {
    if (sign(dot(b - a, c - a)) < 0) return dist(a, c);
    if (sign(dot(a - b, c - b)) < 0) return dist(b, c);
    return fabs(cross((b - a).unit(), c - a).abs());
}
double distance_from_triangle_to_point(p3 a, p3 b, p3 c, p3 d) {
        plane P(a, b, c);
        p3 proj = P.proj(d);
    double dis = min(distance_from_segment_to_point(a, b, d), min(distance_from_segment_to_point(b, c, d), distance_from_segment_to_point(c, a, d)));
    int o = sign(orient_by_normal(a, b, proj, P.n));
    int inside = o == sign(orient_by_normal(b, c, proj, P.n));
    inside &= o == sign(orient_by_normal(c, a, proj, P.n));
    if (inside) return (d - proj).abs();
    return dis;
}
double distance_from_triangle_to_segment(p3 a, p3 b, p3 c, p3 d, p3 e) {
    double l = 0.0, r = 1.0;
    int cnt = 100;
    double ret = inf;
    while (cnt--) {
        double mid1 = l + (r - l) / 3.0, mid2 = r - (r - l) / 3.0;
        double x = distance_from_triangle_to_point(a, b, c, d + (e - d) * mid1);
        double y = distance_from_triangle_to_point(a, b, c, d + (e - d) * mid2);
        if (x < y) {
            r = mid2;
            ret = x;
        } 
        else {
            ret = y;
            l = mid1;
        }
    }
    return ret;
}
// triangles are solid
double distance_from_triangle_to_triangle(p3 a, p3 b, p3 c, p3 d, p3 e, p3 f) {
    double ret = inf;
    ret = min(ret, distance_from_triangle_to_segment(a, b, c, d, e));
    ret = min(ret, distance_from_triangle_to_segment(a, b, c, e, f));
    ret = min(ret, distance_from_triangle_to_segment(a, b, c, f, d));
    ret = min(ret, distance_from_triangle_to_segment(d, e, f, a, b));
    ret = min(ret, distance_from_triangle_to_segment(d, e, f, b, c));
    ret = min(ret, distance_from_triangle_to_segment(d, e, f, c, a));
    return ret;
}


bool operator < (p3 p, p3 q) {
    return tie(p.x, p.y, p.z) < tie(q.x, q.y, q.z);
}
struct edge {
    int v;
    bool same; // is the common edge between two faces in the same order?
};
// Given a series of faces (lists of points) of a polyhedron, reverse some of them
// so that their orientations are consistent (all area vectors of the faces either pointing outwards or inwards)
// just compute the area vector of one face to see if it’s pointing outwards or inwards
vector<vector<p3>> reorient(vector<vector<p3>> fs) {
    int n = fs.size();
        // Find the common edges and create the resulting graph
    vector<vector<edge>> g(n);
    map<pair<p3,p3>, int> es;
    for (int u = 0; u < n; u++) {
        for (int i = 0, m = fs[u].size(); i < m; i++) {
            p3 a = fs[u][i], b = fs[u][(i + 1) % m];
                        // Let’s look at edge a-b
            if (es.count({a, b})) { // seen in same order
                int v = es[{a, b}];
                g[u].push_back({v, true});
                g[v].push_back({u, true});
            } 
            else if (es.count({b, a})) { // seen in different order
                int v = es[ {b, a}];
                g[u].push_back({v,false});
                g[v].push_back({u,false});
            } 
            else { // not seen yet
                es[{a,b}] = u;
            }
        }
    }
        // Perform BFS to find which faces should be flipped
    vector<bool> vis(n,false), flip(n);
    flip[0] = false;
    queue<int> q;
    q.push(0);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (edge e : g[u]) {
            if (!vis[e.v]) {
                vis[e.v] = true;
                                // If the edge was in the same order,
                                // exactly one of the two should be flipped
                flip[e.v] = (flip[u] ^ e.same);
                q.push(e.v);
            }
        }
    }
        // Actually perform the flips
    for (int u = 0; u < n; u++)
        if (flip[u]) {
            reverse(fs[u].begin(), fs[u].end());
        }
    return fs;
}

// O(n^2), O(n) faces in the hull
struct CH3D {
    struct face {
        int a, b, c; // the number of three points on one face of the convex hull
        bool ok;  // whether the face belongs to the face on the final convex hull
    };
    int n; // initial vertex number
    vector<p3> P;
    int num; // convex hull surface triangle number
    vector<face> F; // convex surface triangles
    vector<vector<int>> g;

    void init(vector<p3> p) {
        P = p;
        n = p.size();
        F.resize(8 * n + 1);
        g.resize(n + 1, vector<int> (n + 1));
    }

    double len(p3 a) {
        return sqrt(a | a);
    }
    p3 cross(const p3 &a, const p3 &b, const p3 &c) {
        return (b - a) * (c - a);
    }
    double area(p3 a, p3 b, p3 c) {
        return len((b - a) * (c - a));
    }
    double volume(p3 a, p3 b, p3 c, p3 d) {
        return (b - a) * (c - a) | (d - a);
    }
    // positive: p3 in the same direction
    double dblcmp(p3 &p, face &f) {
        p3 m = P[f.b] - P[f.a];
        p3 n = P[f.c] - P[f.a];
        p3 t = p - P[f.a];
        return (m * n) | t;
    }
    void deal(int p, int a, int b) {
        int f = g[a][b]; // search for another plane adjacent to the edge
        face add;
        if (F[f].ok) {
            if (dblcmp(P[p], F[f]) > eps) dfs(p, f);
            else {
                add.a = b;
                add.b = a;
                add.c = p; // pay attention to the order here, to be right-handed
                add.ok = true;
                g[p][b] = g[a][p] = g[b][a] = num;
                F[num++] = add;
            }
        }
    }
    // recursively search all faces that should be removed from the convex hull
    void dfs(int p, int now) {
        F[now].ok = 0;
        deal(p, F[now].b, F[now].a);
        deal(p, F[now].c, F[now].b);
        deal(p, F[now].a, F[now].c);
    }
    bool same(int s, int t) {
        p3 &a = P[F[s].a];
        p3 &b = P[F[s].b];
        p3 &c = P[F[s].c];
        return fabs(volume(a, b, c, P[F[t].a])) < eps &&
               fabs(volume(a, b, c, P[F[t].b])) < eps &&
               fabs(volume(a, b, c, P[F[t].c])) < eps;
    }
    // building a 3D convex hull
    void create_hull() {
        int i, j, tmp;
        face add;
        num = 0;
        if (n < 4)return;

        // ensure that the first four points are not coplanar
        bool flag = true;
        for (i = 1; i < n; i++) {
            if (len(P[0] - P[i]) > eps) {
                swap(P[1], P[i]);
                flag = false;
                break;
            }
        }
        if (flag) return;
        flag = true;
        // make the first three points not collinear
        for (i = 2; i < n; i++) {
            if (len((P[0] - P[1]) * (P[1] - P[i])) > eps) {
                swap(P[2], P[i]);
                flag = false;
                break;
            }
        }
        if (flag) return;
        flag = true;
        // make the first four points not coplanar
        for (int i = 3; i < n; i++) {
            if (fabs((P[0] - P[1]) * (P[1] - P[2]) | (P[0] - P[i])) > eps) {
                swap(P[3], P[i]);
                flag = false;
                break;
            }
        }
        if (flag) return;

        for (i = 0; i < 4; i++) {
            add.a = (i + 1) % 4;
            add.b = (i + 2) % 4;
            add.c = (i + 3) % 4;
            add.ok = true;
            if (dblcmp(P[i], add) > 0)swap(add.b, add.c);
            g[add.a][add.b] = g[add.b][add.c] = g[add.c][add.a] = num;
            F[num++] = add;
        }
        for (i = 4; i < n; i++) {
            for (j = 0; j < num; j++) {
                if (F[j].ok && dblcmp(P[i], F[j]) > eps) {
                    dfs(i, j);
                    break;
                }
            }
        }
        tmp = num;
        for (i = num = 0; i < tmp; i++)
            if (F[i].ok) F[num++] = F[i];

    }
    double surface_area() {
        double res = 0;
        if (n == 3) {
            p3 p = cross(P[0], P[1], P[2]);
            res = len(p) / 2.0;
            return res;
        }
        for(int i = 0; i < num; i++) {
            res += area(P[F[i].a], P[F[i].b], P[F[i].c]);
        }
        return res / 2.0;
    }
    double volume() {
        double res = 0;
        p3 tmp(0, 0, 0);
        for(int i = 0; i < num; i++) {
            res += volume(tmp, P[F[i].a], P[F[i].b], P[F[i].c]);
        }
        return fabs(res / 6.0);
    }
    int number_of_triangles() { // number of surface triangles
        return num;
    }
    int number_of_polygons() {  // number of surface polygons
        int i, j, res, flag;
        for (i = res = 0; i < num; i++) {
            flag = 1;
            for (j = 0; j < i; j++) {
                if (same(i, j)) {
                    flag = 0;
                    break;
                }
            }
            res += flag;
        }
        return res;
    }
    p3 centroid() { // center of gravity
        p3 ans(0, 0, 0), o(0, 0, 0);
        double all = 0;
        for (int i = 0; i < num; i++) {
            double vol = volume(o, P[F[i].a], P[F[i].b], P[F[i].c]);
            ans = ans + (o + P[F[i].a] + P[F[i].b] + P[F[i].c]) / 4.0 * vol;
            all += vol;
        }
        ans = ans / all;
        return ans;
    }
    double point_to_face_distance(p3 p, int i) {
        return fabs(volume(P[F[i].a], P[F[i].b], P[F[i].c], p) / len((P[F[i].b] - P[F[i].a]) * (P[F[i].c] - P[F[i].a])));
    }
};

// https://desktop.arcgis.com/en/arcmap/10.7/map/projections/geographic-coordinate-system.htm
// given the radius of the sphere, latitude and longitude of a point in degrees
// return the 3D coordinates of the point on the sphere assuming the sphere is centered at the origin
p3 get_sphere(double r, double lat, double lon) {
    lat *= PI / 180, lon *= PI / 180;
    return {r * cos(lat) * cos(lon), r * cos(lat) * sin(lon), r * sin(lat)};
}
int sphere_line_intersection(p3 o, double r, line3d l, pair<p3,p3> &out) {
    double h2 = r * r - l.dist2(o);
    if (h2 < 0) return 0; // the line doesn’t touch the sphere
    p3 p = l.proj(o);
    p3 h = l.d * sqrt(h2)/abs(l.d); // vector parallel to l, of length h
    out = {p - h, p + h};
    return 1 + (h2 > 0);
}
// The shortest distance between two points A and B on a sphere (O, r) is
// given by travelling along plane OAB and on the surface of the sphere. It is called the great-circle distance
// if a and b are outside the sphere, then it will give the distance between their projections on the sphere
double great_circle_dist(p3 o, double r, p3 a, p3 b) {
    // s = r * theta
    return r * get_angle(a - o, b - o);
}

// Assume that the sphere is centered at the origin
// We will call a segment [AB] valid if A and B are not 
// opposite each other on the sphere
bool validSegment(p3 a, p3 b) {
    return a * b != zero || (a | b) > 0;
}

bool proper_intersection(p3 a, p3 b, p3 c, p3 d, p3 &out) {
    p3 ab = a * b, cd = c * d; // normals of planes OAB and OCD
    int oa = sign(cd | a),
        ob = sign(cd | b),
        oc = sign(ab | c),
        od = sign(ab | d);
    out = ab * cd * od; // four multiplications => careful with overflow!
    return (oa != ob && oc != od && oa != oc);
}

// Assume that the sphere is centered at the origin
bool point_on_sphere_segment(p3 a, p3 b, p3 p) {
    p3 n = a*b;
    if (n == zero)
        return a * p == zero && (a | p) > 0;
    return (n | p) == 0 && (n | a * p) >= 0 && (n | b * p) <= 0;
}

struct DirectionSet : vector<p3> {
    using vector::vector; // import constructors
    void insert(p3 p) {
        for (p3 q : *this) if (p*q == zero) return;
        push_back(p);
    }
};
// Assume that the sphere is centered at the origin
// it returns the direction vectors of the intersection points
// to get the actual points, scale the direction vectors to the radius of the sphere
DirectionSet segment_segment_intersection_on_sphere(p3 a, p3 b, p3 c, p3 d) {
    assert(validSegment(a, b) && validSegment(c, d));
    p3 out;
    if (proper_intersection(a, b, c, d, out)) return {out}; 
    DirectionSet s;
    if (point_on_sphere_segment(c, d, a)) s.insert(a);
    if (point_on_sphere_segment(c, d, b)) s.insert(b);
    if (point_on_sphere_segment(a, b, c)) s.insert(c);
    if (point_on_sphere_segment(a, b, d)) s.insert(d);
    return s;
}

// small angle between spherical segments ab and ac
// assume that the sphere is centered at the origin
// all points a, b, c are on the sphere
double angle_on_sphere(p3 a, p3 b, p3 c) {
    return get_angle(a * b, a * c);
}

// oriented angle between spherical segments ab and ac
// that is how much we rotate counterclockwise to get from ab to ac
// assume that the sphere is centered at the origin
// all points a, b, c are on the sphere
double oriented_angle_on_sphere(p3 a, p3 b, p3 c) {
    if ((a * b | c) >= 0) return angle_on_sphere(a, b, c);
    else return 2 * PI - angle_on_sphere(a, b, c);
}

// Assume that the sphere is centered at the origin
// the polygon is simple and given in counterclockwise order
// for each consecutive pair of points, the counterclockwise left 
// part of the segment is considered to be inside the surface area that the polygon encloses
// if the polygon is outside the sphere, the projection of the polygon on the sphere will be considered
double area_on_the_surface_of_the_sphere(double r, vector<p3> p) {
    int n = p.size();
    double sum = -(n - 2) * PI;
    for (int i = 0; i < n; i++) {
        sum += oriented_angle_on_sphere(p[(i + 1) % n], p[(i + 2) % n], p[i]);
    }
    return r * r * sum;
}

// Assume that O is the origin
// it returns 0 if O is outside the polyhedron
// 1 if O is inside the polyhedron, and the vector areas of the faces are oriented towards the outside;
// −1 if O is inside the polyhedron, and the vector areas of the faces are oriented towards the inside.
int winding_number_3D(vector<vector<p3>> fs) {
    double sum = 0;
    for (vector<p3> f : fs) {
        sum += remainder(area_on_the_surface_of_the_sphere(1, f), 4 * PI);
    }
    return round(sum / (4 * PI));
}

struct sphere {
    p3 c;
    double r;
    sphere() {}
    sphere(p3 c, double r) : c(c), r(r) {}
};

// spherical cap is a portion of a sphere cut off by a plane
// https://en.wikipedia.org/wiki/Spherical_cap
struct spherical_cap {
    p3 c;
    double r;
    spherical_cap() {}
    spherical_cap(p3 c, double r) : c(c), r(r) {}

    // angle th is the polar angle between the rays from the center of the sphere to one edge of the cap
    // and orthogonal line from the center of the sphere to the plane of the cap

    // height of the cap (just like real world cap)
    double height(double th)      {
        return r * (1 - cos(th));
    }
    // radius of the base of the cap
    double base_radius(double th)  {
        return r * sin(th);
    }
    // volume of the cap
    double volume(double th)      {
        double h = height(th);
        return PI * h * h * (3 * r - h) / 3.0;
    }
    // surface area of the cap
    double surface_area(double th) {
        double h = height(th);
        return 2 * PI * r * h;
    }
};

// returns the sphere passing through four points
sphere circumscribed_sphere(p3 a, p3 b, p3 c, p3 d) {
    assert( sign(plane(a, b, c).side(d)) != 0);

    plane u = plane(a - b, (a + b) / 2);
    plane v = plane(b - c, (b + c) / 2);
    plane w = plane(c - d, (c + d) / 2);

    assert(!is_parallel(u, v));
    assert(!is_parallel(v, w));
    line3d l1(u, v), l2(v, w);
    assert( sign(dist(l1, l2)) == 0);

    p3 C = closest_on_l1(l1, l2);
    return sphere(C, abs(C - a));
}

// https://mathworld.wolfram.com/Sphere-SphereIntersection.html
// it won't work if one sphere is totally inside the other sphere
// handle that case separately
// returns the surface area and volume of the intersection
pair<double, double> sphere_sphere_intersection(sphere s1, sphere s2) {
    double d = abs(s1.c - s2.c);
    if(sign(d - s1.r - s2.r) >= 0) return {0, 0}; // not intersecting
    // only the distance matters, so we will now consider the centers
    // of the big sphere to be (0, 0, 0) and (d, 0, 0) for the small sphere
    // we can transform the results back to w.r.t the real centers if we want

    double R = max(s1.r, s2.r);
    double r = min(s1.r, s2.r);
    double y = R + r - d;
    double x = (R * R - r * r + d * d) / (2 * d);
    // the intersecting plane is parallel to the yz plane
    // with the above x value as its x coordinate
    double w = d * d - r * r  + R * R;
    double a = sqrt(4 * d * d * R * R - w * w) / (2.0 * d);
    // a is the radius of the intersecting circle on the intersecting plane
    // with center (x, 0)
    double h1 = R - x;
    double h2 = y - h1;
    // h1 is the height of the intersecting spherical cap of the big sphere
    // h2 is for the small sphere

    // total volume of the whole intersection = sum of the volumes of the spherical caps
    double volume      = PI * h1 * h1 * (3 * R - h1) / 3.0 + PI * h2 * h2 * (3 * r - h2) / 3.0;
    // total surface area of the intersecting spherical caps
    double surface_area = 2 * PI * R * h1 + 2 * PI * r * h2;
    return make_pair(surface_area, volume);
}

sphere smallest_enclosing_sphere(vector<p3> p) {
    int n = p.size();
    p3 c(0, 0, 0);
    for(int i = 0; i < n; i++) c = c + p[i];
    c = c / n;

    double ratio = 0.1;
    int pos = 0;
    int it = 100000;
    while (it--) {
        pos = 0;
        for (int i = 1; i < n; i++) {
            if(sq(c - p[i]) > sq(c - p[pos])) pos = i;
        }
        c = c + (p[pos] - c) * ratio;
        ratio *= 0.998;
    }
    return sphere(c, abs(c - p[pos]));
}

// it returns the angle of the spherical cap that is formed by the intersection of all tangents
double tangent_from_point_to_sphere(p3 p, sphere s) {
    double d = abs(p - s.c);
    if (sign(d - s.r) < 0) return -1; // inside the sphere, so no tangent
    if (sign(d - s.r) == 0) return -2; // on the sphere, handle separately
    double tangent_length = sqrt(d * d - s.r * s.r);
    double th = acos(s.r / d);
    return th;
}


struct pyramid {
        int n;     // number of sides of the pyramid
        double l;  // length of each side
        double ang;
        pyramid(int _n, double _l) {
            n = _n;
            l = _l;
            ang = PI / n;
        }
        double base_area() {
            return l * l * n / (4 * tan(ang));
        }
        double height() {
            return l * sqrt(1 - 1 / (4 * sin(ang) * sin(ang)) );
        }
        double volume() {
            return base_area() * height() / 3;
        }
};

struct cylinder {
    double r, h; // radius and height
    cylinder(double _r, double _h) {
        r = _r;
        h = _h;
    }
    double volume() {
        return PI * r * r * h;
    }
    double surface_area() {
        return 2 * PI * r * h + 2 * PI * r * r;
    }
};

struct cone {
    double r, h; // radius and height
    cone(double _r, double _h) {
        r = _r;
        h = _h;
    }
    double volume() {
        return PI * r * r * h / 3.0;
    }
    double surface_area() {
        return PI * r * (r + sqrt(h * h + r * r));
    }
};

int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
        
    return 0;
}
