import sys
import re


# Read individual lines from a file, optionally preserving blanks
def read_lines(filename, allow_blanks = False):
    return [l.strip() for l in open(filename).readlines() if len(l.strip()) > 0 or allow_blanks]

# Concatenate consecutive lines from a file, using blanks as delimiters
# Assume only single blank lines in the input - true so far
def read_groups(filename, delim = ':', space = ':'):
    lines  = read_lines(filename, True)
    groups = []
    group  = ''
    for line in lines:
        if len(line) > 0:
            group = group + line + ' '
        else:
           if len(group) > 0:
               groups.append(group)
               group = ''

    if len(group) > 0:
        groups.append(group)

    return groups


def read_primes():
    lines = read_lines('../utils/primes.txt')
    primes = []
    for line in lines:
        s = line.split()
        for p in s:
            primes.append(int(p))
    return primes  


def factorise(n, primes):
    result = {}
    for p in primes:
        if n % p == 0:
            result[p] = 0
        while n % p == 0:
            result[p] = result[p] + 1
            n = n / p
    return result           


def totient(n, primes):
    factors = factorise(n, primes)
    result = 1
    for p in factors:
        result = result * (p ** (factors[p] - 1)) * (p - 1)
    return result


def gcd(a, b):
    p = max(a, b)
    q = min(a, b)
    while True:
        r = p % q
        if r == 0: 
            return q
        p = q
        q = r
     

def lcm(a, b):
    return (a * b) / gcd(a, b)

