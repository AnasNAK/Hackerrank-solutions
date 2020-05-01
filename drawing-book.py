#!/bin/python3

import sys


n = int(input().strip())
p = int(input().strip())
# your code goes here
print(min(p//2,n//2-(p//2)))
