#!/usr/bin/env python3

from time import time
from random import seed
from random import randint

seed(time())

# n: number of tests to generate
# l: length of c in bits (size of "key")
# a: random int inferior to c ("text")
# b: random int inferior to c (exponent)
# c: random int of length l (modulo)

total = 0
text = ""

def genTests(n, l):
    global total
    global text
    total += n
    while n > 0:
        c = randint(0, 2 ** l)
        a = randint(0, c)
        b = randint(0, c)
        res = pow(a, b, c)
        text += "\t{ \"" + str(a) + "\", \"" + str(b) + "\", \"" + str(c) + "\", \"" + str(res) + "\", \"" + str(l) + "\" },\n"
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
print("const char *modexp_tests[" + str(total + 1) + "][5] = {")
print(text, end='')
print("\t{ NULL, NULL, NULL, NULL, NULL }")
print("};")  
