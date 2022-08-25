# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    modmulTest.py                                      :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/04 18:07:25 by yforeau           #+#    #+#              #
#    Updated: 2021/08/04 18:07:26 by yforeau          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/usr/bin/env python3

from time import time
from random import seed
from random import randint

seed(time())

# n: number of tests to generate
# l: length of c in bits (size of "key")
# a: random int of length l*2 ("text")
# b: random int of length l*2 (exponent)
# c: random int of length l (modulo)

total = 0
text = ""

def genTests(n, l):
    global total
    global text
    l2 = l + (l // 3)
    total += n
    while n > 0:
        a = randint(0, 2 ** l2)
        b = randint(0, 2 ** l2)
        c = randint(0, 2 ** l)
        res = ((a % c) * (b % c)) % c
        text += "\t{ \"" + str(a) + "\", \"" + str(b) + "\", \"" + str(c) + "\", \"" + str(res) + "\", \"" + str(l) +  "\" },\n"
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
print("const char *modmul_tests[" + str(total + 1) + "][5] = {")
print(text, end='')
print("\t{ NULL, NULL, NULL, NULL, NULL }")
print("};")  
