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
import os

th.manual_seed(0)
bth_sz = 64

pf = tf.Compose([
	tf.ToTensor(),
	tf.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
	])

train_data = CIFAR10(root='.data', train=True, transform=pf, download=False)
test_data = CIFAR10(root='.data', train=False, transform=pf)

train_loader = DataLoader(train_data, batch_size=bth_sz, shuffle=True)
test_loader = DataLoader(test_data, batch_size=bth_sz, shuffle=True)

def inspect_data():
	print("len(train_data):", len(train_data))
	print("len(test_data):", len(test_data))

	k = 3
	fig, axs = plt.subplots(1, k, figsize=(10, 3))
	ids = th.randperm(len(train_data))[:k]
	i = 0
	for ax, _ in zip(axs, ['a', 'b', 'c']):
		ax.imshow(train_data[ids[i]][0].numpy().transpose((1,2,0)))
		#ax.set_title("{}".format(test_data[ids[i]][0].item()))
		i += 1
	plt.show()
#inspect_data()

class ConvBNReLU6(nn.Sequential):
	def __init__(self, in_channels, out_channels, kernel_size=3, stride=1, groups=1):
		super(ConvBNReLU6, self).__init__(
			nn.Conv2d(in_channels, out_channels, kernel_size, 
				stride=stride, padding=(kernel_size-1)//2, groups=groups, bias=False),
			nn.BatchNorm2d(out_channels),
			nn.ReLU6(inplace=True)
		)


class InvertedResidual(nn.Module):
	def __init__(self, in_channels, out_channels, stride, expend_ratio):
		super(InvertedResidual, self).__init__()
		hidden_channel = in_channels*expend_ratio
		self.use_shortcut = stride == 1 and in_channels == out_channels

		layers = []
		if expend_ratio != 1:
			layers.append(ConvBNReLU6(in_channels, hidden_channel, kernel_size=1))
		layers.extend([
			ConvBNReLU6(hidden_channel, hidden_channel, stride=stride, groups=hidden_channel),
			nn.Conv2d(hidden_channel, out_channels, kernel_size=1, bias=False),
			nn.BatchNorm2d(out_channels)
		])
		self.ir = nn.Sequential(*layers)
	def forward(self, x):
		return x + self.ir(x) if self.use_shortcut else self.ir(x)

class ConvNet(nn.Module):
	def __init__(self, n, keep_rate = 0.5):
		super(ConvNet, self).__init__()
		self.conv1 = nn.Conv2d(3, 6, 3, stride = 1, padding = 1)
		self.ap1 = nn.AvgPool2d(2, stride = 2)
		self.dp1 = nn.Dropout(keep_rate)
		self.conv2 = nn.Conv2d(6, 32, 3, stride = 1, padding = 1)
		self.ap2 = nn.AvgPool2d(2, stride = 2)
		self.dp2 = nn.Dropout(keep_rate)
		self.fc3 = nn.Linear(32*(n//4)*(n//4), 192)
		self.dp3 = nn.Dropout(keep_rate)
		self.fc4 = nn.Linear(192, 96)
		self.dp4 = nn.Dropout(keep_rate)
		self.fc5 = nn.Linear(96, 10)
	def forward(self, x):
		x = self.dp1(F.tanh(self.ap1(self.conv1(x))))
		x = self.dp2(F.tanh(self.ap2(self.conv2(x))))
		x = self.dp3(F.tanh(self.fc3(x.view(x.size(0), -1))))
		x = self.dp4(F.tanh(self.fc4(x)))
		x = self.fc5(x)
		return x

class MyNet(nn.Module):
	def __init__(self, num_classes=30, alpha=1.0, keep_rate=0.2):
		super(MyNet, self).__init__()
		in_channels = 32
		hidden_channel = 48
		last_channels = 128
		features = []
		features.append(ConvBNReLU6(3, in_channels, stride=1))
		features.append(InvertedResidual(in_channels, in_channels, 1, 1))
		features.append(InvertedResidual(in_channels, in_channels, 1, 1))
		features.append(InvertedResidual(in_channels, in_channels, 1, 1))
		features.append(ConvBNReLU6(in_channels, last_channels, 1))

		self.features = nn.Sequential(*features)
		self.avgpool = nn.AdaptiveAvgPool2d((1, 1))
		self.classifier = nn.Sequential(
            nn.Dropout(keep_rate),
            nn.Linear(last_channels, num_classes)
        )
		for m in self.modules():
			if isinstance(m, nn.Conv2d):
				nn.init.kaiming_normal_(m.weight, mode='fan_out')
				if m.bias != None:
					nn.init.zeros_(m.bias)
			elif isinstance(m, nn.BatchNorm2d):
				nn.init.ones_(m.weight)
				nn.init.zeros_(m.bias)
			elif isinstance(m, nn.Linear):
				nn.init.normal_(m.weight, 0, (2/(m.weight.size(0)+m.weight.size(1)))**0.5)
				nn.init.zeros_(m.bias)
	def forward(self, x):
		x = self.features(x)
		x = self.avgpool(x)
		x = x.view(x.size(0), -1)
		x = self.classifier(x)
		return x


train_sz, test_sz, epoches = 64, 128, 20
learn_rate = 0.1
momentum = 0.5
keep_rate = 0.2

mlps = [MyNet(num_classes = 10, keep_rate = keep_rate), ConvNet(32, keep_rate)]
eval_weights = [1.0/len(mlps) for i in range(len(mlps))]
SGD = opt.SGD([{'params':mlps[i].parameters()} for i in range(len(mlps))],
	lr=learn_rate, momentum=momentum)
Loss = nn.CrossEntropyLoss()
for epoch in range(epoches):
	N = len(mlps)
	train_loss, train_acc = [0.0 for i in range(N)], [0.0 for i in range(N)]
	for img, label in train_loader:
		SGD.zero_grad()
		for i, mlp in enumerate(mlps):
			mlp.train()
			out = mlp(img)
			Loss(out, label).backward()
		SGD.step()
	
	sum_acc = 0.0
	for i, mlp in enumerate(mlps):
		cnt = 0
		for img, label in train_loader:
			mlp.eval()
			out = mlp(img)
			train_loss[i] += Loss(out, label).item()
			_, pred = out.max(1)
			num_correct = (pred == label).sum().item()
			train_acc[i] += num_correct
			cnt += label.size(0)
		train_loss[i] /= len(train_loader)
		train_acc[i] /= cnt
		sum_acc += train_acc[i]

	for i in range(N):
		eval_weights[i] = train_acc[i]/sum_acc

	cnt = 0
	test_loss, test_acc = 0.0, 0.0
	for img, label in test_loader:
		tmp_loss = 0.0
		tmp_out = th.zeros(label.size(0), 10)
		for i, mlp in enumerate(mlps):
			mlp.eval()
			out = mlp(img)
			loss = Loss(out, label)
			tmp_loss += loss.item()*eval_weights[i]
			assert tmp_out.size() == out.size()
			tmp_out += out*eval_weights[i]
		_, pred = tmp_out.max(1)
		num_correct = (pred == label).sum().item()
		test_loss += tmp_loss
		test_acc += num_correct
		cnt += label.size(0)
	test_loss /= len(test_loader)
	test_acc /= cnt

	print("epoch{}:".format(epoch))
	print("\tTrian Loss and Train Acc:")
	for i in range(N):
		print("\t\tmodel{}:{:.4f} {:.4f}".format(i, train_loss[i], train_acc[i]))
	print("\tTest Loss:{:.4f} Test Acc:{:.4f}".format(test_loss, test_acc))