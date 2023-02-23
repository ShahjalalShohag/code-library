#include<bits/stdc++.h>
using namespace std;

const int N = 105;
// https://en.wikipedia.org/wiki/Negative_base
int decimal(string s) {
  int ans = 0, cur = 1, n = s.size();
  for (int i = n - 1; i >= 0; i--) {
    ans += (s[i] - '0') * cur;
    cur *= -2;
  }
  return ans;
}
string nega_binary(int x) {
  if (x == 0) return "0";
  string s = "";
  while (x) {
    int rem = x % (-2);
    x /= -2;
    if (rem < 0) {
      rem += 2; x += 1;
    }
    s.push_back(char(rem + '0'));
  }
  reverse(s.begin(), s.end());
  return s;
}
void format(string &a) {
  reverse(a.begin(), a.end());
  while (!a.empty() and a.back() == '0') {
    a.pop_back();
  }
}
void format(string &a, string &b) { // make them of equal size
  int n = max(a.size(), b.size());
  reverse(a.begin(), a.end());
  while (a.size() < n) {
    a.push_back('0');
  }
  reverse(a.begin(), a.end());
  reverse(b.begin(), b.end());
  while (b.size() < n) {
    b.push_back('0');
  }
  reverse(b.begin(), b.end());
}
bool is_equal(string a, string b) {
  format(a, b);
  return a == b;
}
bool is_zero(string s) {
  format(s);
  return s == "0";
}
bool is_negative(string s) {
  format(s);
  if (is_zero(s)) return false;
  return (int)s.size() % 2 == 0;
}
// return -s
string negate_number(string s) { // convert 01 -> 11 and 11 -> 01 whenever possible
  string ans = "";
  int n = s.size(), carry = 0;
  for (int i = 0, j; i < n; i = j) {
    if (s[i] == '0') {
      ans += '0';
      j = i + 1;
      continue;
    }
    for (j = i; j < n and s[i] == s[j]; j++);
    int cnt = j - i;
    if (cnt & 1) {
      if (!ans.empty()) {
        assert(ans.back() == '0');
        ans.pop_back();
      }
      cnt--;
      ans.push_back('1');
      ans.push_back('1');
    }
    while (cnt) {
      ans.push_back('0');
      ans.push_back('1');
      cnt -= 2;
    }
  }
  return ans;
}
string add(string a, string b) {
  format(a, b);
  int n = a.size();
  string ans;
  int carry = 0;
  for (int i = n - 1; i >= 0; i--) {
    int x = a[i] - '0', y = b[i] - '0';
    int sum = x + y + carry;
    if (sum == 0 or sum == 1) {
      ans.push_back(char(sum + '0'));
      carry = 0;
    }
    else if (sum == 2) {
      ans.push_back('0');
      carry = -1;
    }
    else if (sum == -1) {
      ans.push_back('1');
      carry = 1;
    }
    else {
      assert(sum == 3);
      ans.push_back('1');
      carry = -1;
    }
  }
  if (carry) {
    if (carry == 1) {
      ans.push_back('1');
    }
    else {
      assert(carry == -1);
      ans.push_back('1');
      ans.push_back('1');
    }
  }
  reverse(ans.begin(), ans.end());
  return ans;
}
string subtract(string a, string b) {
  return add(a, negate_number(b));
}
string divide_by_2_floor(string s) { // floor division
  s.pop_back();
  if (s.empty()) s = "0";
  return negate_number(s);
}
string divide_by_2_ceil(string s) { // ceil division
  s = add(s, nega_binary(1));
  return divide_by_2_floor(s);
}
string multiply_by_2(string s) {
  s = negate_number(s);
  s.push_back('0');
  return s;
}
map<int, string> bin;
map<int, int> vis;
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  for (int i = -N; i < N; i++) {
    vis[i] = 1;
    bin[i] = nega_binary(i);
    assert(decimal(bin[i]) == i);
  }
  for (int i = 0; i < N; i++) {
    if (vis[i] and vis[-i]) {
      string s = bin[i];
      string t = bin[-i];
      // cout << negate_number(s) << ' ' << t << '\n';
      assert(is_equal(negate_number(s), t));
    }
  }
  for (int i = -N; i < N; i++) {
    for (int j = -N; j < N; j++) {
      if (vis[i] and vis[j] and vis[i + j]) {
        // cout << bin[i + j] << ' ' << add(bin[i], bin[j]) << '\n';
        assert(is_equal(bin[i + j], add(bin[i], bin[j])));
      }
    }
  }
  for (int i = -N; i < N; i++) {
    // if (vis[i] and vis[i * 2]) {
    //   assert(is_equal(bin[i * 2], multiply_by_2(bin[i])));
    // }
    // int flr = i >= 0 ? i / 2 : (i - 1) / 2;
    // if (vis[i] and vis[flr]) {
    //   assert(is_equal(bin[flr], divide_by_2_floor(bin[i])));
    // }
    int cl = i >= 0 ? (i + 1) / 2 : i / 2;
    if (vis[i] and vis[cl]) {
      // cout << i << ' ' << cl << ' ' << bin[cl] << ' ' << divide_by_2_ceil(bin[i]) << '\n';
      assert(is_equal(bin[cl], divide_by_2_ceil(bin[i])));
    }
  }
  return 0;
}
