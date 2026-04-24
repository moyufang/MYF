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
	def __init__(self, latent_dim, in_channels, img_sz, rho = 0.2):
		super(GenerateNet, self).__init__()
		self.img_sz = img_sz
		self.model = nn.Sequential(
			nn.ConvTranspose2d(latent_dim, latent_dim*4, 2, 1, 0, bias = False),
			nn.BatchNorm2d(latent_dim*4),
			nn.LeakyReLU(rho, True),

			nn.ConvTranspose2d(latent_dim*4, latent_dim*2, 4, 2, 0, bias = False),
			nn.BatchNorm2d(latent_dim*2),
			nn.LeakyReLU(rho, True),

			nn.ConvTranspose2d(latent_dim*2, latent_dim, 4, 2, 0, bias = False),
			nn.BatchNorm2d(latent_dim),
			nn.LeakyReLU(rho, True),

			nn.ConvTranspose2d(latent_dim, in_channels, 4, 2, 0, bias = False),
			nn.Tanh()
		)
	def forward(self, x):
		return self.model(x.view(x.size(0), -1, 1, 1)).view(x.size(0), -1, self.img_sz, self.img_sz)
class DiscriminateNet(nn.Module):
	def __init__(self, img_sz, in_channels, latent_dim, rho = 0.2):
		super(DiscriminateNet, self).__init__()
		self.model = nn.Sequential(
			nn.Conv2d(in_channels, latent_dim, 4, 2, 0, bias = False),
			nn.BatchNorm2d(latent_dim),
			nn.LeakyReLU(rho, True),

			nn.Conv2d(latent_dim, latent_dim*2, 4, 2, 0, bias = False),
			nn.BatchNorm2d(latent_dim*2),
			nn.LeakyReLU(rho, True),

			nn.Conv2d(latent_dim*2, latent_dim*4, 4, 2, 0, bias = False),
			nn.BatchNorm2d(latent_dim*4),
			nn.LeakyReLU(rho, True),

			nn.Conv2d(latent_dim*4, 1, 2, 1, 0, bias = False),
			nn.Sigmoid()
		)
		
	def forward(self, x):
		return self.model(x).view(-1)


batch_size, epoches = 64, 20
g_learn_rate = 0.05
d_learn_rate = 0.1
leaky_rho = 0.2
img_sz, h_dim, latent_dim = 30, 400, 20

pf = tf.Compose([tf.Resize(img_sz), tf.ToTensor(), tf.Normalize([0.5], [0.5])])
train_datas = mnist.MNIST('./data', train = True, transform=pf, download = True)
test_datas = mnist.MNIST('./data', train = False, transform=pf)

train_loader = DataLoader(train_datas, batch_size = batch_size, shuffle = True)
test_loader = DataLoader(test_datas, batch_size = batch_size, shuffle = True)
img = next(iter(train_loader))[0]
print(len(train_loader), img.size())

if True:
	ckpt_path = 'DCGAN.ckpt'
	G = GenerateNet(20, 1, img_sz, leaky_rho)
	D = DiscriminateNet(img_sz, 1, 20, leaky_rho)
else:
	ckpt_path = 'DCGAN.ckpt'
	ckpt = th.load(ckpt_path)
	G = ckpt['G']
	D = ckpt['D']

g_optimizer = opt.Adam(G.parameters(), lr = g_learn_rate)
d_optimizer = opt.Adam(D.parameters(), lr = d_learn_rate)
Loss = nn.BCELoss()
train_losses = [[], []]

for epoch in range(epoches):
	train_loss = [0.0, 0.0]
	for img, label in train_loader:
		real_label = th.ones(img.size(0))
		fack_label = th.zeros(img.size(0))
		#img = img.view(img.size(0), )

		##train for D
		D.train()
		out = D(img)
		real_loss = Loss(out, real_label)

		z = th.randn(img.size(0), latent_dim)
		fack_img = G(z)
		out = D(fack_img)
		fack_loss = Loss(out, fack_label)

		loss = real_loss + fack_loss
		train_loss[0] += loss.item()

		D.zero_grad()
		G.zero_grad()
		loss.backward()
		d_optimizer.step()

		##train for G
		G.train()
		z = th.randn(img.size(0), latent_dim)
		fack_img = G(z)
		out = D(fack_img).view(-1)
		fack_loss = Loss(out, real_label)
		train_loss[1] += fack_loss.item()

		D.zero_grad()
		G.zero_grad()
		fack_loss.backward()
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
	save_image(out.view(img_cnt, 1, img_sz, img_sz), reconsPath)
	
	Image = mpimg.imread(reconsPath)
	plt.imshow(Image) # 显示图像
	#plt.imshow(mpimg.imread(reoutPath))
	plt.axis('off') # 不显示坐标轴
	plt.show()
