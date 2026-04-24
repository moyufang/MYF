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

th.manual_seed(0)
bth_sz = 64

pf = tf.Compose([
	tf.ToTensor(),
	tf.Normalize([0.5, 0.5, 0.5], [0.5, 0.5, 0.5])
	])

train_data = CIFAR10(root='.data', train=True, transform=pf, download=True)
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

def _make_divisible(ch, divisor=8, min_ch=None):
    if min_ch is None:
        min_ch = divisor #最小通道数,若为None,则为8,所以 最小为8
    new_ch = max(min_ch, int(ch + divisor / 2) // divisor * divisor) #趋近于8的整数倍,四舍五入
    # Make sure that round down does not go down by more than 10%.
    if new_ch < 0.9 * ch:
        new_ch += divisor
    return new_ch

class ConvBNReLU6(nn.Sequential):
	def __init__(self, in_channels, out_channels, kernel_size=3, stride=1, groups=1):
		super(ConvBNReLU6, self).__init__(
			nn.Conv2d(in_channels, out_channels, kernel_size, 
				stride, (kernel_size-1)//2, groups=groups, bias=False),
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

class MobileNetV2(nn.Module):
	def __init__(self, num_classes=1000, alpha=1.0, round_nearest=8, keep_rate=0.2):
		super(MobileNetV2, self).__init__()
		in_channels = _make_divisible(32 * alpha, round_nearest)#调整到8的整数倍
		last_channels = _make_divisible(1280 * alpha, round_nearest)
		inverted_residual_setting = [
			#t扩展因子,c输出特征矩阵深度channel,n是bottleneck的重复次数,s是步距
			 [1, 16, 1, 1],
            [6, 24, 2, 2],
            [6, 32, 3, 2],
            [6, 64, 4, 2],
            [6, 96, 3, 1],
            [6, 160, 3, 2],
            [6, 320, 1, 1]
		]
		features = []
		features.append(ConvBNReLU6(3, in_channels, stride=1))
		for t, c, n, s in inverted_residual_setting:
			out_channels = _make_divisible(c*alpha, round_nearest)
			for i in range(n):
				stride = s if i == 0 else 1
				features.append(InvertedResidual(in_channels, out_channels, stride, t))
				in_channels = out_channels
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
momentum = 0.8
keep_rate = 0.2

if True:
	nnw = MyNet(num_classes = 10, keep_rate=keep_rate)
else:
	nnw = th.load('MobileNetV2.pth')

img, label = next(iter(train_loader))
print(label.min(), label.max())
out = nnw(img)
print(img.size(), label.size(), out.size(), len(train_loader))

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
	start_time = time.time()
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
	end_time = time.time()
	print("OneBatchTrainTime:{}".format(end_time-start_time))
	train_loss /= len(train_loader)
	train_losses.append(train_loss)
	train_acc /= len(train_loader)
	
	test_loss, test_acc = 0, 0
	nnw.eval()
	start_time = time.time()
	for img, label in test_loader:
		out = nnw(img)
		loss = Loss(out, label)
		test_loss += loss.item()
		_, pred = out.max(1)
		num_correct = (pred == label).sum().item()
		test_acc += num_correct / img.shape[0]
	end_time = time.time()
	print("OneBatchTestTime:{}".format(end_time-start_time))
	test_loss /= len(test_loader)
	test_acc /= len(test_loader)
	test_losses.append(test_loss)
	print('epoch: {}, Train Loss: {:.4f}, Train Acc: {:.4f}, Test Loss: {:.4f}, Test Acc: {:.4f}'
		.format(epoch, train_loss, train_acc, test_loss, test_acc))
plt.title('trainloss')
plt.plot(np.arange(len(train_losses)), train_losses)
plt.legend(['Train Loss'], loc='upper right')
plt.show()

th.save(nnw, 'MobileNetV2.pth')