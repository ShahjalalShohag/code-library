#include<bits/stdc++.h>
using namespace std;

/*Given an array a that contains n elements and the
operation op that satisfies associative property:
(x op y) op z=x op (y op z) is true for any x, y, z.

The following implementation of Sqrt Tree can perform the following operations:
build in O(nloglogn),
answer queries in O(1) and update an element in O(sqrt(n)).*/

#define SqrtTreeItem int//change for the type you want

SqrtTreeItem op(const SqrtTreeItem &a, const SqrtTreeItem &b) {
  return a + b; //just change this operation for different problems,no change is required inside the code
}

inline int log2Up(int n) {
  int res = 0;
  while ((1 << res) < n) {
    res++;
  }
  return res;
}
//0-indexed
struct SqrtTree {
  int n, llg, indexSz;
  vector<SqrtTreeItem> v;
  vector<int> clz, layers, onLayer;
  vector< vector<SqrtTreeItem> > pref, suf, between;

  inline void buildBlock(int layer, int l, int r) {
    pref[layer][l] = v[l];
    for (int i = l + 1; i < r; i++) {
      pref[layer][i] = op(pref[layer][i - 1], v[i]);
    }
    suf[layer][r - 1] = v[r - 1];
    for (int i = r - 2; i >= l; i--) {
      suf[layer][i] = op(v[i], suf[layer][i + 1]);
    }
  }

  inline void buildBetween(int layer, int lBound, int rBound, int betweenOffs) {
    int bSzLog = (layers[layer] + 1) >> 1;
    int bCntLog = layers[layer] >> 1;
    int bSz = 1 << bSzLog;
    int bCnt = (rBound - lBound + bSz - 1) >> bSzLog;
    for (int i = 0; i < bCnt; i++) {
      SqrtTreeItem ans;
      for (int j = i; j < bCnt; j++) {
        SqrtTreeItem add = suf[layer][lBound + (j << bSzLog)];
        ans = (i == j) ? add : op(ans, add);
        between[layer - 1][betweenOffs + lBound + (i << bCntLog) + j] = ans;
      }
    }
  }

  inline void buildBetweenZero() {
    int bSzLog = (llg + 1) >> 1;
    for (int i = 0; i < indexSz; i++) {
      v[n + i] = suf[0][i << bSzLog];
    }
    build(1, n, n + indexSz, (1 << llg) - n);
  }

  inline void updateBetweenZero(int bid) {
    int bSzLog = (llg + 1) >> 1;
    v[n + bid] = suf[0][bid << bSzLog];
    update(1, n, n + indexSz, (1 << llg) - n, n + bid);
  }

  void build(int layer, int lBound, int rBound, int betweenOffs) {
    if (layer >= (int)layers.size()) {
      return;
    }
    int bSz = 1 << ((layers[layer] + 1) >> 1);
    for (int l = lBound; l < rBound; l += bSz) {
      int r = min(l + bSz, rBound);
      buildBlock(layer, l, r);
      build(layer + 1, l, r, betweenOffs);
    }
    if (layer == 0) {
      buildBetweenZero();
    } else {
      buildBetween(layer, lBound, rBound, betweenOffs);
    }
  }

  void update(int layer, int lBound, int rBound, int betweenOffs, int x) {
    if (layer >= (int)layers.size()) {
      return;
    }
    int bSzLog = (layers[layer] + 1) >> 1;
    int bSz = 1 << bSzLog;
    int blockIdx = (x - lBound) >> bSzLog;
    int l = lBound + (blockIdx << bSzLog);
    int r = min(l + bSz, rBound);
    buildBlock(layer, l, r);
    if (layer == 0) {
      updateBetweenZero(blockIdx);
    } else {
      buildBetween(layer, lBound, rBound, betweenOffs);
    }
    update(layer + 1, l, r, betweenOffs, x);
  }

  inline SqrtTreeItem query(int l, int r, int betweenOffs, int base) {
    if (l == r) {
      return v[l];
    }
    if (l + 1 == r) {
      return op(v[l], v[r]);
    }
    int layer = onLayer[clz[(l - base) ^ (r - base)]];
    int bSzLog = (layers[layer] + 1) >> 1;
    int bCntLog = layers[layer] >> 1;
    int lBound = (((l - base) >> layers[layer]) << layers[layer]) + base;
    int lBlock = ((l - lBound) >> bSzLog) + 1;
    int rBlock = ((r - lBound) >> bSzLog) - 1;
    SqrtTreeItem ans = suf[layer][l];
    if (lBlock <= rBlock) {
      SqrtTreeItem add = (layer == 0) ? (
                   query(n + lBlock, n + rBlock, (1 << llg) - n, n)
                 ) : (
                   between[layer - 1][betweenOffs + lBound + (lBlock << bCntLog) + rBlock]
                 );
      ans = op(ans, add);
    }
    ans = op(ans, pref[layer][r]);
    return ans;
  }

  inline SqrtTreeItem query(int l, int r) {
    return query(l, r, 0, 0);
  }

  inline void update(int x, const SqrtTreeItem &item) {
    v[x] = item;
    update(0, 0, n, 0, x);
  }

  SqrtTree(const vector<SqrtTreeItem>& a)
    : n((int)a.size()), llg(log2Up(n)), v(a), clz(1 << llg), onLayer(llg + 1) {
    clz[0] = 0;
    for (int i = 1; i < (int)clz.size(); i++) {
      clz[i] = clz[i >> 1] + 1;
    }
    int tllg = llg;
    while (tllg > 1) {
      onLayer[tllg] = (int)layers.size();
      layers.push_back(tllg);
      tllg = (tllg + 1) >> 1;
    }
    for (int i = llg - 1; i >= 0; i--) {
      onLayer[i] = max(onLayer[i], onLayer[i + 1]);
    }
    int betweenLayers = max(0, (int)layers.size() - 1);
    int bSzLog = (llg + 1) >> 1;
    int bSz = 1 << bSzLog;
    indexSz = (n + bSz - 1) >> bSzLog;
    v.resize(n + indexSz);
    pref.assign(layers.size(), vector<SqrtTreeItem>(n + indexSz));
    suf.assign(layers.size(), vector<SqrtTreeItem>(n + indexSz));
    between.assign(betweenLayers, vector<SqrtTreeItem>((1 << llg) + bSz));
    build(0, 0, n, 0);
  }
};
int main() {
  int i, j, k, n, m, q, l, r;
  cin >> n;
  vector<int> v;
  for(i = 0; i < n; i++) cin >> k, v.push_back(k);
  SqrtTree t = SqrtTree(v);
  cin >> q;
  while(q--) {
    cin >> l >> r;
    --l, --r;
    cout << t.query(l, r) << endl;
  }
}
// https://cp-algorithms.com/data_structures/sqrt-tree.html
