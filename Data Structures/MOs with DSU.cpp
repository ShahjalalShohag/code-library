#include <bits/stdc++.h>
#define endl '\n'

using namespace std;

//Given n vertices and m edges perform q queries of type (l,r)
//output the number of connected components if we added edges i such that l<=i<=r
//This code runs for 1<=n,m,q<=2e5 in 2 second
const int N = 2e5 + 9;
struct query {
  int l, r, idx;
  query() {
    l = 0;
    r = 0;
    idx = 0;
  }
  query(int _l, int _r, int _idx) {
    l = _l;
    r = _r;
    idx = _idx;
  }
};

struct persistent_dsu {
  struct state {
    int u, ru, v, rv;
    state() {
      u = 0;
      ru = 0;
      v = 0;
      rv = 0;
    }
    state(int _u, int _ru, int _v, int _rv) {
      u = _u;
      ru = _ru;
      v = _v;
      rv = _rv;
    }
  };

  int cnt;
  int depth[N], par[N];
  stack<state> st;

  persistent_dsu() {
    cnt = 0;
    memset(depth, 0, sizeof(depth));
    memset(par, 0, sizeof(par));
    while(!st.empty()) st.pop();
  }

  void init(int _sz) {
    cnt = _sz;
    for(int i = 0; i <= _sz; i++)
      par[i] = i, depth[i] = 1;
  }

  int root(int x) {
    if(x == par[x]) return x;
    return root(par[x]);
  }

  bool connected(int x, int y) {
    return root(x) == root(y);
  }

  void unite(int x, int y) {
    int rx = root(x), ry = root(y);
    if(rx == ry) return;

    if(depth[rx] < depth[ry])
      par[rx] = ry;
    else if(depth[ry] < depth[rx])
      par[ry] = rx;
    else par[rx] = ry, depth[ry]++;

    cnt--;
    st.push(state(rx, depth[rx], ry, depth[ry]));

  }

  void snapshot() {
    st.push(state(-1, -1, -1, -1));
  }

  void rollback() {
    while(!st.empty()) {
      if(st.top().u == -1)
        return;

      ++cnt;
      par[st.top().u] = st.top().u;
      par[st.top().v] = st.top().v;
      depth[st.top().u] = st.top().ru;
      depth[st.top().v] = st.top().rv;
      st.pop();
    }
  }
};

struct edge {
  int u, v;
  edge() {
    u = 0;
    v = 0;
  }
  edge(int _u, int _v) {
    u = _u;
    v = _v;
  }
};

int n, ed, m;
edge a[N];
query q[N];

void read() {
  cin >> n >> ed >> m;

  for(int i = 1; i <= ed; i++) {
    int u, v;
    cin >> u >> v;
    a[i] = edge(u, v);
  }
}

int rt, cnt_q;
persistent_dsu d;

bool cmp(query fir, query sec) {
  if(fir.l / rt != sec.l / rt) return fir.l / rt < sec.l / rt;
  return fir.r < sec.r;
}

int answer[N];
void add(int idx) {
  d.unite(a[idx].u, a[idx].v);
}

void solve() {
  d.init(n);
  d.snapshot();
  rt = sqrt(ed);
  cnt_q = 0;

  int fm = m;
  for(int i = 0; i < m; i++) {
    int l, r;
    cin >> l >> r;

    if(r - l + 1 <= rt) {
      for(int k = l; k <= r; k++) add(k);
      answer[i] = d.cnt;
      d.rollback();
      continue;
    }

    q[cnt_q++] = query(l, r, i);
  }

  m = cnt_q;
  sort(q, q + m, cmp);
  int last, border, last_block = -1, block;

  for(int i = 0; i < m; i++) {
    block = q[i].l / rt;
    if(last_block != block) {
      d.init(n);
      border = rt * (block + 1);
      last = border;
    }

    last_block = block;
    for(int k = last + 1; k <= q[i].r; k++) add(k);
    d.snapshot();

    for(int k = q[i].l; k <= border; k++) add(k);
    answer[q[i].idx] = d.cnt;
    d.rollback();

    last = q[i].r;

  }

  for(int i = 0; i < fm; i++)
    cout << answer[i] << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  while(t--) {
    read();
    solve();
  }
  return 0;
}
// https://www.codechef.com/problems/GERALD07
