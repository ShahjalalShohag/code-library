#include<bits/stdc++.h>
using namespace std;

#define pii pair<int,int>
#define pll pair<ll,ll>
#define eb emplace_back
#define ll long long
#define nl '\n'
#define deb(x) cerr<<#x" = "<<x<<nl
#define in() ( { int a ; scanf("%d",&a); a; } )

const int N = 3e5 + 9;
const int mod = 1e9 + 7;

///An element Z[i] of Z array stores length of the longest substring
///starting from str[i] which is also a prefix of str[0..n-1].
///The first entry of Z array is meaning less as complete string is always prefix of itself.
///Here Z[0]=0.
vector<int> z_function(string s) {
    int n = (int) s.length();
    vector<int> z(n);
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i <= r)
            z[i] = min (r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            ++z[i];
        if (i + z[i] - 1 > r)
            l = i, r = i + z[i] - 1;
    }
    return z;
}
int32_t main()
{
    string s;
    cin>>s;
    vector<int>  ans=z_function(s);
    for(auto x:ans) cout<<x<<' ';
    return 0;
}
