#!/usr/bin/env python3

from time import time
from random import seed
from random import randint

seed(time())

# n: number of tests to generate
# l: length of b in bits (size of "key")
# a: random int of length l (value to inverse)
# b: random int of length l (modulo)

total = 0
text = ""

def extended_gcd(aa, bb):
    lastremainder, remainder = abs(aa), abs(bb)
    x, lastx, y, lasty = 0, 1, 1, 0
    while remainder:
        lastremainder, (quotient, remainder) = remainder, divmod(lastremainder, remainder)
        x, lastx = lastx - quotient*x, x
        y, lasty = lasty - quotient*y, y
    return lastremainder, lastx * (-1 if aa < 0 else 1), lasty * (-1 if bb < 0 else 1)

def modinv(a, m):
    g, x, y = extended_gcd(a, m)
    if g != 1:
        return 0, g
    return x % m, g

def genTests(n, l):
    global total
    global text
    total += n
    while n > 0:
        g = 0
        while g != 1:
            a = randint(0, 2 ** l)
            b = randint(0, 2 ** l)
            res, g = modinv(a, b)
        text += "\t{ \"" + str(a) + "\", \"" + str(b) + "\", \"" + str(res) +  "\", \"" + str(l) + "\" },\n"
        n -= 1

genTests(25, 32)
genTests(25, 64)
genTests(25, 128)
genTests(25, 256)
genTests(25, 512)
genTests(25, 1024)
genTests(25, 2048)
genTests(25, 4096)
genTests(25, 8192)
    
print("#include <stddef.h>")
print()
print("const char *modinv_tests[" + str(total + 1) + "][4] = {")
print(text, end='')
print("\t{ NULL, NULL, NULL, NULL }")
print("};")  
