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

class GenerateNet(nn.Module):
	def __init__(self, latent_dim, h_dim, out_dim, rho = 0.2):
		super(GenerateNet, self).__init__()
		self.fc1 = nn.Sequential(
			nn.Linear(latent_dim, h_dim, bias=False),
			#nn.BatchNorm1d(h_dim),
			nn.LeakyReLU(rho, True))
		self.fc2 = nn.Sequential(
			nn.Linear(h_dim, h_dim, bias=False),
			#nn.BatchNorm1d(h_dim),
			nn.LeakyReLU(rho, True))
		self.fc3 = nn.Sequential(
			nn.Linear(h_dim, out_dim),
			nn.Tanh())
	def forward(self, x):
		return self.fc3(self.fc2(self.fc1(x)))

class DiscernNet(nn.Module):
	def __init__(self, in_dim, h_dim, rho = 0.2):
		super(DiscernNet, self).__init__()
		self.fc1 = nn.Sequential(
			nn.Linear(in_dim, h_dim,bias=False),
			nn.BatchNorm1d(h_dim),
			nn.LeakyReLU(rho, True))
		self.fc2 = nn.Sequential(
			nn.Linear(h_dim, h_dim,bias=False),
			nn.BatchNorm1d(h_dim),
			nn.LeakyReLU(rho, True))
		self.fc3 = nn.Sequential(
			nn.Linear(h_dim, 1))
	def forward(self, x):
		return self.fc3(self.fc2(self.fc1(x)))

th.manual_seed(0)

img_sz, h_dim, latent_dim = 28, 400, 20
alpha = 0.99
leaky_rho = 0.2

batch_size, epoches = 64, 40
g_learn_rate = 0.0005
d_learn_rate = 0.0005
clip_value = 0.05
n_critic = 1

pf = tf.Compose([tf.ToTensor(), tf.Normalize([0.0], [0.1])])
train_datas = mnist.MNIST('./data', train = True, transform=pf, download = True)
test_datas = mnist.MNIST('./data', train = False, transform=pf)

train_loader = DataLoader(train_datas, batch_size = batch_size, shuffle = True)
test_loader = DataLoader(test_datas, batch_size = batch_size, shuffle = True)
img = next(iter(train_loader))[0]
print(len(train_loader), img.size())

if False:
	ckpt_path = 'GAN.ckpt'
	G = GenerateNet(latent_dim, h_dim, img_sz*img_sz, leaky_rho)
	D = DiscernNet(img_sz*img_sz, h_dim, leaky_rho)
else:
	ckpt_path = 'GAN.ckpt'
	ckpt = th.load(ckpt_path)
	G = ckpt['G']
	D = ckpt['D']

g_optimizer = opt.RMSprop(G.parameters(), lr = g_learn_rate, alpha = alpha)
d_optimizer = opt.RMSprop(D.parameters(), lr = d_learn_rate, alpha = alpha)
train_losses = [[], []]

for epoch in range(epoches):
	train_loss = [0.0, 0.0]
	for i, (img, label) in enumerate(train_loader):
		real_label = th.ones(img.size(0))
		fack_label = th.zeros(img.size(0))
		img = img.view(img.size(0), -1)

		##train for D
		D.train()
		z = th.randn(img.size(0), latent_dim)
		fack_img = G(z)
		fack_out = D(fack_img)
		out = D(img).view(-1)
		loss = -th.mean(out) + th.mean(fack_out)
		
		train_loss[0] += loss.item()

		D.zero_grad()
		G.zero_grad()
		loss.backward()
		d_optimizer.step()

		with th.no_grad():
			for p in D.parameters():
				p.data.clamp_(-clip_value, clip_value)

		##train for G
		if i % n_critic == 0:
			G.train()
			z = th.randn(img.size(0), latent_dim)
			fack_img = G(z)
			fack_out = D(fack_img).view(-1)
			loss = -th.mean(fack_out)
			train_loss[1] += loss.item()

			D.zero_grad()
			G.zero_grad()
			loss.backward()
			g_optimizer.step()
	for fg in [0, 1]:
		train_losses[fg].append(train_loss[fg])
	print("epoch{}: D_Loss:{:.4f} G_Loss:{:.4f}".format(epoch, train_loss[0], train_loss[1]))

if len(train_losses[0]) > 0:
	plt.title('trainloss')
	for fg in [0, 1]:
		plt.plot(np.arange(len(train_losses[fg])), train_losses[fg])
	plt.legend(['Train Loss'], loc='upper right')
	plt.show()

th.save({'G':G,'D':D}, ckpt_path)

from PIL import Image

def save_image(x, to_path):
	
	vec = (x.numpy()*255).astype(np.uint8)
	num_img = x.shape[0]
	H = int(x.shape[0]**(0.5)+0.0001)
	W = (x.shape[0]-1) // H+1
	h, w = x.shape[2], x.shape[3]
	#x = x.reshape(x.size(0), x.shape[2], x.shape[3])
	image = Image.new('RGB', (W*w, H*h), color = 'white')
	print(H, W, h, w)
	print(vec[0].shape)

	for c in range(num_img):
		i, j = c//H, c%W
		sub_image = Image.fromarray(vec[c].reshape(h, w).astype(np.uint8))
		image.paste(sub_image, (j*w, i*h, j*w+w, i*h+h))

	image.save(to_path)

with th.no_grad():
	img_cnt = 64
	z = th.randn(img_cnt, latent_dim)
	out = G(z)
	reconsPath = 'GAN-1.png'
	print(th.mean(out).item(), th.std(out).item(), th.min(out).item(), th.max(out).item())
	save_image(out.view(img_cnt, 1, img_sz, img_sz), reconsPath)
	
	Image = mpimg.imread(reconsPath)
	plt.imshow(Image) # 显示图像
	#plt.imshow(mpimg.imread(reoutPath))
	plt.axis('off') # 不显示坐标轴
	plt.show()
