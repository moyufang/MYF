import torch as th

class SELayer(nn.Module):
	def __init__(self, channel, reduction=16):
		super(SELayer, self).__init__()
		self.avg_pool = nn.AdaptiveAvgPool2d(1)  
		self.fc = nn.Sequential(
			nn.Linear(channel, channel // reduction, bias=False),  
			nn.ReLU(inplace=True),
			nn.Linear(channel // reduction, channel, bias=False),
			nn.Sigmoid()
		)
	def forward(self, x):
		b, c, _, _ = x.size()
		y = self.avg_pool(x).view(b, c)
		y = self.fc(y).view(b, c, 1, 1)
		return x * y.expand_as(x)

class ResBlock(nn.Module):
	expansion = 1
	def __init__(self, in_channels, out_channels, stride=1, downsample=None):
		super(ResBlock, self).__init__()
		self.conv1 = nn.Conv2d(in_channels, out_channels, 3, stride, 1, bias=False)
		self.bn1 = nn.BatchNorm2d(out_channels)
		self.relu = nn.ReLU()
		self.conv2 = nn.Conv2d(out_channels, out_channels, 3, 1, 1, bias=False)
		self.bn2 = nn.BatchNorm2d(out_channels)
		self.downsample = downsample
	def forward(self, x):
		identity = x
		if self.downsample is not None: identity = self.downsample(x)
		out = self.relu(self.bn1(self.conv1(x)))
		out = self.bn2(self.conv2(out))
		return self.relu(out+identity)

# ResNet-50/101/152 残差结构 Bottleneck
class ResBottle(nn.Module):
	expansion = 4
	def __init__(self, in_channels, out_channels, stride=1, downsample=None):
		super(ResBottle, self).__init__()
		self.conv1 = nn.Conv2d(in_channels, out_channels, 1, 1, bias=False)
		self.bn1 = nn.BatchNorm2d(out_channels)
		self.conv2 = nn.Conv2d(out_channels, out_channels, 3, stride, 1, bias=False)
		self.bn2 = nn.BatchNorm2d(out_channels)
		self.conv3 = nn.Conv2d(out_channels, out_channels*self.expansion, 1, stride, 1, bias=False)
		self.bn3 = nn.BatchNorm2d(out_channels*self.expansion)
		self.relu = nn.ReLU(True)
		self.downsample = downsample
	def forward(self, x):
	identity = x
	if self.downsample is not None: identity = self.downsample(x)
	out = self.relu(self.bn1(self.conv1(x)))
	out = self.relu(self.bn2(self.conv2(out)))
	out = self.bn3(self.conv3(out))
	return self.relu(out + identity)

class DenseBlock(nn.Module):
	def __init__(self, in_channels, growth_rate=32, bn_size=4, k=5):
		self.layers = []
		for i in range(k):
			self.layers.append(nn.Sequential(
				nn.Conv2d(in_channels+growth_rate, growth_rate*bn_size, 3, 1, 1, bias=False),
				nn.BatchNorm2d(in_channels),
				nn.ReLU(True),
				nn.Conv2d(growth_rate*bn_size, growth_size, 3, 1, 1, bias=False),
				nn.BatchNorm2d(in_channels),
				nn.ReLU(True),
			))
	def forward(self, x):
		features = [x]
		for layer in self.layers:
			y = layer(th.cat(features, dim=1))
			features.append(y)
		return features[-1]


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
