from fractions import Fraction
x = 3232.45622121
# gets the closest rational number to x where denominator is limited
p = Fraction(x).limit_denominator(2323)
print(p)