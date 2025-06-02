
#### 二分查找

```python
import bisect
sorted_list = []

bisect.bisect_left(sorted_list, key) # 返回最小的大于等于 key 的索引位置
bisect.insort_left(sorted_list, key) # 将 key 插入到最小的大于等于 key 的索引位置之前
bisect.bisect_right(sorted_list, key) # 返回最小的大于 key 的索引位置
bisect.insort_right(sorted_list, key) # 将 key 插入到最小的大于 key 的索引位置之前
```

#### 双端队列
```python
from collections import deque

q = deque()
q.append(1)
q.appendleft(1)
q.pop(1)
q.popleft(1)

is_empty = lambda q:len(q)==0

lst = list(q)

```

#### 并查集

```python
class UnionFind:
	def __init__(self, n):
		self.n = n
		self.clear()
	def clear(self):
		self.uf = [-1]*self.n
	def find(self, u):
		top, stk = 0, [0]*64
		while self.uf[u] > 0:
			stk[top] = u; top += 1; u = self.uf[u]
		while top > 0:
			top -= 1; self.uf[stk[top]] = u
		return u
	def unite(self, u, v,):
		u = self.find(u); v = self.find(v)
		if u == v: return False
		if self.uf[u] > self.uf[v]: u,v = v,u
		self.uf[u] += self.uf[v]; self.uf[v] = u;
		return True
```