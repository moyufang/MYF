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

#分类任务

train_sz, test_sz, epoches = 64, 128, 20
learn_rate = 0.1
momentum = 0.8
weight_decay = 0.1
keep_rate = 0.3

class LeNet(nn.Module):
	def __init__(self, n):
		super(LeNet, self).__init__()
		self.conv1 = nn.Conv2d(1, 6, 3, stride = 1, padding = 1)
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
		#print(x.size()[1:])
		x = self.dp2(F.tanh(self.ap2(self.conv2(x))))
		#print(x.size()[1:])
		x = self.dp3(F.tanh(self.fc3(x.view(x.size(0), -1))))
		x = self.dp4(F.tanh(self.fc4(x)))
		x = self.fc5(x)
		return x

th.manual_seed(0)

n = 36
pf = tf.Compose([tf.Resize(n), tf.ToTensor(), tf.Normalize([0.5], [0.5])])
train_datas = mnist.MNIST('./data', train = True, transform=pf, download = True)
test_datas = mnist.MNIST('./data', train = False, transform=pf)

train_loader = DataLoader(train_datas, batch_size = train_sz, shuffle = True)
test_loader = DataLoader(test_datas, batch_size = test_sz, shuffle = True)

if False:
	nnw = LeNet(n = n)
else:
	nnw = th.load('LeNet.pth')

Loss = nn.CrossEntropyLoss()
SGD = opt.SGD(nnw.parameters(), lr = learn_rate, momentum = momentum)#, weight_decay = weight_decay)
train_losses, test_losses= [], []

for epoch in range(epoches):
	train_loss, train_acc, test_loss, test_acc = 0, 0, 0, 0
	nnw.train()
	#if epoch >= 5 and train_losses[epoch-1]/(train_losses[epoch-2]) >= 0.99:
	#	SGD.param_groups[0]['lr'] *= 0.1
	if epoch % 20 == 0:
		SGD.param_groups[0]['lr'] *= 0.1
	for img, label in train_loader:
		out = nnw(img)
		loss = Loss(out, label)
		#print(img.size(), label.size(), out.size())
		#exit()

		SGD.zero_grad()
		loss.backward()
		SGD.step()

		train_loss += loss.item()
		# 计算分类的准确率
		_, pred = out.max(1)
		num_correct = (pred == label).sum().item()
		train_acc += num_correct / img.shape[0]
	train_loss /= len(train_loader)
	train_losses.append(train_loss)
	train_acc /= len(train_loader)
	
	test_loss, test_acc = 0, 0
	nnw.eval()
	for img, label in test_loader:
		out = nnw(img)
		loss = Loss(out, label)
		test_loss += loss.item()
		_, pred = out.max(1)
		num_correct = (pred == label).sum().item()
		test_acc += num_correct / img.shape[0]
	test_loss /= len(test_loader)
	test_acc /= len(test_loader)
	test_losses.append(test_loss)
	print('epoch: {}, Train Loss: {:.4f}, Train Acc: {:.4f}, Test Loss: {:.4f}, Test Acc: {:.4f}'
		.format(epoch, train_loss, train_acc, test_loss, test_acc))
plt.title('trainloss')
plt.plot(np.arange(len(train_losses)), train_losses)
plt.legend(['Train Loss'], loc='upper right')
plt.show()

th.save(nnw, 'LeNet.pth')