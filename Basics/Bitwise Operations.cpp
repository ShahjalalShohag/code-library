#include<bits/stdc++.h>
using namespace std;

// Checks if kth bit of x is set (1) or not (0)
int check_kth_bit(int x, int k) {
  return (x >> k) & 1;
}

// Prints the positions of all set (1) bits in binary representation of x
void print_on_bits(int x) {
  for (int k = 0; k < 32; k++) {
    if (check_kth_bit(x, k)) {
      cout << k << ' '; // prints the position of the set bit
    }
  }
  cout << '\n';
}

// Returns the count of set (1) bits in binary representation of x
int count_on_bits(int x) {
  int ans = 0;
  for (int k = 0; k < 32; k++) {
    if (check_kth_bit(x, k)) {
      ans++;
    }
  }
  return ans;
}

// Checks if x is even or odd
bool is_even(int x) {
  if (x & 1) {
    return false;
  }
  else {
    return true;
  }
}

// Sets the kth bit of x to 1 and returns the result
int set_kth_bit(int x, int k) {
  return x | (1 << k);
}

// Sets the kth bit of x to 0 and returns the result
int unset_kth_bit(int x, int k) {
  return x & (~(1 << k));
}

// Toggles the kth bit of x and returns the result
int toggle_kth_bit(int x, int k) {
  return x ^ (1 << k);
}

// Checks if x is a power of 2
bool check_power_of_2(int x) {
  return count_on_bits(x) == 1;
}

int main() {
  // Bitwise AND (&)
  int and_result = 12 & 25;  // 12 (binary 1100) & 25 (binary 11001) = 8 (binary 1000)
  cout << "AND result: " << and_result << '\n'; // Output: 8

  // Bitwise OR (|)
  int or_result = 12 | 25;  // 12 (binary 1100) | 25 (binary 11001) = 29 (binary 11101)
  cout << "OR result: " << or_result << '\n'; // Output: 29

  // Bitwise XOR (^)
  int xor_result = 12 ^ 25;  // 12 (binary 1100) ^ 25 (binary 11001) = 21 (binary 10101)
  cout << "XOR result: " << xor_result << '\n'; // Output: 21

  // Bitwise NOT (~)
  int not_result = ~12;
  cout << "NOT result: " << not_result << '\n'; // Output: -13

  // Left shift (<<)
  int left_shift_result = 3 << 2;  // 3 (binary 11) << 2 = 12 (binary 1100)
  cout << "Left shift result: " << left_shift_result << '\n'; // Output: 12

  // Right shift (>>)
  int right_shift_result = 12 >> 2;  // 12 (binary 1100) >> 2 = 3 (binary 11)
  cout << "Right shift result: " << right_shift_result << '\n'; // Output: 3

  // Difference between 1 << x and 1LL << x
  int x = 31;
  long long res1 = 1 << x;  // This can lead to overflow if x is large
  long long res2 = 1LL << x;  // This avoids overflow since we're shifting on a long long
  cout << "1 << x result: " << res1 << '\n';  // Output: -2147483648 (due to overflow)
  cout << "1LL << x result: " << res2 << "\n\n";  // Output: 2147483648 (correct value)


  x = 11; // binary representation: 1011

  cout << "Check 2nd bit of 11: " << check_kth_bit(x, 2) << '\n'; // Output: 0
  cout << "Set bits in 11 are at positions: ";
  print_on_bits(x); // Output: 0 1 3
  cout << "Number of set bits in 11: " << count_on_bits(x) << '\n'; // Output: 3
  cout << "Is 11 even? " << is_even(x) << '\n'; // Output: 0 (false)
  cout << "11 after setting 2nd bit: " << set_kth_bit(x, 2) << '\n'; // Output: 15
  cout << "15 after unsetting 2nd bit: " << unset_kth_bit(15, 2) << '\n'; // Output: 11
  cout << "11 after toggling 3rd bit: " << toggle_kth_bit(x, 3) << '\n'; // Output: 3
  cout << "Is 8 a power of 2: " << check_power_of_2(8) << '\n'; // Output: 1 (true)

  return 0;
}
