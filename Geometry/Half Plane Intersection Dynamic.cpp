#include <bits/stdc++.h>
using namespace std;

#define double long double
const double eps = 1e-12;
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
double cross2(PT a, PT b, PT c) { return cross(b - a, c - a); }
// intersection point between ab and cd assuming unique intersection exists
PT line_line_intersection(PT a, PT b, PT c, PT d) {
    double a1 = a.y - b.y, b1 = b.x - a.x, c1 = cross(a, b);
    double a2 = c.y - d.y, b2 = d.x - c.x, c2 = cross(c, d);
    double det = a1 * b2 - a2 * b1;
    assert(sign(det) != 0); // to check precision issues
    PT ans = PT((b1 * c2 - b2 * c1) / det, (c1 * a2 - a1 * c2) / det);
    return ans;
}
const double D = 1e6; // each point is divided by D for better precision
struct Angle {
    double x, y;
    Angle(double x, double y) : x(x), y(y) {}
    Angle operator - (Angle a) const { return {x - a.x, y - a.y}; }
    int quad() const {
        if (y < 0) return (sign(x) >= 0) + 2;
        if (y > 0) return (sign(x) <= 0);
        return (sign(x) <= 0) * 2;
    }
    PT p(bool truncate = 1) const {
        if (truncate) return {x / D, y / D};
        return {x, y}; 
    }
    Angle rotateccw90() const { return {-y, x}; }
};
bool operator < (Angle a, Angle b) {
    return make_pair(a.quad(), a.y * b.x) < make_pair(b.quad(), a.x * b.y);
}
struct HalfPlaneSet : multimap<Angle, PT> { //(dir, PT)
    using iter = multimap<Angle, PT>::iterator;
    HalfPlaneSet() {
        insert({{+1, 0}, {-20, -20}}); //points are divided by D
        insert({{0, +1}, {+20, -20}});
        insert({{-1, 0}, {+20, +20}});
        insert({{0, -1}, {-20, +20}});
    }
    iter get_next(iter it) { return (next(it) == end() ? begin() : next(it)); }
    iter get_prev(iter it) { return (it == begin() ? prev(end()) : prev(it)); }
    iter fix(iter it) { return it == end() ? begin() : it; }
    //cut the left part of the line from point a to point b
    void cut(Angle a, Angle b) { // O(log n) amortized
        if (empty()) return;
        int old_size = size();
        auto eval = [&](iter it) {
            return orientation(a.p(), b.p(), it->second);
        };
        auto intersect = [&](iter it)  {
            return line_line_intersection(a.p(), b.p(), it->second, it->first.p(0) + it->second);
        };
        auto it = fix(lower_bound(b - a));
        if (eval(it) >= 0) return;
        while (size() && eval(get_prev(it)) < 0) fix(erase(get_prev(it)));
        while (size() && eval(get_next(it)) < 0) it = fix(erase(it));
        if (empty()) return;
        if (eval(get_next(it)) > 0) it->second = intersect(it);
        else it = fix(erase(it));
        if (old_size <= 2) return;
        it = get_prev(it);
        auto p = intersect(it);
        insert(it, {b - a, p});
        if (eval(it) == 0) erase(it);
    }
    // maximum dot product with c
    double maximum_dot_product(Angle c) { // answer / (D * D)
        assert(!empty());
        auto it = fix(lower_bound(c.rotateccw90()));
        return dot(it->second, c.p());
    }
    void print() {
        for (auto x : *this) {
            cout << x.second.x << " " << x.second.y << '\n';
            cout << "dir: " << x.first.x << " " << x.first.y << '\n';
        }
    }
    double area() { // answer / (D * D)
        if (size() <= 2) return 0;
        double ans = 0;
        for (auto it = begin(); it != end(); ++it) {
            ans += cross(it->second, get_next(it)->second);
        }
        return ans;
    }
};

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        vector<Angle> p;
        for (int i = 0; i < n; ++i) {
            double x, y; cin >> x >> y;
            p.emplace_back(x, y);
        }
        reverse(p.begin(), p.end());
        HalfPlaneSet h;
        for (int i = 0; i < n; i++) {
            h.cut(p[i], p[(i + 1) % n]);
        }
        cout << fixed << setprecision(10) << h.area() / 800.L << '\n';
    }
}
// https://www.codechef.com/problems/ALLPOLY