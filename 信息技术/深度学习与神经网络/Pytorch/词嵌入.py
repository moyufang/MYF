import numpy as np
import torch as th
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as opt
import torchvision
import torchvision.transforms as tf
from torchvision.datasets import CIFAR10
from torch.utils.data import DataLoader
import matplotlib.pyplot as plt
import time
import jieba

keep_rate = 0.2
epoches = 400

class Net(nn.Module):
	def __init__(self, words_sz, num_classes, embedding_dim, hidden_dim, num_directions=2):
		super(Net, self).__init__()
		self.embed = nn.Embedding(words_sz, embedding_dim)
		self.lstm = nn.LSTM(embedding_dim, hidden_dim, bidirectional = True if num_directions == 2 else False)
		self.s = (
			th.zeros(num_directions, 1, hidden_dim),
			th.zeros(num_directions, 1, hidden_dim)
		)
		self.classifier = nn.Sequential(
			#nn.Dropout(keep_rate),
			nn.Linear(num_directions*hidden_dim, num_classes))
	def init_s(self):
		self.s = (th.zeros_like(self.s[0]), th.zeros_like(self.s[1]))
		#如果把以上语句换成以下语句则会出错，因为每次 backward 前都要用新的输入Tensor
		# self.s[0].zeros_()
		# sefl.s[1].zeros_()

	def forward(self, sentence):
		x = self.embed(sentence).view(sentence.size(0), 1, -1)
		x, self.s = self.lstm(x, self.s)
		x = x.view(x.size(0), -1)
		x = self.classifier(x)
		return x

def prepare_sequence(seq, to_ix):
	#print(seq, to_ix)
	idxs = [to_ix[w] for w  in seq]
	return th.LongTensor(idxs)

train_data = [
	('The cat ate the fish'.split(), ['DET', 'NN', 'V', 'DET', 'NN']),
	('They read that book'.split(), ['NN', 'V', 'DET', 'NN'])
]
test_data = [
	('They ate the fish'.split(), ['NN', 'V', 'DET', 'NN'])
]
words_to_ix = {}
tag_to_ix = {'DET':0, 'NN':1, 'V':2}
for seq, _ in train_data:
	for w in seq:
		if w not in words_to_ix:
			words_to_ix[w] = len(words_to_ix)
print(words_to_ix)

train_loader = [(
	prepare_sequence(train_data[i][0], words_to_ix), prepare_sequence(train_data[i][1], tag_to_ix)
	) for i in range(len(train_data))]
test_loader = [(
	prepare_sequence(test_data[i][0], words_to_ix), prepare_sequence(test_data[i][1], tag_to_ix)
	) for i in range(len(test_data))]

net = Net(len(words_to_ix), len(tag_to_ix), 8, 16)
Loss = nn.CrossEntropyLoss()
SGD = opt.SGD(net.parameters(), lr = 0.1)

for epoch in range(epoches):
	net.train()
	for seq, label in train_loader:
		net.init_s()
		out = net(seq)
		#print(out)
		#print(label)
		loss = Loss(out, label)


		SGD.zero_grad()
		loss.backward()
		SGD.step()

	net.eval()
	cnt = 0
	test_acc = 0
	for seq, label in test_loader:
		out = net(seq)
		_, pred = out.max(1)
		num_correct = (pred == label).sum().item()
		test_acc += num_correct
		cnt += label.size(0)
	if epoch % int(epoches // 10) == 0:
		print(test_acc / len(test_loader))











