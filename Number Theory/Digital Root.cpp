/*
For any number n > 9, the digital root is equivalent to 1 + ((n - 1) % 9). This is because numbers 1 through 9 have a digital root which is the number itself, and the pattern then repeats from 10 onwards, with 10 having a digital root of 1 again, 11 having a digital root of 2, and so on.

Therefore, the solution directly calculates this value without the need to iteratively add the digits of the number. It handles the edge case of 0 separately, as its digital root should be 0. The given code uses this principle to return the digital root in a single line:

If num is 0, return 0.
If num is not 0, apply the digital root formula: (num - 1) % 9 + 1.
*/
#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    // Function to add the digits of a number until the result is a single digit.
    int addDigits(int num) {
        // Using Digital Root formula which is a well-known and proven method for this problem.
        // If the number is 0, the digital root is 0.
        if (num == 0) { 
            return 0; 
        }
        // For all other numbers, the digital root is the remainder when the number
        // is divided by 9, with a special case when num is a multiple of 9.
        else {
            // Subtract 1 from num to handle the multiple of 9 scenario, then
            // mod by 9 to get the remainder, and add 1 to adjust for the initial subtraction.
            return (num - 1) % 9 + 1;
        }
    }
};

// https://leetcode.com/problems/add-digits/

// Reference: https://algo.monster/liteproblems/258
