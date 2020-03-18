#include<bits/stdc++.h>
using namespace std;

#define ll long long
#define eb emplace_back
#define nl '\n'
#define deb(x) cerr << #x" = " << x << nl
#define in() ( { int a ; scanf("%d", &a); a; } )

const int N = 1e5 + 9;
const int mod = 1e9 + 7;

struct Line {
    ll k, d;
    ll eval(int x) {
        return k * x + d;
    }
};

struct LiChaoNode {
    Line line;
    LiChaoNode* left = nullptr;
    LiChaoNode* right = nullptr;

    LiChaoNode(Line line) : line(line) {}

    void add_line(Line nw, int l, int r) {
        int m = (l + r) / 2;
        bool lef = nw.eval(l) < line.eval(l);
        bool mid = nw.eval(m) < line.eval(m);
        if(mid) swap(line, nw);
        if(l == r) return;
        if(lef != mid) {
            if(left == nullptr) left = new LiChaoNode(nw);
            else left -> add_line(nw, l, m);
        }
        else{
            if(right == nullptr) right = new LiChaoNode(nw);
            else right -> add_line(nw, m + 1, r);
        }
    }

    ll query(int x, int l, int r) {
        ll val = line.eval(x);
        int m = (l + r) / 2;
        if(l < r) {
            if(x <= m && left != nullptr) val = min(val, left -> query(x, l, m));
            if(x > m && right != nullptr) val = min(val, right -> query(x, m + 1, r));
        }
        return val;
    }
};

struct LiChaoTree
{
    int L, R;
    LiChaoNode* root;

    LiChaoTree() : L(numeric_limits<int>::min() / 2), R(numeric_limits<int>::max() / 2), root(nullptr) {}
    LiChaoTree(int L, int R) : L(L), R(R) {
        root = new LiChaoNode({0, numeric_limits<ll>::max() / 2});
    }

    void add_line(Line line) {
        root -> add_line(line, L, R);
    }
    ll query(int x) {
        return root -> query(x, L, R);
    }
};


int a[N], b[N];
int32_t main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n; cin >> n;
    for(int i = 0; i < n; i++) cin >> a[i];
    for(int i = 0; i < n; i++) cin >> b[i];
    LiChaoTree t = LiChaoTree(0, (int) 1e9);
    t.add_line({b[0], 0});
    ll ans = 0;
    for(int i = 1; i < n; i++){
        ans = t.query(a[i]);
        t.add_line({b[i], ans});
    }
    cout << ans << nl;
    return 0;
}
