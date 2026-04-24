
# 基本函数

`torch.tensor` 是一个用于创建 `Tensor` 的工厂函数。
```python
torch.tensor(data, *, dtype=None, device=None, requires_grad=False, pin_memory=False)

# Tensor 转 numpy 数组的方法
torch.Tensor.numpy(True) #False 的话会共享内存(有些情况下False也不共享内存)
# numpy 转 TensorFlow 的方法
torch.from_numpy(numpy_ndarray)
```

pytorch 修改 Tensor 中，首参数是 `input` 函数基本都有 `tensor` 方法的版本，如下
```python
output = input.scatter(dim, index, src, reduce=None) # 形式1：方法形式（目标张量调用）
output = torch.scatter(input, dim, index, src, reduce=None) # 形式2：函数形式
```

![[Tensor属性.png]]
![[Tensor比较.png]]
![[Tensor创建.png]]![[Tensor矩乘.png]]

```python
# 爱因斯坦求和约定实现更高维张量点积
D = torch.randn(2, 3, 4, 5)
E = torch.randn(2, 3, 5, 6)
F = torch.einsum('bnmk,bnkl->bnml', D, E)  # 结果形状：(2, 3, 4, 6)
```

![[Tensor索引.png]]![[Tensor统计.png]]![[Tensor修改.png]]
`unsqueeze` 与 `squeeze` 均会返回一个共享内存的变量，但原变量的 size 不变，与此同时 `squeeze` 仅在 `dim` 指定的维度大小为 $1$ 时才生效
```python
#排序轴，通常用于调整 opencv 中读入的图片数组
x = torch.randn(400, 500, 3)
y = torch.permute(x, (2, 0, 1))
#移动轴，与上条语句有同样效果
y = torch.movedim(x, -1, 0) 
#numpy 中的对标函数
nx = np.zeros((400, 500, 3))
ny = np.transpose(nx, (2, 0, 1))
```

![[Tensor运算.png]]


[PyTorch|Tensor基础操作：拼接、切分、索引和变换](https://blog.csdn.net/windhawk_fly/article/details/128645774)
`cat` 不新增维度，`stack` 在指定 `dim` 上新增一个维度

# torch.cuda


**设备管理**
- `torch.cuda.is_available()`：检查系统是否有可用的 CUDA GPU
- `torch.cuda.device_count()`：返回可用的 GPU 数量 
- `torch.cuda.current_device()`：返回当前选择的 GPU 索引
**张量操作**
- 将张量移动到 GPU：`tensor.cuda()` 或 `tensor.to('cuda')`
- 将张量移回 CPU：`tensor.cpu()` 或 `tensor.to('cpu')`
**内存管理**
- `torch.cuda.empty_cache()`：清空未使用的缓存内存
- `torch.cuda.memory_allocated()`：返回当前 GPU 已分配的内存
- `torch.cuda.max_memory_allocated()`：返回本次运行中 GPU 分配的最大内存
##### 设备对象

```python
import torch

# 创建一个设备对象
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

# 对于张量
x = torch.randn(3, 3)
x = x.to(device)  # 移动到GPU（如果可用）

# 对于模型
model = torch.nn.Linear(10, 5)
model = model.to(device)  # 将整个模型移动到GPU

# 指定特定GPU（在多GPU系统中）
device = torch.device("cuda:1")  # 使用第二个GPU

# 同时指定设备和数据类型
x = x.to(device=device, dtype=torch.float64)

# 设置默认的GPU设备
torch.cuda.set_device(1)  # 使用第二个GPU
```

# 梯度求导

新创建的 `Tensor` 默认不跟踪梯度，指定 `requires_grad` 来跟踪梯度
```python
x = torch.tensor([1.0, 2.0], requires_grad=True)
```

停止梯度跟踪的方法

```python
# 方法1: 使用detach()
# 此方法同样使用于各种在 nn.Module 里产生的运算
y_detached = y.detach()

# 方法2: 使用torch.no_grad()上下文
with torch.no_grad():
    y = x * 2
```