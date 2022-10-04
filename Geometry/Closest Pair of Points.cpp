#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;
#define x first
#define y second
long long dist2(pair<int, int> a, pair<int, int> b) {
	return 1LL * (a.x - b.x) * (a.x - b.x) + 1LL * (a.y - b.y) * (a.y - b.y);
}
pair<int, int> closest_pair(vector<pair<int, int>> a) {
	int n = a.size();
	assert(n >= 2);
	vector<pair<pair<int, int>, int>> p(n);
	for (int i = 0; i < n; i++) p[i] = {a[i], i};
	sort(p.begin(), p.end());
	int l = 0, r = 2;
	long long ans = dist2(p[0].x, p[1].x);
	pair<int, int> ret = {p[0].y, p[1].y};
	while (r < n) {
		while (l < r && 1LL * (p[r].x.x - p[l].x.x) * (p[r].x.x - p[l].x.x) >= ans) l++;
		for (int i = l; i < r; i++) {
			long long nw = dist2(p[i].x, p[r].x);
			if (nw < ans) {
				ans = nw;
				ret = {p[i].y, p[r].y};
			}
		}
		r++;
	}
	return ret;
}
int32_t main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	int n; cin >> n;
	vector<pair<int, int>> p(n);
	for (int i = 0; i < n; i++) cin >> p[i].x >> p[i].y;
	pair<int, int> z = closest_pair(p);
	if (z.x > z.y) swap(z.x, z.y);
	cout << z.x << ' ' << z.y << ' ' << fixed << setprecision(6) << sqrtl(dist2(p[z.x], p[z.y])) << '\n';
  return 0;
}
