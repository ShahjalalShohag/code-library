#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

#define double long double
const double inf = 1e100;
const double eps = 1e-12;
const double PI = acos(-1.0);
int sign(double x) {
    return (x > eps) - (x < -eps);
}
struct PT {
    double x, y;
    PT() {
        x = 0, y = 0;
    }
    PT(double x, double y) : x(x), y(y) {}
    PT(const PT &p) : x(p.x), y(p.y)    {}
    PT operator + (const PT &a) const {
        return PT(x + a.x, y + a.y);
    }
    PT operator - (const PT &a) const {
        return PT(x - a.x, y - a.y);
    }
    PT operator * (const double a) const {
        return PT(x * a, y * a);
    }
    friend PT operator * (const double &a, const PT &b) {
        return PT(a * b.x, a * b.y);
    }
    PT operator / (const double a) const {
        return PT(x / a, y / a);
    }
    bool operator == (PT a) const {
        return sign(a.x - x) == 0 && sign(a.y - y) == 0;
    }
    bool operator != (PT a) const {
        return !(*this == a);
    }
    bool operator < (PT a) const {
        return sign(a.x - x) == 0 ? y < a.y : x < a.x;
    }
    bool operator > (PT a) const {
        return sign(a.x - x) == 0 ? y > a.y : x > a.x;
    }
    double norm() {
        return sqrt(x * x + y * y);
    }
    double norm2() {
        return x * x + y * y;
    }
    PT perp() {
        return PT(-y, x);
    }
    double arg() {
        return atan2(y, x);
    }
    PT truncate(double r) { // returns a vector with norm r and having same direction
        double k = norm();
        if (!sign(k)) return *this;
        r /= k;
        return PT(x * r, y * r);
    }
};
inline double dot(PT a, PT b) {
    return a.x * b.x + a.y * b.y;
}
inline double dist2(PT a, PT b) {
    return dot(a - b, a - b);
}
inline double dist(PT a, PT b) {
    return sqrt(dot(a - b, a - b));
}
inline double cross(PT a, PT b) {
    return a.x * b.y - a.y * b.x;
}
inline double cross2(PT a, PT b, PT c) {
    return cross(b - a, c - a);
}
inline int orientation(PT a, PT b, PT c) {
    return sign(cross(b - a, c - a));
}
PT perp(PT a) {
    return PT(-a.y, a.x);
}
PT rotateccw90(PT a) {
    return PT(-a.y, a.x);
}
PT rotatecw90(PT a) {
    return PT(a.y, -a.x);
}
PT rotateccw(PT a, double t) {
    return PT(a.x * cos(t) - a.y * sin(t), a.x * sin(t) + a.y * cos(t));
}
PT rotatecw(PT a, double t) {
    return PT(a.x * cos(t) + a.y * sin(t), -a.x * sin(t) + a.y * cos(t));
}
double SQ(double x) {
    return x * x;
}
double rad_to_deg(double r) {
    return (r * 180.0 / PI);
}
double deg_to_rad(double d) {
    return (d * PI / 180.0);
}
double get_angle(PT a, PT b) {
    double costheta = dot(a, b) / a.norm() / b.norm();
    return acos(max((double)-1.0, min((double)1.0, costheta)));
}
struct seg {
    PT p, q;
    int id;
    seg(PT a, PT b, int c) {
        p = a; q = b; id = c;
    }
    double eval_y(double x) const {
        if (fabs(p.x - q.x) < eps) return p.y;
        return p.y + (q.y - p.y) * (x - p.x) / (q.x - p.x);
    }
};
inline bool intersect1d (double l1, double r1, double l2, double r2) {
    if (l1 > r1)  swap (l1, r1);
    if (l2 > r2)  swap (l2, r2);
    return max (l1, l2) <= min (r1, r2) + eps;
}
// seg - seg intersection
bool intersect (const seg & a, const seg & b) {
    return intersect1d (a.p.x, a.q.x, b.p.x, b.q.x)
           && intersect1d (a.p.y, a.q.y, b.p.y, b.q.y)
           && cross2 (a.p, a.q, b.p) * cross2 (a.p, a.q, b.q) <= eps
           && cross2 (b.p, b.q, a.p) * cross2 (b.p, b.q, a.q) <= eps;
}
bool operator < (const seg& a, const seg& b) {
    double x = max(min(a.p.x, a.q.x), min(b.p.x, b.q.x));
    return a.eval_y(x) < b.eval_y(x);
}
struct event {
    double x;
    int ty, id;
    event() {}
    event(double x, int ty, int id) : x(x), ty(ty), id(id) {}
    bool operator < (const event& e) const {
        if (fabs(x - e.x) >= eps) return x < e.x;
        return make_pair(ty, id) < make_pair(e.ty, e.id);
    }
};
typedef set<seg>::iterator iter;
// returns any pair of segments such that they intersect
pair<int, int> any_intersection(const vector<seg>& a) {
    int n = (int)a.size();
    vector<event> e;
    for (int i = 0; i < n; ++i) {
        e.push_back(event(min(a[i].p.x, a[i].q.x), +1, i));
        e.push_back(event(max(a[i].p.x, a[i].q.x), -1, i));
    }
    sort(e.begin(), e.end());
    set<seg> cur;
    vector<iter> where;
    where.resize((int)a.size());
    try {
        for (int i = 0; i < (int)e.size(); ++i) {
            int id = e[i].id;
            if (e[i].ty == +1) {
                iter nxt = cur.lower_bound(a[id]);
                iter prv = nxt == cur.begin() ? cur.end() : prev(nxt);;
                if (nxt != cur.end() && intersect(*nxt, a[id])) return make_pair(nxt->id, id);
                if (prv != cur.end() && intersect(*prv, a[id])) return make_pair(prv->id, id);
                where[id] = cur.insert(nxt, a[id]);
            } else {
                auto nw = where[id];
                if (nw == cur.end()) continue;
                iter nxt = next(nw);
                iter prv = nw == cur.begin() ? cur.end() : prev(nw);;
                if (nxt != cur.end() && prv != cur.end() && intersect(*nxt, *prv))
                    return make_pair(prv->id, nxt->id);
                cur.erase(nw);
            }
        }
    }
    catch(...) {return {0, 1};}
    return make_pair(-1, -1);
}
PT a[N], d[N];
double per[N];
int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n; cin >> n;
    for (int i = 0; i < n; i++) {
        int x, y, dx, dy, sx;
        cin >> x >> y >> dx >> dy >> sx;
        a[i] = PT(x, y);
        d[i] = PT(dx, dy);
        per[i] = sx;
    }
    double l = 0, r = 1e12;
    int ok = 0;
    while (r - l >= 1e-7) {
        double mid = (l + r) * 0.5;
        vector<seg> v;
        for (int i = 0; i < n; i++) {
            PT q = a[i] + d[i].truncate(mid * per[i]);
            v.push_back(seg(a[i], q, i));
        }
        if (any_intersection(v).first != -1) {
            r = mid;
            ok = 1;
        } else l = mid;
    }
    double ans = (l + r) * 0.5;
    if (ok) cout << fixed << setprecision(10) << ans << '\n';
    else cout << "No show :(\n";
    return 0;
}
// https://codeforces.com/contest/1359/problem/F
