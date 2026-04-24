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

th.manual_seed(0)
class VAE(nn.Module):
	def __init__(self, img_sz = 28, ic = 1, hc = 400, zc = 20, z_dim = 20):
		super(VAE, self).__init__()
		self.img_sz = img_sz
		self.conv1 = nn.Sequential(
			nn.Conv2d(ic, hc, 3, stride = 1, padding = 1),
			nn.BatchNorm2d(hc),
			nn.ReLU())
		self.conv2 = nn.Sequential(
			nn.Conv2d(hc, zc, 3, stride = 1, padding = 1),
			nn.BatchNorm2d(zc),
			nn.ReLU())
		self.fc3_mu = nn.Sequential(
			nn.Linear(img_sz*img_sz*zc, z_dim),
			nn.ReLU())
		self.fc3_var = nn.Sequential(
			nn.Linear(img_sz*img_sz*zc, z_dim),
			nn.ReLU())

		self.fc4 = nn.Sequential(
			nn.Linear(z_dim, img_sz*img_sz*zc),
			nn.ReLU())
		self.iconv5 = nn.Sequential(
			nn.ConvTranspose2d(zc, hc, 3, stride = 1, padding = 1),
			nn.BatchNorm2d(hc),
			nn.ReLU())
		self.iconv6 = nn.Sequential(
			nn.ConvTranspose2d(hc, ic, 3, stride = 1, padding = 1),
			nn.BatchNorm2d(ic),
			nn.Sigmoid())
		self.mu, self.log_var = None, None
	def encode(self, x):
		x = self.conv2(self.conv1(x))
		x = x.view(x.size(0), -1)
		self.mu = self.fc3_mu(x)
		self.log_var = self.fc3_var(x)
		return self.mu + th.randn_like(self.log_var)*th.exp(self.log_var/2)
	def decode(self, z):
		z = self.fc4(z).view(z.size(0), -1, self.img_sz, self.img_sz)
		z = self.iconv6(self.iconv5(z))
		return z
	def forward(self, x):
		return self.decode(self.encode(x))
	def get_loss(self, y, x):
		#print(x)
		#print(y)
		reconst_loss = F.binary_cross_entropy(y, x, size_average=False)
		kl_div = -0.5 * th.sum(1+self.log_var-self.mu.pow(2)-self.log_var.exp())
		return reconst_loss + kl_div

class FCVAE(nn.Module):
	def __init__(self, img_sz = 28, h_dim = 64, z_dim = 128):
		super(FCVAE, self).__init__()
		self.img_sz = img_sz
		self.fc1 = nn.Sequential(
			nn.Linear(img_sz*img_sz, h_dim),
			#nn.BatchNorm1d(h_dim),
			nn.ReLU())
		self.fc2_mu = nn.Sequential(
			nn.Linear(h_dim, z_dim),
			#nn.BatchNorm1d(z_dim),
			#nn.ReLU()
			)
		self.fc2_var = nn.Sequential(
			nn.Linear(h_dim, z_dim),
			#nn.BatchNorm1d(z_dim),
			#nn.ReLU()
			)

		self.fc3 = nn.Sequential(
			nn.Linear(z_dim, h_dim),
			#nn.BatchNorm1d(h_dim),
			nn.ReLU())
		self.fc4 = nn.Sequential(
			nn.Linear(h_dim, img_sz*img_sz),
			#nn.BatchNorm1d(img_sz*img_sz),
			nn.Sigmoid())
		self.mu, self.log_var = None, None
	def encode(self, x):
		x = self.fc1(x.view(x.size(0), -1))
		self.mu, self.log_var = self.fc2_mu(x), self.fc2_var(x)/4
		return self.mu + th.randn_like(self.log_var)*th.exp(self.log_var/2)
	def decode(self, z):
		z = self.fc4(self.fc3(z))
		return z.view(z.size(0), 1, self.img_sz, self.img_sz)
	def forward(self, x):
		return self.decode(self.encode(x))
	def get_loss(self, y, x):
		#print(x)
		#print(y)
		reconst_loss = F.binary_cross_entropy(y, x, size_average=False)
		kl_div = -0.5 * th.sum(1+self.log_var-self.mu.pow(2)-self.log_var.exp())
		return reconst_loss + kl_div


batch_size, epoches = 64, 50
learn_rate = 0.001
momentum = 0.8
weight_decay = 0.1
keep_rate = 0.3

pf = tf.Compose([tf.ToTensor(), tf.Normalize([0.0], [1.0])])
train_datas = mnist.MNIST('./data', train = True, transform=pf, download = True)
test_datas = mnist.MNIST('./data', train = False, transform=pf)

train_loader = DataLoader(train_datas, batch_size = batch_size, shuffle = True)
test_loader = DataLoader(test_datas, batch_size = batch_size, shuffle = True)
img = next(iter(train_loader))[0]
print(len(train_loader), img.size())

h_dim, z_dim = 400, 20

if False:
	model_path = 'FCVAE.pth'
	if False:
		vae = FCVAE(28, h_dim, z_dim)
	else:
		vae = th.load(model_path)
else:
	model_path = 'VAE.pth'
	if True:
		vae = VAE(28, 1, 64, 128, 20)
	else:
		vae = th.load(model_path)
optimizer = opt.Adam(vae.parameters(), lr = learn_rate)
#optimizer = opt.SGD(vae.parameters(), lr = learn_rate, momentum = momentum)
train_losses, test_losses= [], []

for epoch in range(epoches):
	train_loss, test_loss = 0, 0
	vae.train()
	if epoch % 20 == 0:
		optimizer.param_groups[0]['lr'] *= 0.1
	cnt = 0
	for img, label in train_loader:
		#cnt += 1
		#print(cnt)
		out = vae(img)
		loss = vae.get_loss(out, img)

		optimizer.zero_grad()
		loss.backward()
		optimizer.step()

		train_loss += loss.item()
	train_losses.append(train_loss)
	print('epoch: {}, Train Loss: {:.4f}'
		.format(epoch, train_loss))

if len(train_losses) > 0:
	plt.title('trainloss')
	plt.plot(np.arange(len(train_losses)), train_losses)
	plt.legend(['Train Loss'], loc='upper right')
	plt.show()

th.save(vae, model_path)

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

epoch = 0
with th.no_grad():
	x = next(iter(train_loader))[0]
	# 保存采样图像，即潜在向量Z通过解码器生成的新图像
	z = th.randn(batch_size, z_dim)
	out = vae.decode(z).view(-1, 1, 28, 28)
	save_image(out, 'sampled-{}.png'.format(epoch+1))
	reoutPath = 'sampled-1.png'

	# 保存重构图像，即原图像通过解码器生成的图像
	out = vae(x)
	x_concat = th.cat([x.view(-1, 1, 28, 28), out.view(-1, 1, 28, 28)], dim=3)
	save_image(x_concat, 'reconst-{}.png'.format(epoch+1))

	reconsPath = 'reconst-1.png'
	Image = mpimg.imread(reconsPath)
	plt.imshow(Image) # 显示图像
	#plt.imshow(mpimg.imread(reoutPath))
	plt.axis('off') # 不显示坐标轴
	plt.show()
