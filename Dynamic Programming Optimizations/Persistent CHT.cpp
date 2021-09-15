struct PT {
  int x, y;
  PT(int x = 0, int y = 0): x(x), y(y) {}
  friend ll dot(PT &a, PT &b) {
    return 1ll * a.x * b.x + 1ll * a.y * b.y;
  }
  friend int orientation(PT &a, PT &b, PT &c) {
    ll s = 1ll * (b.x - a.x) * (c.y - a.y) - 1ll * (b.y - a.y) * (c.x - a.x);
    return (s > 0) - (s < 0);
  }
};
struct PersistentCHT { //minimizes dot product
  const static int N = 1e6 + 6;
  const static int lg = 22;
  int p[N][lg], sz;
  PT pnt[N];
  int insert(PT a, int rt) {
    for (int u, v, i = lg - 1; i >= 0; i--)
      if ((u = p[rt][i]) && (v = p[u][0]) && orientation(pnt[v], pnt[u], a) <= 0)
        rt = u;
    if (p[rt][0] && orientation(pnt[p[rt][0]], pnt[rt], a) <= 0) rt = p[rt][0];
    pnt[++sz] = a;
    p[sz][0] = rt;
    for (int i = 1; i < lg; i++) p[sz][i] = p[p[sz][i - 1]][i - 1];
    return sz;
  }
  ll query(PT a, int rt) {
    for (int u, v, i = lg - 1; i >= 0; i--)
      if ((u = p[rt][i]) && (v = p[u][0]) && dot(a, pnt[v]) > dot(a, pnt[u]))
        rt = u;
    if (p[rt][0] && dot(a, pnt[p[rt][0]]) > dot(a, pnt[rt])) rt = p[rt][0];
    return rt ? dot(a, pnt[rt]) : -1e18;
  }
} cht;
