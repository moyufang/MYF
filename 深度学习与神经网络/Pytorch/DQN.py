import torch as th
import torch.autograd as ag
from matplotlib import pyplot as plt
from torchvision.datasets import mnist
import torchvision.transforms as tf
from torch.utils.data import DataLoader
import torch.nn.functional as F
import torch.optim as opt
from torch import nn
import numpy as np
import matplotlib.image as mpimg
import os
import time

th.manual_seed(0)

def randint(low, high):
	x = th.randint(low, high, (1,))[0].item()
	return x

new_model_flag = True
map_sz, action_sz, trap_ratio, chest_ratio = 8, 4, 0.1, 0.1
map_type = [0, 1, 2, 3, 4] #空地、陷阱、宝箱、玩家、走过的空地
reward_eval = [0, -1, 1, 0, 0]
reward_train = [1, -1, 5, 0, -0.5]
robot_tag = 10
d = [[0, 1], [0, -1], [-1, 0], [1,0]]
bth_sz, T, memory_sz, C, co_map_sz, train_count, epoches= 512, map_sz, 4096, 1, 16, 4, 50
keep_rate, gama, epsilon, learn_rate = 0.2, 0.5, 0.1, 0.01

device = th.device("cuda:0" if th.cuda.is_available() else "cpu")  
print(device)

def ck(i, j, h = map_sz, w = map_sz):
	return i >= 0 and i < h and j >= 0 and j < w

def find_robot(s):
	for i in range(0, map_sz):
		for j in range(0, map_sz):
			if (s[i][j] == map_type[3]):
				return i,j
	assert False, "Find no robot!\n"

def get_ns_r(s, a, i, j, train = False):
	ni, nj, ns = i+d[a][0], j+d[a][1], s.clone()
	ns.requires_grad = False
	assert ck(ni, nj), "Out of map!\n"
	ns[i][j] = map_type[4]
	r = ns[ni][nj].item()
	r = reward_train[r] if train else reward_eval[r]
	ns[ni][nj] = map_type[3]
	return ns, r

def init_s(h = map_sz, w = map_sz):
	s = th.zeros(h, w).to(th.int64)
	s.requires_grad = False
	cnt = 0
	empty = []
	for i in range(h):
		for j in range(w):
			x = th.rand(1)[0].item()
			if x <= trap_ratio:
				s[i][j] = map_type[1]
			elif x <= trap_ratio+chest_ratio:
				s[i][j] = map_type[2]
			else:
				s[i][j] = map_type[0]
				empty.append([i, j])
	if len(empty) == 0:
		si, sj = randint(0, h), randint(0, w)
	else:
		si, sj = empty[randint(0, len(empty))]
	s[si][sj] = map_type[3]
	return s

def tf_s(s):
	map_sz = s.size(0)
	i, j = find_robot(s)
	tmp_s = th.zeros(2, map_sz, map_sz)
	for i in range(map_sz):
		for j in range(map_sz):
			tmp_s[1][i][j] = reward_train[s[i][j].item()]
	tmp_s[1][i][j] = reward_train[4]
	tmp_s[0][i][j] = robot_tag
	return tmp_s

class MemoryTable:
	def __init__(self, N):
		self.s, self.a, self.r, self.ns = None, None, None, None
		self.n, self.cur, self.N = 0, 0, N
		assert self.N > 0, "N <= 0!"
	def add(self, s, a, r, ns):
		s, ns = tf_s(s), tf_s(ns)
		s = s.to(device)
		a = a.to(device)
		r = r.to(device)
		ns = ns.to(device)
		if self.n == 0:
			self.s, self.a, self.r, self.ns =\
				s.view(1, 2, map_sz, map_sz), a.view(1), r.view(1), ns.view(1, 2, map_sz, map_sz)
			self.n += 1
		elif self.n < self.N:
			self.s = th.cat((self.s, s.view(1, 2, map_sz, map_sz)), 0)
			self.a = th.cat((self.a, a.view(1)), 0)
			self.ns = th.cat((self.ns, ns.view(1, 2, map_sz, map_sz)), 0)
			self.r = th.cat((self.r, r.view(1)), 0)
			self.n += 1
		else:
			cur = self.cur
			self.s[cur], self.a[cur], self.r[cur], self.ns[cur] = s, a, r, ns
			self.cur = (cur+1)%self.N
		self.s = self.s.to(device)
		self.a = self.a.to(device)
		self.r = self.r.to(device)
		self.ns = self.ns.to(device)

	def get_bth(self, bth_sz):
		assert bth_sz > 0 and self.n > 0, "QTable is empty!"
		indices = th.randperm(self.n)[:min(bth_sz, self.n)]
		return\
			self.s[indices].to(device),\
			self.a[indices].to(device),\
			self.r[indices].to(device),\
			self.ns[indices].to(device)

class ResBlock(nn.Module):

    def __init__(self, in_dim, h_dim, stride=1, expansion=1):
        super(ResBlock, self).__init__()
        self.expansion=1
        self.conv = nn.Sequential(
            nn.Conv2d(in_dim, h_dim, 3, stride, 1, bias=False),
            nn.BatchNorm2d(h_dim),
            nn.ReLU(True),

            nn.Conv2d(h_dim, h_dim, 3, 1, 1, bias=False),
            nn.BatchNorm2d(h_dim),
            nn.ReLU(True),
        )
        self.shortcut = nn.Sequential()
        if stride != 1 or in_dim != h_dim:
            self.shortcut = nn.Sequential(
                nn.Conv2d(in_dim, h_dim, 1, stride=stride, bias=False),
                nn.BatchNorm2d(h_dim)
            )
    def forward(self, x):
        return self.conv(x)+self.shortcut(x)

class DQN(nn.Module):
    def __init__(self, map_sz, h_dim = 32, action_sz = 4):
        super(DQN, self).__init__()
        self.map_sz = map_sz
        self.conv = nn.Sequential(
            nn.Conv2d(2, h_dim, 3, 1, 1, bias = False),
            nn.BatchNorm2d(h_dim),
            nn.ReLU(True),

            ResBlock(h_dim, h_dim, 1),
            ResBlock(h_dim, h_dim, 1),

            # ResBlock(h_dim, h_dim*2, 1),
            # ResBlock(h_dim*2, h_dim*2, 1),
            
            #nn.AvgPool2d(2, 2),
            ResBlock(h_dim, h_dim*4, 1),
            ResBlock(h_dim*4, h_dim*4, 1),
            
            # nn.AvgPool2d(2, 2),
            # ResBlock(h_dim*4, h_dim*8, 1),
            # ResBlock(h_dim*8, h_dim*8, 1),
        )
        self.pool = nn.AdaptiveAvgPool2d((1, 1))
        self.fc = nn.Sequential(
            # nn.Dropout(keep_rate),
            # nn.Linear(h_dim*4, h_dim*8),
            # nn.ReLU(True),
            # nn.Dropout(keep_rate),
            # nn.Linear(h_dim*8, h_dim*4),
            # nn.ReLU(True),
            # nn.Dropout(keep_rate),
            nn.Linear(h_dim*4, action_sz),
            #nn.Sigmoid()
        )

    def forward(self, x):
        x = x.to(th.float32).view(-1, 2, self.map_sz, self.map_sz)
        x = self.conv(x)
       	x = self.pool(x)
        x = x.view(x.size(0), -1)
        x = self.fc(x)
        return x

def show(s, road):
	# 可视化地图  
	map_sz = s.shape[0]
	s[road[0][0]][road[0][1]] = 0
	for i in range(map_sz):
		for j in range(map_sz):
			if (s[i][j] == map_type[4]) or (s[i][j] == map_type[0]):
				s[i][j] = 0
			elif s[i][j] == 1:
				s[i][j] = -1
			elif s[i][j] == 2:
				s[i][j] = 1
	plt.figure(figsize=(5, 5))  
	plt.imshow(s, cmap='coolwarm', vmin=-1, vmax=1) 
	

	for i in range(len(road)-1):
		#print(road[i])
		plt.arrow(
			road[i][1], road[i][0],
			0.9*(road[i+1][1]-road[i][1]), 0.9*(road[i+1][0]-road[i][0]),
    		head_width=0.1, head_length=0.1, fc='r', ec='r')
	#plt.colorbar()  # 显示颜色条  
	plt.grid(False)  # 关闭网格线  
	plt.axis('off')  # 关闭坐标轴 
	plt.show()

def test_map():
	mtb = MemoryTable(5)
	step, y = 10, 0
	ori_s = s = init_s()
	i, j = find_robot(s)
	assert s[i][j] == map_type[3]
	road = [[i, j]]
	print(s)
	print("({},{},{})".format(i, j, y))
	for _ in range(step):
		a = randint(0, 4)
		while not ck(i+d[a][0], j+d[a][1]):
			a = randint(0, 4)
		ns, r = get_ns_r(s, a, i, j)
		i, j = i+d[a][0], j+d[a][1]
		road.append([i, j])
		y += r

		mtb.add(s, th.tensor(a), th.tensor(r), ns)

		s = ns
		# print(s)
		# print("({},{},{})".format(i, j, y))
	bs, ba, br, bns = mtb.get_bth(5)
	show(ori_s.numpy(), np.array(road))
	# print(bs)
	# print(ba)
	# print(br)
	# print(bns)

#test_map()
#exit()

ckpt_path = 'DQN.ckpt'
ckpt_path = os.path.join('./kaggle/working/', ckpt_path)
if new_model_flag:
	policy_net = DQN(map_sz, action_sz = action_sz)
	target_net = DQN(map_sz, action_sz = action_sz)
else:
	model_dict = th.load(ckpt_path)
	policy_net = model_dict['policy_net']
	target_net = model_dict['target_net']
policy_net.to(device), target_net.to(device)

for p in target_net.parameters():
	p.requires_grad = False

mtb = MemoryTable(memory_sz)

optimizer = opt.Adam(policy_net.parameters(), lr = learn_rate)

def get_a(s, i, j):
	target_net.eval()
	a, max_q = -1, 0
	a_list = target_net(tf_s(s).view(1, 2, map_sz, map_sz)).squeeze(0)
	for fg in range(action_sz):
		ni, nj = i+d[fg][0], j+d[fg][1]
		if ck(ni, nj) and (a == -1 or a_list[fg] > max_q):
			a, max_q = fg, a_list[fg]
	assert a >= 0 and a < action_sz
	return a

def eval(s, ifshow=False):
	steps, y = map_sz*map_sz//2, 0
	i, j = find_robot(ori_s)
	neg = s.eq(map_type[1]).sum().item()
	pos = s.eq(map_type[2]).sum().item()
	road = [[i, j]]
	for _ in range(steps):
		a = get_a(s, i, j)
		assert a >= 0 and a < action_sz
		ns, r = get_ns_r(s, a, i, j)
		#assert r == -1 or r == 0 or r == 1
		s = ns
		i, j = i+d[a][0], j+d[a][1]
		road.append([i, j])
		assert ck(i, j)
		y += r
	if ifshow:
		show(ori_s.numpy(), np.array(road))
	return neg, pos, y

ori_s = s = init_s()
for epoch in range(epoches):
	epoch_time_start = time.time()
	s_list = [init_s() for _ in range(co_map_sz)]
	pos_list = [find_robot(s_list[ids]) for ids in range(co_map_sz)]
	iter_time_start = time.time()
	optim_time = 0
	for t in range(T):
		for ids, s in enumerate(s_list):
			i, j = pos_list[ids]

			if th.rand(1)[0].item() > epsilon:
				a = get_a(s, i, j)
			else:
				a = randint(0, action_sz)
				while not ck(i+d[a][0], j+d[a][1]):
					a = randint(0, action_sz)

			ns, r = get_ns_r(s, a, i, j, True)
			mtb.add(s, th.tensor(a), th.tensor(r), ns)
			s_list[ids], pos_list[ids] = s, (i+d[a][0], j+d[a][1])

			##optimize DQN
			policy_net.train()
			target_net.eval()
			for _ in range(train_count):
				bs, ba, br, bns = mtb.get_bth(bth_sz)
				one_optim_time_start = time.time() 
				out = policy_net(bs)
				cur_values = out.gather(1, ba.unsqueeze(1)).to(device).view(-1)
				# if t == 0 and ids %4 == 0:
				# 	print("epoch{}".format(epoch), out[0])
				expect_values = target_net(bns).max(1)[0].view(-1).to(device).detach()
				y = (expect_values*gama+br).to(device).detach()
				loss = F.smooth_l1_loss(cur_values, y).to(device)

				optimizer.zero_grad()
				loss.backward()
				optimizer.step()
				#print(loss.item())
			one_optim_time = time.time() - one_optim_time_start
			optim_time += one_optim_time
	iter_time = (time.time()-iter_time_start)/(T*len(s_list)*train_count)
	optim_time /= (T*len(s_list))

	if epoch % C == 0:
		target_net.load_state_dict(policy_net.state_dict())

	neg, pos, y = eval(ori_s, False)
	print("epoch{} neg:{} pos:{} y:{} iter_time:{:.4f} optim_time:{:.4f}".format(
		epoch, neg, pos, y, iter_time, optim_time))
	if (epoch+1)%5 == 0:
		th.save({
			'policy_net':policy_net,
			'target_net':target_net
			}, ckpt_path)
target_net.load_state_dict(policy_net.state_dict())
eval(ori_s, True)

th.save({
	'policy_net':policy_net,
	'target_net':target_net
	}, ckpt_path)

