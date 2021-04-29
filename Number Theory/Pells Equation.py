import sys
from math import sqrt

# continued fraction of sqrt(x)
# there will always be a cycle of size <= 2 * sqrt(x) in the continued fraction
def get_continued_fraction(ans, x):
  a0 = int(sqrt(x))
  n, m, a = 1, 0, 0
  f = True
  if (a0 * a0 != x):
    while (a != 2 * a0):
      m = n * a - m;
      n = (x - m * m) / n;
      a = int((a0 + m) / n);
      if not f:
        ans.append(a)
      f = False

# generating (x, y)
def gen(a, x, y, i, cur):
  if (i >= cur):
    return x * a[i] + 1, a[i]
  else:
    rx, ry = gen(a, y, a[i + 1], i + 1, cur)
    x = x * rx + ry
    y = rx
    return x, y

# generate the solutions to the equation x^2 - n*y^2 = 1
# (1, 0) is always a solution, so try to generate a solution with positive x and y
# O(sqrt(n)) but solutions can be too large
# when n > 1e6 it gets RecursionError
# when n is a square or n <= 0, it outputs (1, 0) only
def main():
  n = int(input())
  a = []
  if n >= 0:
    get_continued_fraction(a, n)
  cycle = len(a) - 1
  p, q = 1, 0
  if (cycle > 0):
    if (cycle % 2 == 0):
      p, q = gen(a, a[0], a[1], 1, cycle - 2)
    else:
      a += a[1:] + a[1:]
      p, q = gen(a, a[0], a[1], 1, 2 * cycle - 1)

  # printing first k solutions with increasing (x, y)
  k = 5
  x, y = p, q
  print(x, y)
  lx, ly = x, y
  for _ in range(k - 1):
    cx = lx * x + n * ly * y
    cy = lx * y + ly * x
    print(cx, cy)
    lx, ly = cx, cy

if __name__ == '__main__':
  main()

# https://brilliant.org/wiki/quadratic-diophantine-equations-pells-equation/