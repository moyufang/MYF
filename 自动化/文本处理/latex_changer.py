"""
This py is used substitute 'B' with 'A' in the given 'utf-8' text with linear time complexity,
	designed via the mixed use of AC automation and finite automation.
It has features below:
(1) 'A' and 'B' is allowed to be the combination of several special character and word
(2) In this program, a word is defined as consecutive characters composed only by letters and digits.
	For example, '234abc' and 'abc234' are both regard as single word.
(3) Substitution only works in the environment of latex formula, 
		thus only working in the middle of '$' or '$$'.
	You can understand when it works better via same cases in function 'test(selected)'.
"""

import collections

class AC:
	def __init__(self):
		AC.clear(self)
	def clear(self):
		self.char_set = {}
		self.tag = [['','',0]]
		self.char_n = 0
		self.root, self.cur = 0, 0
		self.node = []#nxt, fail, tag
		self.builded = False
	def ins(self, s, t):
		assert s != ''
		self.tag.append([s, t, 0])
		for c in s:
			if c not in self.char_set:
				self.char_set[c] = self.char_n; self.char_n += 1
	def new_node(self, fail = 0, tag = 0):
		self.node.append([[0]*self.char_n, fail, tag])
		return len(self.node)-1
	def build(self):
		# build Trie
		self.new_node(0, 0)
		self.root = self.new_node(1, 0)
		for i in range(len(self.tag)):
			if i == 0: continue
			s = self.tag[i][0]
			u = self.root
			for c in s:
				assert c in self.char_set
				cid = self.char_set[c]
				if self.node[u][0][cid] == 0:
					self.node[u][0][cid] = self.new_node()
				u = self.node[u][0][cid]
			self.node[u][2] = i
			self.tag[i][2] = u
		# build AC automation
		que = collections.deque()
		self.node[self.root][1] = self.root
		for i in range(self.char_n):
			if self.node[self.root][0][i] != 0:
				que.append(self.node[self.root][0][i])
			else:
				self.node[self.root][0][i] = self.root
		while len(que) > 0:
			u = que.popleft()
			for i in range(self.char_n):
				v = self.node[u][0][i]
				if v != 0:
					que.append(v);
					self.node[v][1] = self.node[self.node[u][1]][0][i]
				else:
					self.node[u][0][i] = self.node[self.node[u][1]][0][i]
		self.builded = True
	def reset(self):
		self.cur = self.root
	def next(self, c):
		assert self.builded
		if c not in self.char_set: self.cur = self.root; return None
		self.cur = self.node[self.cur][0][self.char_set[c]]
		tagid = self.node[self.cur][2]
		return tuple(self.tag[tagid][0:2]) if tagid != 0 else None

class Replacer(AC):
	def __init__(self):
		super().__init__()
		self.cur2 = 0; self.state = None
		self.stk = collections.deque(); self.stk2 = collections.deque();
	def clear(self):
		self.reset()
		AC.clear(self)
	def reset(self):
		AC.reset(self)
		self.cur2 = 0; self.stk.clear(); self.stk2.clear(); self.state = None
	def match(self):
		t = None
		for c in self.stk2:
			self.stk.append(c)
			t = self.next(c)
		if t != None and len(t[0]) >= len(self.stk2) and\
			(len(self.stk) == len(t[1]) or \
				not self.stk[-len(t[0])-1].isalnum() or \
				not t[0][0].isalnum()):
			for i in range(len(t[0])): self.stk.pop()
			for c in t[1]: self.stk.append(c)
			AC.reset(self)
		self.stk2.clear()
	def handle(self, s):
		assert self.builded
		self.reset()
		for c in s:
			if self.cur2 == 0:
				self.stk.append(c)
				if c == '$': self.cur2 = 1; self.state = False; AC.reset(self)
			elif c != '$' and self.cur2 in [1, 2, 3, 5]:
				if self.cur2 == 5:
					assert len(self.stk2) == 0
					self.stk2.append('$'); self.match()
				if c.isalnum(): self.cur2 = 3; self.stk2.append(c)
				elif c == '\\': self.cur2 = 4; self.match(); self.stk2.append(c)
				else: self.cur2 = 3; self.match(); self.stk2.append(c); self.match()
			elif self.cur2 in [1, 2, 3]:
				if self.cur2 == 1: self.cur2 = 2; self.state = True; self.stk.append('$')
				elif self.cur2 == 2: self.cur2 = 5;
				else:
					self.match()
					if self.state: self.cur2 = 5
					else: self.cur2 = 0; self.state = None; self.stk.append('$')
			elif self.cur2 == 5:
				assert len(self.stk2) == 0
				self.cur2 = 0; self.state = None; self.stk.append('$$')
			elif self.cur2 == 4:
				if c.isalnum(): self.cur2 = 3; self.stk2.append(c)
				else: self.cur2 = 3; self.stk2.append(c); self.match()
			else: assert False
			#print("%s %d"%(c, self.cur2), self.state)
		return ''.join(list(self.stk))

def test(selected):
	rpr = Replacer()
	if selected == 1:
		rpr.ins('abc', 'cba')
		rpr.ins('($abc', '')
		# '2abc' is regards a single word differing from 'abc'.
		s = '$$abc($abc)(2abc)$$'
		# '$$cba)(2abc)$$'
	elif selected == 2:
		rpr.ins('apple banana', 'cat')
		s = r'apple banana$apple banana$'
		# apple banana$cat$
	elif selected == 3:
		rpr.ins(' ', '.')
		rpr.ins(r'$', '.')
		rpr.ins(r'\\', '.')
		# Notice that '\ ' is regarded as single char instead of the combinaion of '\' and ' '
		# You can insert the pair ('\ ', '') to erase '\ '
		s = r'$${df$\over dx}\ \ (\\)($)$$'
		# '$${df.\over.dx}\ \ (.)(.)$$'
	elif selected == 4:
		rpr.ins('$', '.')
		rpr.ins('abcdefg', '.')
		# Only the first pair of '$' is regarded as valid area.
		s = r'$ abcdefg $ abcdefg $'
		# '$ . $ abcdefg $'
	elif selected == 5:
		rpr.ins('abc abc', '.')
		s = r'$abc abc(pabc abc$'
		# '$.(pabc abc$'
	else: assert False
	# After all 'ins' are done, build the AC automation before calling 'handle()'.
	# Once calling 'build()', do not call 'ins()' any more.
	# Unless you call 'clear()' to reset the 'Replacer'
	rpr.build()
	t = rpr.handle(s)
	print(s, '\n', t, sep='')

test(1)

rpr = Replacer()
rpr.ins(r'\part', r'\partial')
rpr.build()

# with open(r"D:\ljh\MYF\深度学习与神经网络\神经网络训练方法.md", "r", encoding='utf-8') as file:
# 	s = file.read()
# with open(r"D:\ljh\MYF\深度学习与神经网络\test.md", "w", encoding='utf-8') as file:
# 	t = rpr.handle(s)
# 	file.write(t)


