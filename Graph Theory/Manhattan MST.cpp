#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

/*
For a given set of n points in 2D plane, it finds a minimum
spanning tree whose edge weight is given by the Manhattan distance:
d(i,j) = |x(i) - x(j)| + |y(i) - y(j)|.
*/

int n;
vector<pair<int, int>> g[N];

struct point {
	int x, y, id;
	bool operator<(const point &p) const { return x == p.x ? y < p.y : x < p.x; }
} p[N];

struct node {
	int val, id;
} t[N];

struct DSU {
	int p[N];
	void init(int n) { for (int i = 1; i <= n; i++)  p[i] = i; }
	int find(int u) { return p[u] == u ? u : p[u] = find(p[u]); }
	void merge(int u, int v) { p[find(u)] = find(v); }
} dsu;

struct edge {
	int u, v, c;
	bool operator < (const edge &p) const {
		return c < p.c;
	}
};
vector<edge> edges;

int query(int x)
{
	int r = 2e9, id = -1;
	for (; x <= n; x += (x & -x)) if (t[x].val < r) r = t[x].val, id = t[x].id;
	return id;
}

void modify(int x, int w, int id)
{
	for (; x > 0; x -= (x & -x)) if (t[x].val > w) t[x].val = w, t[x].id = id;
}

int dist(point &a, point &b)
{
	return abs(a.x - b.x) + abs(a.y - b.y);
}

void add(int u, int v, int c)
{
	edges.push_back({u, v, c});
}

void kruskal()
{
	dsu.init(n);
	sort(edges.begin(), edges.end());
	for (edge e : edges) {
		int u = e.u, v = e.v, c = e.c;
		if (dsu.find(u) != dsu.find(v)) {
			g[u].push_back({v,c});
			g[v].push_back({u,c});
			dsu.merge(u, v);
		}
	}
}

void manhattan()
{
	for (int i = 1; i <= n; ++i) p[i].id = i;
	for (int dir = 1; dir <= 4; ++dir) {
		if (dir == 2 || dir == 4) {
			for (int i = 1; i <= n; ++i) swap(p[i].x, p[i].y);
		} else if (dir == 3) {
			for (int i = 1; i <= n; ++i) p[i].x = -p[i].x;
		}
		sort(p + 1, p + 1 + n);
		vector<int> v; static int a[N];
		for (int i = 1; i <= n; ++i) a[i] = p[i].y - p[i].x, v.push_back(a[i]);
		sort(v.begin(), v.end());
		v.erase(unique(v.begin(), v.end()), v.end());
		for (int i = 1; i <= n; ++i) a[i] = lower_bound(v.begin(), v.end(), a[i]) - v.begin() + 1;
		for (int i = 1; i <= n; ++i) t[i].val = 2e9, t[i].id = -1;
		for (int i = n; i >= 1; --i) {
			int pos = query(a[i]);
			if (pos != -1) add(p[i].id, p[pos].id, dist(p[i], p[pos]));
			modify(a[i], p[i].x + p[i].y, i);
		}
	}
}

int main()
{
	scanf("%d", &n);
	for(int i = 1; i <= n; i++) scanf("%d%d", &p[i].x, &p[i].y);
	manhattan();
	kruskal();
	//g = manhattan mst adjacency list and corresponding costs
	return 0;
}
