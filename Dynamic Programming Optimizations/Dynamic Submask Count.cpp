#include<bits/stdc++.h>
using namespace std;
const int N = (1 << 16) + 9;
/**
You have a list of integers, initially the list is empty.
You have to process  operations of three kinds:

 add s: Add integer  to your list, note that an integer can exist more than one time in the list
 del s: Delete one copy of integer  from the list,
        it's guaranteed that at least one copy of  will exist in the list.
 cnt s: Count how many integers  are there in the list such that a&s =a
        i.e. the number of submasks of s


let's look at the numbers in their binary representation ,
since all numbers are less than 2^16 then binary representation
of each number will have at most 16 bits
note that A and B = A if and only if for every i: if the i'th bit of A is 1
then i'th bit of B is 1

so if i'th bit of B is 0 , then i'th bit of A must be 0 ,
 but if i'th bit of B is 1 then i'th bit of A can be 0 or 1
so for a fixed B the number of numbers A which satisify A and B = A is 2^(number of 1 bits in B)

so let's try this solution:
let's have an array C with indices from 0 to 2^16-1 , initially all values are zero
whenever we have to add a number S then we increment the value of C[S] by 1
whenever we have to do cnt operation with the given number S ,
then generate all numbers X such that X and S = X
and compute the sum of C[X] for all numbers X


add operation is O(1) but cnt operation is O(2^16) worst case so
the solution will not fit in time limit

let's try another solution:
we have the same array C as previous solution
during add S operation we generate all numbers X such that X and S = S
and for each X we increment C[x] by 1
during cnt S operation we only output C[S]

this solution have O(2^16) for add operation worst case but for
cnt operation is it O(1) so this solution is slow in worst case too

now we have two solutions , the first one is slow on cnt operations
and the second is slow on add operations
so let's try to combine both solution to get a solution that
is fast on both add and cnt operations

we have to use two arrays C and D , each with indices between 0 and 2^16-1

during add S operation:
if S have more than 8 1-bits then generate all numbers X such that S and X = S
for each X , add one to C[X]
this is O(8 * 2^8) worst case
if S have at most 8 1-bits then just add one to D[X]
this is also O(1)

during cnt S operation:
if S have at most 8 1-bits then the numbers that we want to
count will have at most 8 1-bits so we deal only with array D,
generate all numbers X such that X and S = X
then find the sum of values of D[x] and output it
this is O(8 * 2^8)


if S have more than 8 1-bits then we need to count numbers that  have more than
8 1-bits and numbers that have at most 8 1-bits saperately
first, the number of numbers that have more than 8 1-bits is just C[S]

second, to count the numbers that have at most 8 1-bits we need to
count the numbers X in the list that have at most 8 1-bits that doesn't satisfy X and S = X
then subtract it from total number of numbers in our list that have at most 8 1-bits
now to count the numbers X that doesn't satisify X and S = X  we
flip the digits of S (change every 0 to 1 and every 1 to 0)
(note that S become to have at most 8 1-bits)

then count the numbers X that have at least one common 1-bit with S by
inclusion - exclusion principle
this step is O(8 * 2^8)
now sum up two values to have final answer
in total cnt operation is O(8 * 2^8) worst case

so our solution now run in total complexity O(8 * 2^8 * Q)

final note: dealing with delete operation is the same as adding operation
but after changing any step that add a value to an array or variable
we subtract that value instead
**/

int c[N], small[N], big[N];
int main() {
  string s;
  int i, j, k, n, m, q, tot = 0;
  cin >> q;
  while(q--) {
    cin >> s >> n;
    if(s[0] == 'a') {
      if(__builtin_popcount(n) <= 8) {
        small[n]++;
        for(i = n; i > 0; i = (i - 1)&n) c[i]++;
        tot++;
      } else {
        vector<int> v;
        for(i = 0; i < 16; i++) if(!((n >> i) & 1)) v.push_back(i);
        k = v.size();
        for(i = 0; i < (1 << k); i++) {
          int val = n;
          for(j = 0; j < k; j++) if((i >> j) & 1) {
              val |= (1 << v[j]);
            }
          big[val]++;
        }
      }
    } else if(s[0] == 'd') {
      if(__builtin_popcount(n) <= 8) {
        small[n]--;
        for(i = n; i > 0; i = (i - 1)&n) c[i]--;
        tot--;
      } else {
        vector<int> v;
        for(i = 0; i < 16; i++) if(!((n >> i) & 1)) v.push_back(i);
        k = v.size();
        for(i = 0; i < (1 << k); i++) {
          int val = n;
          for(j = 0; j < k; j++) if((i >> j) & 1) {
              val |= (1 << v[j]);
            }
          big[val]--;
        }
      }
    } else {
      if(__builtin_popcount(n) <= 8) {
        int ans = 0;
        for(i = n; i > 0; i = (i - 1)&n) ans += small[i];
        ans += small[0];
        cout << ans << endl;
      } else {
        int ans = big[n];
        vector<int> v;
        for(i = 0; i < 16; i++) if(!((n >> i) & 1)) v.push_back(i);
        k = v.size();
        int nw = 0;
        for(i = 1; i < (1 << k); i++) {
          int val = 0, cnt = 0;
          for(j = 0; j < k; j++) if((i >> j) & 1) {
              val |= (1 << v[j]);
              cnt++;
            }
          if(cnt & 1) nw += c[val];
          else nw -= c[val];
        }
        ans += tot - nw;
        cout << ans << endl;
      }
    }
  }
  return 0;
}
// https://www.hackerrank.com/contests/countercode/challenges/subset
