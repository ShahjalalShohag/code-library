#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;
struct aho_corasick_static {
  int cnt[N], link[N], psz;
  map<char, int> to[N];

  void clear() {
    for(int i = 0; i < psz; i++)
      cnt[i] = 0, link[i] = -1, to[i].clear();

    psz = 1;
    link[0] = -1;
    cnt[0] = 0;
  }

  aho_corasick_static() {
    psz = N - 2;
    clear();
  }

  void add_word(string s) {
    int u = 0;
    for(char c : s) {
      if(!to[u].count(c)) to[u][c] = psz++;
      u = to[u][c];
    }

    cnt[u]++;
  }

  void push_links() {
    queue<int> Q;
    int u, v, j;
    char c;

    Q.push(0);
    link[0] = -1;

    while(!Q.empty()) {
      u = Q.front();
      Q.pop();

      for(auto it : to[u]) {
        v = it.second;
        c = it.first;
        j = link[u];

        while(j != -1 && !to[j].count(c)) j = link[j];
        if(j != -1) link[v] = to[j][c];
        else link[v] = 0;

        cnt[v] += cnt[link[v]];
        Q.push(v);
      }
    }
  }

  int get_count(string p) {
    int u = 0, ans = 0;
    for(char c : p) {
      while(u != -1 && !to[u].count(c)) u = link[u];
      if(u == -1) u = 0;
      else u = to[u][c];
      ans += cnt[u];
    }

    return ans;
  }
};

struct aho_corasick {
  vector<string> li[20];
  aho_corasick_static ac[20];

  void clear() {
    for(int i = 0; i < 20; i++) {
      li[i].clear();
      ac[i].clear();
    }
  }

  aho_corasick() {
    clear();
  }

  void add_word(string s) {
    int pos = 0;
    for(int l = 0; l < 20; l++)
      if(li[l].empty()) {
        pos = l;
        break;
      }

    li[pos].push_back(s);
    ac[pos].add_word(s);

    for(int bef = 0; bef < pos; bef++) {
      ac[bef].clear();
      for(string s2 : li[bef]) {
        li[pos].push_back(s2);
        ac[pos].add_word(s2);
      }

      li[bef].clear();
    }

    ac[pos].push_links();
  }
  //sum of occurrences of all patterns in this string
  int get_count(string s) {
    int ans = 0;
    for(int l = 0; l < 20; l++)
      ans += ac[l].get_count(s);

    return ans;
  }
};

string s[N];
aho_corasick aho;

int main() {
  int i, j, k, n, m;
  cin >> n;
  for(int i = 0; i < n; i++)
    cin >> s[i];
  aho.clear();
  for(int i = 0; i < n; i++) {
    aho.add_word(s[i]);
    cout << aho.get_count("aaaaaasssaaa") << endl;
  }
  return 0;
}
