#!/usr/bin/env python3

import numpy as np

die = 0.1
limit = 10
num = np.random.randint(int(1e3),int(1e5))
print(num)
p = [0.1 for x in range(limit)]
res = []
for _ in range(num):
	for i in range(len(p)):
		p[i] = max(0.1,p[i]-0.1)
	now = np.random.choice(limit,1,p)
	res.append(now[0])
	p[now[0]] = min(0.6,p[now[0]]+0.2)
for ele in res:
    print(ele)