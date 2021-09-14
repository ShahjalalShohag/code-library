/**
if problems have following characteristics-
    1.it can be solved offline
    2.if every insert operations were before query operations
    each query operations could have been solved in O(L)(i mean faster like logn)
    3.query operation is cumulative i.e. we don't need every insert at once to solve this query
this type of problem can be solved using divide and conquer
complexity: nlogn*O(L)
**/
Problem:

    You have an empty set. You need to perform following operations –
    Insert a given number X in the set.
    Count how many numbers in the set is less than or equal to a given number X

Solution:

    op[1...n] = operations
    ans[1...n] = array to store answers
    solve(l, r) {
      if op[l...r] has no queries: return
      m = (l + r) / 2
      ds = statically built DS using insert operations in op[l...m]
      for each query operation i in op[m+1...r]:
        ans[i] += ds.query(op[i])
      solve(l, m)
      solve(m+1, r)
    }

/**
some dp can be solved using this trick
**/
Problem:

    Lets have a look at the LIS problem. it has following solution-
    for(int i = 1; i <= n; i++) {
      for(int j = 1; j < i; j++) if(a[j] < a[i])
        dp[i] = max(dp[i], dp[j] + 1)
    }
    it can modeled like update-query problem
    for(int i = 1; i <= n; i++) {
      dp[i] = query(a[i]);
      insert({a[i], dp[i]});
    }

Solution:

    solve(l, r) {
      m = (l + r) / 2
      solve(l, m)
      make ds with a[l..m] and dp[l..m]
      update dp[m+1..r] using the ds
      solve(m+1, r)
    }
