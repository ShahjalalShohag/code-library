#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

// credit: chemthan
typedef set<pair<int, int> >::iterator iter;
struct PT {
    int x, y;
    PT() : x(0), y(0) {}
    PT(int x, int y) : x(x), y(y) {}
    PT(const PT& rhs) : x(rhs.x), y(rhs.y) {}
    PT(const iter& p) : x(p->first), y(p->second) {}
    PT operator - (const PT& rhs) const {
        return PT(x - rhs.x, y - rhs.y);
    }
};
long long cross(PT a, PT b) {
    return (long long) a.x * b.y - (long long) a.y * b.x;
}
inline int inside(set<pair<int, int>>& hull, const PT& p) { //border inclusive
    int x = p.x, y = p.y;
    iter p1 = hull.lower_bound(make_pair(x, y));
    if (p1 == hull.end()) return 0;
    if (p1->first == x) return p1 != hull.begin() && y <= p1->second;
    if (p1 == hull.begin()) return 0;
    iter p2(p1--);
    return cross(p - PT(p1), PT(p2) - p) >= 0;
}
inline void del(set<pair<int, int>>& hull, iter it, long long& scross) {
    if (hull.size() == 1) {
        hull.erase(it);
        return;
    }
    if (it == hull.begin()) {
        iter p1 = it++;
        scross -= cross(p1, it);
        hull.erase(p1);
        return;
    }
    iter p1 = --it, p2 = ++it;
    if (++it == hull.end()) {
        scross -= cross(p1, p2);
        hull.erase(p2);
        return;
    }
    scross -= cross(p1, p2) + cross(p2, it) - cross(p1, it);
    hull.erase(p2);
}
inline void add(set<pair<int, int>>& hull, iter it, long long& scross) {
    if (hull.size() == 1) return;
    if (it == hull.begin()) {
        iter p1 = it++;
        scross += cross(p1, it);
        return;
    }
    iter p1 = --it, p2 = ++it;
    if (++it == hull.end()) {
        scross += cross(p1, p2);
        return;
    }
    scross += cross(p1, p2) + cross(p2, it) - cross(p1, it);
}
inline void add(set<pair<int, int>>& hull, const PT& p, long long& scross) { //no collinear PTs
    if (inside(hull, p)) return;
    int x = p.x, y = p.y;
    iter pnt = hull.insert(make_pair(x, y)).first, p1, p2;
    add(hull, pnt, scross);
    for ( ; ; del(hull, p2, scross)) {
        p1 = pnt;
        if (++p1 == hull.end()) break;
        p2 = p1;
        if (++p1 == hull.end()) break;
        if (cross(PT(p2) - p, PT(p1) - p) < 0) break;
    }
    for ( ; ; del(hull, p2, scross)) {
        if ((p1 = pnt) == hull.begin()) break;
        if (--p1 == hull.begin()) break;
        p2 = p1--;
        if (cross(PT(p2) - p, PT(p1) - p) > 0) break;
    }
}

int main() {
    long long ucross = 0, dcross = 0;
    set<pair<int, int>> uhull, dhull;
    PT p[] = {PT(0, 0), PT(3, 0), PT(3, 3), PT(0, 3), PT(0, 1), PT(0, 2), PT(3, 1), PT(3, 2)};
    for (int i = 0; i < 5; i++) {
        add(uhull, PT(+p[i].x, +p[i].y), ucross);
        add(dhull, PT(-p[i].x, -p[i].y), dcross);
    }
    for (iter it = uhull.begin(); it != uhull.end(); it++) {
        cout << "(" << +it->first << "," << +it->second << ") ";
    }
    cout << "\n";
    for (iter it = dhull.begin(); it != dhull.end(); it++) {
        cout << "(" << -it->first << "," << -it->second << ") ";
    }
    cout << "\n";
    cout << fixed << setprecision(1) << "Area: " << fabs(ucross + dcross) / 2.0 << "\n";
    return 0;
}
