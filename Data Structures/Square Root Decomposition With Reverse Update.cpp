#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9, B = 330;

int a[N];
int t[4000][4000], z[4000];
int blocks[3500], bid;
int L[N], R[N], T, n, assign[N], rev[N];
int create_block(int l, int r, int as) {
  ++T;
  z[T] = 0;
  L[T] = l; R[T] = r;
  if (!as) {
    for (int i = l; i <= r; i++) {
      t[T][z[T]++] = a[i];
    }
    sort(t[T], t[T] + z[T]);
    assign[T] = 0;
  }
  else {
    assign[T] = as;
  }
  rev[T] = 0;
  return T;
}
void rebuild() {
  T = 0; bid = 0;
  for (int i = 1; i <= n; i += B) {
    int l = i, r = min(i + B - 1, n);
    blocks[bid++] = create_block(l, r, 0);
  }
}
int nxt[3500], nid, fresh[5], fid;
void update_blocks(int l, int r) {
  int cur = 1;
  nid = 0;
  for (int w = 0; w < bid; w++) {
    int id = blocks[w];
    int cl = cur, cr = cur + R[id] - L[id];
    if (cl >= l && cr <= r) {
      nxt[nid++] = id;
    }
    else {
      int le = max(cl, l), ri = min(cr, r);
      if (le <= ri) {
        int realL = L[id] + le - cl;
        int realR = L[id] + ri - cl;
        if (rev[id]) {
          realR = R[id] - (le - cl);
          realL = R[id] - (ri - cl);
        }
        fid = 0;
        if (rev[id]) {
          if (realR < R[id]) {
            fresh[fid++] = create_block(realR + 1, R[id], assign[id]);
          }
          fresh[fid++] = create_block(realL, realR, assign[id]);
          if (realL > L[id]) {
            fresh[fid++] = create_block(L[id], realL - 1, assign[id]);
          }
        }
        else {
          if (realL > L[id]) {
            fresh[fid++] = create_block(L[id], realL - 1, assign[id]);
          }
          fresh[fid++] = create_block(realL, realR, assign[id]);
          if (realR < R[id]) {
            fresh[fid++] = create_block(realR + 1, R[id], assign[id]);
          }
        }
        for (int w = 0; w < fid; w++) {
          int x = fresh[w];
          nxt[nid++] = x;
          rev[x] = rev[id];
          assign[x] = assign[id];
        }
      }
      else {
        nxt[nid++] = id;
      }
    }
    cur = cr + 1;
  }
  for (int i = 0; i < nid; i++) {
    blocks[i] = nxt[i];
  }
  bid = nid;
}
int ty[N], l[N], r[N], x[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  cin >> n;
  vector<int> v;
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
    v.push_back(a[i]);
  }
  int q; cin >> q;
  for (int i = 1; i <= q; i++) {
    string p; cin >> p;
    if (p == "get") {
      ty[i] = 3;
      cin >> l[i] >> r[i] >> x[i];
      v.push_back(x[i]);
    }
    else if (p == "set") {
      ty[i] = 1;
      cin >> l[i] >> r[i] >> x[i];
      v.push_back(x[i]);
    }
    else {
      ty[i] = 2;
      cin >> l[i] >> r[i];
    }
  }
  sort(v.begin(), v.end());
  v.erase(unique(v.begin(), v.end()), v.end());
  for (int i = 1; i <= n; i++) {
    a[i] = upper_bound(v.begin(), v.end(), a[i]) - v.begin();
  }
  for (int i = 1; i <= q; i++) {
    if (ty[i] != 2) {
      x[i] = upper_bound(v.begin(), v.end(), x[i]) - v.begin();
    }
  }
  vector<int> b(n + 1);
  for (int i = 1; i <= n; i++) {
    b[i] = a[i];
  }
  for (int i = 1; i <= q; i++) {
    if ((i - 1) % 600 == 0) {
      int cur = 1;
      for (int w = 0; w < bid; w++) {
        int id = blocks[w];
        int cl = cur, cr = cur + R[id] - L[id];
        for (int i = cl; i <= cr; i++) {
          int rid = rev[id] ? R[id] - (i - cl) : L[id] + (i - cl);
          b[i] = assign[id] ? assign[id] : a[rid];
        }
        cur = cr + 1;
      }
      for (int i = 1; i <= n; i++) {
        a[i] = b[i];
      }
      rebuild();
    }
    update_blocks(l[i], r[i]);
    if (ty[i] == 1) {
      int cur = 1;
      for (int w = 0; w < bid; w++) {
        int id = blocks[w];
        int cl = cur, cr = cur + R[id] - L[id];
        if (cl >= l[i] && cr <= r[i]) {
          assign[id] = x[i];
        }
        cur = cr + 1;
      }
    }
    else if (ty[i] == 3) {
      int cur = 1, ans = 0;
      for (int w = 0; w < bid; w++) {
        int id = blocks[w];
        int cl = cur, cr = cur + R[id] - L[id];
        if (cl >= l[i] && cr <= r[i]) {
          if (assign[id]) {
            ans += (R[id] - L[id] + 1) * (assign[id] >= x[i]);
          }
          else ans += z[id] - (lower_bound(t[id], t[id] + z[id], x[i]) - t[id]);
        }
        cur = cr + 1;
      }
      cout << ans << '\n';
    }
    else {
      int cur = 1, st = N, en = 0;
      for (int j = 0; j < bid; j++) {
        int cl = cur, cr = cur + R[blocks[j]] - L[blocks[j]];
        if (cl >= l[i] && cr <= r[i]) {
          st = min(st, j);
          en = j;
          rev[blocks[j]] ^= 1;
        }
        cur = cr + 1;
      }
      reverse(blocks + st, blocks + en + 1);
    }
  }
  return 0;
}
// https://www.e-olymp.com/en/problems/2961