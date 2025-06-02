# import torch as th
# import torch.autograd as ag
# from matplotlib import pyplot as plt


# #回归任务
# def test1():
# 	N = 100
# 	x = th.linspace(-10, 10, N)
# 	y = (3*x**2-4*x+2) + th.randn(x.size())*10

# 	w = th.randn(2, 1, requires_grad = True)
# 	b = th.ones(N, requires_grad = True)

# 	steps = 5000
# 	# 由于数据的特性使得 backwards 后参数 w 和 b 的梯度特别大，需要调小学习率，否则越训练 Loss 越大
# 	alpha = 0.00001 
# 	for s in range(steps):
# 		z = w[0]*(x**2)+w[1]*x+b
# 		L = ((y-z)**2).mean()
# 		L.backward()
# 		with th.no_grad():
# 			w -= alpha*w.grad
# 			b -= alpha*b.grad
# 			w.grad.zero_()
# 			b.grad.zero_()
# 		print(L)

# 	plt.scatter(x.numpy(), y.numpy())
# 	plt.plot(x.numpy(), z.detach().numpy())
# 	plt.show()

import torch as th
import torch.autograd as ag
from matplotlib import pyplot as plt
from torchvision.datasets import mnist
import torchvision.transforms as tf
from torch.utils.data import DataLoader
import torch.nn.functional as func
import torch.optim as opt
from torch import nn
import numpy as np

#分类任务

class Net(nn.Module):
	def __init__(self, n):
		super(Net, self).__init__()
		L = self.L = len(n)-1
		nnw = self.nnw = []
		for i in range(1, L):
			nnw.append(nn.Sequential(
				nn.Linear(n[i-1], n[i]),
				nn.ReLU(),
				nn.Dropout(keep_rate)
				))
			self.add_module("l{}".format(i), nnw[i-1])
		nnw.append(nn.Sequential(nn.Linear(n[L-1], n[L])))
		self.add_module("l{}".format(L), nnw[L-1])
		self.n = n
	def forward(self, x):
		L, n, nnw = self.L, self.n, self.nnw
		for i in range(0, L):
			x = nnw[i](x)
		return x

def test2():
	th.manual_seed(0)
	train_sz, test_sz, epoches = 64, 128, 20
	learn_rate = 0.05
	momentum = 0.5
	weight_decay = 0.1
	keep_rate = 0.6

	pf = tf.Compose([tf.ToTensor(), tf.Normalize([0.5], [0.5])])
	train_datas = mnist.MNIST('./data', train = True, transform=pf, download = True)
	test_datas = mnist.MNIST('./data', train = False, transform=pf)

	train_loader = DataLoader(train_datas, batch_size = train_sz, shuffle = True)
	test_loader = DataLoader(test_datas, batch_size = test_sz, shuffle = True)

	if False:
		nnw = Net([28*28, 300, 200, 200, 10])
	else:
		nnw = th.load('nnw768_300_200_200_10.pth')



	Loss = nn.CrossEntropyLoss()
	SGD = opt.SGD(nnw.parameters(), lr = learn_rate, momentum = momentum)#, weight_decay = weight_decay)
	train_losses, test_losses= [], []

	for epoch in range(epoches):
		train_loss, train_acc, test_loss, test_acc = 0, 0, 0, 0
		nnw.train()
		#if epoch >= 5 and train_losses[epoch-1]/(train_losses[epoch-2]) >= 0.99:
		#	SGD.param_groups[0]['lr'] *= 0.1
		if epoch % 10 == 0:
			SGD.param_groups[0]['lr'] *= 0.1
		for img, label in train_loader:
			img = img.view(img.size(0), -1)
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
			img = img.view(img.size(0), -1)
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

	th.save(nnw, 'nnw768_300_200_200_10.pth')



test2()