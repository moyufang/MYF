import torch as th
import torch
from matplotlib import pyplot as plt
import numpy as np


l = th.tensor([[1, 2], [3, 4], [5, 6], [7, 8], [9, 10]])

a = th.randperm(5)[:3]
b = l[a]
b[0][1] = 1231
print(b, l)
