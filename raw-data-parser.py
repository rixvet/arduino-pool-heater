#!/usr/bin/env python

import sys
import re

time = 920804400
speed = 'U'
for file in sys.argv[1:]:
  for line in open(file,'r').xreadlines():
    time += 1
    if line.startswith('Temperature'):
      if len(line) == 61:
        n = line.split()
        print ':'.join(map(str,[time,n[2], n[4], n[6], n[8], speed]))
    elif line.startswith('Speed'):
      try:
        speed = line.split()[1] 
        if int(speed) > 100:
          speed = 'U'
      except ValueError:
        speed = 'U'

#Speed: 0
#Temperatures T0: 20.2705 T1: 21.8217 T2: 21.0234 T3: 14.6659

