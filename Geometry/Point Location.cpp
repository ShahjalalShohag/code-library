#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

// you are given a planar subdivision without no vertices of degree one and zero,
// and a lot of queries. Each query is a point, for which we should determine 
// the face of the subdivision it belongs to. We will sove it offline.
// for doubles change the compare methods and the point type.
// O(log n) per query
typedef long long ll;
bool ge(const ll& a, const ll& b) { return a >= b; }
bool le(const ll& a, const ll& b) { return a <= b; }
bool eq(const ll& a, const ll& b) { return a == b; }
bool gt(const ll& a, const ll& b) { return a > b; }
bool lt(const ll& a, const ll& b) { return a < b; }
int sign(const ll& x) { return le(x, 0) ? eq(x, 0) ? 0 : -1 : 1; }
struct PT {
    ll x, y;
    PT() {}
    PT(ll _x, ll _y) : x(_x), y(_y) {}
    PT operator-(const PT& a) const { return PT(x - a.x, y - a.y); }
    ll dot(const PT& a) const { return x * a.x + y * a.y; }
    ll dot(const PT& a, const PT& b) const { return (a - *this).dot(b - *this); }
    ll cross(const PT& a) const { return x * a.y - y * a.x; }
    ll cross(const PT& a, const PT& b) const { return (a - *this).cross(b - *this); }
    bool operator == (const PT& a) const { return a.x == x && a.y == y; }
};
struct edge {
    PT l, r;
};
bool edge_cmp(edge* edge1, edge* edge2) {
    const PT a = edge1->l, b = edge1->r;
    const PT c = edge2->l, d = edge2->r;
    int val = sign(a.cross(b, c)) + sign(a.cross(b, d));
    if (val != 0) return val > 0;
    val = sign(c.cross(d, a)) + sign(c.cross(d, b));
    return val < 0;
}
enum EventType { DEL = 2, ADD = 3, GET = 1, VERT = 0 };
struct Event {
    EventType type; int pos;
    bool operator < (const Event& event) const {
        return type < event.type;
    }
};
vector<edge*> sweepline(vector<edge*> planar, vector<PT> queries) {
    using pt_type = decltype(PT::x);
    // collect all x-coordinates
    auto s = set<pt_type, std::function<bool(const pt_type&, const pt_type&)>>(lt);
    for (PT p : queries) s.insert(p.x);
    for (edge* e : planar) {
        s.insert(e->l.x);
        s.insert(e->r.x);
    }
    // map all x-coordinates to ids
    int cid = 0;
    auto id = map<pt_type, int, std::function<bool(const pt_type&, const pt_type&)>>(lt);
    for (auto x : s) id[x] = cid++;
    // create events
    auto t = set<edge*, decltype(*edge_cmp)>(edge_cmp);
    auto vert_cmp = [](const pair<pt_type, int>& l,
    const pair<pt_type, int>& r) {
        if (!eq(l.first, r.first)) return lt(l.first, r.first);
        return l.second < r.second;
    };
    auto vert = set<pair<pt_type, int>, decltype(vert_cmp)>(vert_cmp);
    vector<vector<Event>> events(cid);
    for (int i = 0; i < (int)queries.size(); i++) {
        int x = id[queries[i].x];
        events[x].push_back(Event{GET, i});
    }
    for (int i = 0; i < (int)planar.size(); i++) {
        int lx = id[planar[i]->l.x], rx = id[planar[i]->r.x];
        if (lx > rx) {
            swap(lx, rx);
            swap(planar[i]->l, planar[i]->r);
        }
        if (lx == rx) {
            events[lx].push_back(Event{VERT, i});
        } 
        else {
            events[lx].push_back(Event{ADD, i});
            events[rx].push_back(Event{DEL, i});
        }
    }
    // perform sweep line algorithm
    vector<edge*> ans(queries.size(), nullptr);
    for (int x = 0; x < cid; x++) {
        sort(events[x].begin(), events[x].end());
        vert.clear();
        for (Event event : events[x]) {
            if (event.type == DEL) {
                t.erase(planar[event.pos]);
            }
            if (event.type == VERT) {
                vert.insert(make_pair(min(planar[event.pos]->l.y, planar[event.pos]->r.y), event.pos));
            }
            if (event.type == ADD) {
                t.insert(planar[event.pos]);
            }
            if (event.type == GET) {
                auto jt = vert.upper_bound(make_pair(queries[event.pos].y, planar.size()));
                if (jt != vert.begin()) {
                    --jt;
                    int i = jt->second;
                    if (ge(max(planar[i]->l.y, planar[i]->r.y), queries[event.pos].y)) {
                        ans[event.pos] = planar[i];
                        continue;
                    }
                }
                edge* e = new edge;
                e->l = e->r = queries[event.pos];
                auto it = t.upper_bound(e);
                if (it != t.begin()) ans[event.pos] = *(--it);
                delete e;
            }
        }
        for (Event event : events[x]) {
            if (event.type != GET) continue;
            if (ans[event.pos] != nullptr && eq(ans[event.pos]->l.x, ans[event.pos]->r.x)) continue;
            edge* e = new edge;
            e->l = e->r = queries[event.pos];
            auto it = t.upper_bound(e);
            delete e;
            if (it == t.begin()) e = nullptr;
            else e = *(--it);
            if (ans[event.pos] == nullptr) {
                ans[event.pos] = e;
                continue;
            }
            if (e == nullptr) continue;
            if (e == ans[event.pos]) continue;
            if (id[ans[event.pos]->r.x] == x) {
                if (id[e->l.x] == x) {
                    if (gt(e->l.y, ans[event.pos]->r.y)) ans[event.pos] = e;
                }
            } 
            else {
                ans[event.pos] = e;
            }
        }
    }
    return ans;
}
// Each edge usually bounds two faces and it is, therefore, convenient to regard
// each edge as two "half-edges" (which are represented by the two edges 
// with opposite directions, between two vertices). 
// Each half-edge is "associated" with a single face and thus has a pointer to 
// that face. All half-edges associated with a face are clockwise or 
// counter-clockwise. A half-edge has a pointer to the next half-edge 
// and previous half-edge OF THE SAME FACE. To reach the other face, 
// we can go to the twin of the half-edge and then traverse the other face. 
// Each half-edge also has a pointer to its origin vertex 
// the outer face is numbered −1.
struct DCEL {
    struct edge {
        PT origin;
        edge* nxt = nullptr;
        edge* twin = nullptr;
        int face;
        int id = 0;
    };
    vector<edge*> body;
};
// For each query a pair (1,i) is returned if the point lies 
// strictly inside the face number i, and a pair (0,i) is returned 
// if the point lies on the edge number i.
vector<pair<int, int>> point_location(DCEL planar, vector<PT> queries) {
    vector<pair<int, int>> ans(queries.size());
    vector<edge*> planar2;
    map<intptr_t, int> pos;
    map<intptr_t, int> added_on;
    int n = planar.body.size();
    for (int i = 0; i < n; i++) {
        if (planar.body[i]->face > planar.body[i]->twin->face) continue;
        edge* e = new edge;
        e->l = planar.body[i]->origin;
        e->r = planar.body[i]->twin->origin;
        added_on[(intptr_t)e] = i;
        pos[(intptr_t)e] =
            lt(planar.body[i]->origin.x, planar.body[i]->twin->origin.x)
            ? planar.body[i]->face
            : planar.body[i]->twin->face;
        planar2.push_back(e);
    }
    auto res = sweepline(planar2, queries);
    for (int i = 0; i < (int)queries.size(); i++) {
        if (res[i] == nullptr) {
            ans[i] = make_pair(1, -1);
            continue;
        }
        PT p = queries[i];
        PT l = res[i]->l, r = res[i]->r;
        if (eq(p.cross(l, r), 0) && le(p.dot(l, r), 0)) {
            ans[i] = make_pair(0, added_on[(intptr_t)res[i]]);
            continue;
        }
        ans[i] = make_pair(1, pos[(intptr_t)res[i]]);
    }
    for (auto e : planar2) delete e;
    return ans;
}
int32_t main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n; cin >> n;
    vector<PT> Q(n);
    for (int i = 0; i < n; i++) {
    	cin >> Q[i].x >> Q[i].y;
    }
    int q; cin >> q;
    DCEL planar;
    for (int i = 0; i < q; i++) {
    	int n; cin >> n;
    	vector<PT> p(n);
    	vector<DCEL::edge*> e(n);
    	for (int j = 0; j < n; j++) {
    		cin >> p[j].x >> p[j].y;
    		e[j] = new DCEL::edge;
    		e[j]-> twin = new DCEL::edge;
    		e[j]->id = i; e[j]->twin->id = i;
    	}
    	for (int j = 0; j < n; j++) {
    		e[j]->origin = p[j];
    		e[j]->nxt = e[(j + 1) % n];
    		e[j]->face = i;
    		auto rev = e[j]->twin;
    		rev->origin = p[(j + 1) % n];
    		rev->face = -1;
    		rev->nxt = e[(j - 1 + n) % n] -> twin;
    		rev->twin = e[j];
    	}
    	for (int i = 0; i < n; i++) {
    		planar.body.push_back(e[i]);
    		planar.body.push_back(e[i]->twin);
    	}
    }
    auto ret = point_location(planar, Q);
    vector<int> ans(q, 0);
    for (int i = 0; i < n; i++) {
    	if (ret[i].first) {
    		if (ret[i].second != -1) {
    			ans[ret[i].second]++;
    		}
    	}
    	else ans[planar.body[ret[i].second]->id]++;
    }
    for (int i = 0; i < q; i++) cout << ans[i] << '\n';
    return 0;
}
// https://acm.timus.ru/problem.aspx?space=1&num=1848