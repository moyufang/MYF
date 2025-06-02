[原文](https://www.luogu.com.cn/blog/zhouziheng666/qian-tan-st-biao) 



# 浅谈ST表

166 likes, on 2019-01-31 10:01:43, in [日报](https://www.luogu.com.cn/blog/zhouziheng666/#type=日报)

先来一个小问题：

```
有N个数，M次询问，每次给定区间[L,R]，求区间内的最大值。

N<=10,M<=10
```

老师，我会 $O(N)$ 暴力枚举！

再来：

```
N<=10^5,M<=10^5
```

老师，我会线段树 $O(\log N)$​​处理每个询问！

再来：[P3865 【模板】ST表](https://www.luogu.org/problemnew/show/P3865)

```
N<=10^5,M<=10^6
```

这时我们发现，随着M*M*的增大，$O(\log N)$ 的询问处理已经不够优秀，我们需要 $O(1)$ 处理询问的方法。这就引出了我们今天的主题——$\text{ST}$ 表。

### 前置技能

倍增算法（例题：[P3379 【模板】最近公共祖先（LCA）](https://www.luogu.org/problemnew/show/P3379)）

区间动规（例题：[P3146 [USACO16OPEN\]248](https://www.luogu.org/problemnew/show/P3146)）

### 算法流程

我们现在要 $O(1)$ 求出区间最大值，一个很自然的想法便是记录 $f(i,j)$ 为 $[i,j]$内的最大值，显然有转移方程 $f(i,j)=\max(f(i,j-1),a_j)$

但是这样预处理是 $O(N^2)$ 的，不能通过，我们考虑进一步优化。

观察到一个性质：$\max$ 操作允许区间重叠，也就是$\max(a,b,c)=\max(\max(a,b),\max(b,c))$（这个性质非常重要，决定了ST*S**T*表是否能用来维护这种操作，例如ST*S**T*表一般不能维护区间和，因为 $a+b+c\neq a+b+b+c$，也就是我们可以由两个较小的、有重叠的区间直接推出一个大区间，因此我们可以少维护一些区间

计算机中有很多事物是跟 $2$ 有关的。这里也是这样，我们采用倍增思想，令 $f(i,j)$ 为从 $a_i$ 开始的、连续 $2^j$ 个数的最大值，显然：

$f(i,0)=a_i$（显然根据定义可得）

$f(i,j)=\max(f(i,j-1),f(i+2^{j-1},j-1)$ 

这一条非常重要，我们画个图理解一下：

![img](https://cdn.luogu.com.cn/upload/pic/50965.png)

现在我们考虑 $f(1,2)$ ，也就是 $[1,4]$ 的最大值

![img](https://cdn.luogu.com.cn/upload/pic/50966.png)

我们把$[1,4]$分为了$[1,2]$和 $[3,4]$ 两个小区间，这两个区间是我们之前求过的 $f(1,1)$ 与 $f(3,1)$，而 $f(1,1)=8,f(3,1)=7$，则 $f(1,2)=\max(f(1,1),f(3,1))=8$​ 。

我们发现，在这种方式下，以每个点为起点都有 $O(\log N)$ 个区间，每个区间可以 $O(1)$ 求出，则预处理总时间、空间复杂度都为 $O(N\log N)$ 。

那怎么处理询问呢？

根据 $\max$ 的性质，我们可以把区间拆成两个相重叠的区间。看图：

![img](https://cdn.luogu.com.cn/upload/pic/50967.png)

记询问区间长度为len*l**e**n*，我们从左端点向右找一段长为 $2^{\log(len)}$ 的区间（蓝色部分），右端点向左也找一段长为 $2^{\log(len)}$ 的区间（黄色部分），显然这两段区间已经覆盖了整个区间（中间重叠了一块绿色部分），取最大值即可

当然为了保证询问复杂度为 $O(1)$，我们需要提前预处理出每个$\log(len)$ 向下取整后的值。整个算法总时间复杂度为 $O(N\log N+M)$。

顺便附上代码：

```cpp
#include<cstdio>
#include<algorithm>

using namespace std;

int a[100001]={};
int lg[100001]={-1};
int maxn[100001][50]={};

int main()
{
    int n=0,m=0;
    scanf("%d%d",&n,&m);
    for(int i=1;i<=n;i++)
    {
        scanf("%d",&a[i]);
        lg[i]=lg[i/2]+1;
    }

    for(int i=1;i<=n;i++)
    {
        maxn[i][0]=a[i];
    }

    for(int i=1;i<=lg[n];i++)
    {
        for(int j=1;j+(1<<i)-1<=n;j++)
        {
            maxn[j][i]=max(maxn[j][i-1],maxn[j+(1<<(i-1))][i-1]);
        }
    }

    int l=0,r=0;
    while(m--)
    {
        scanf("%d%d",&l,&r);
        int len=lg[r-l+1];
        printf("%d\n",max(maxn[l][len],maxn[r-(1<<(len))+1][len]));
    }
    return 0;
}
```

### 应用

先来一道模板题：[P2880 [USACO07JAN\]平衡的阵容Balanced Lineup](https://www.luogu.org/problemnew/show/P2880)

```
给定N个数和M个询问，求每次询问区间内极差=最大值-最小值。
```

用 $\text{ST}$ 表求出区间最大值、最小值即可，最小值同理。（最小值也满足那个性质）

```cpp
#include<cstdio>
#include<algorithm>

using namespace std;

int a[100001]={};
int lg[100001]={-1};
int maxn[100001][50]={};
int minn[100001][50]={};

int main()
{
    int n=0,m=0;
    scanf("%d%d",&n,&m);
    for(int i=1;i<=n;i++)
    {
        scanf("%d",&a[i]);
        lg[i]=lg[i/2]+1;
        maxn[i][0]=a[i];
        minn[i][0]=a[i];
    }
    for(int i=1;i<=lg[n];i++)
    {
        for(int j=1;j+(1<<i)-1<=n;j++)
        {
            maxn[j][i]=max(maxn[j][i-1],maxn[j+(1<<(i-1))][i-1]);
            minn[j][i]=min(minn[j][i-1],minn[j+(1<<(i-1))][i-1]);
        }
    }
    int l=0,r=0;
    while(m--)
    {
        scanf("%d%d",&l,&r);
        int len=lg[r-l+1];
        printf("%d\n",max(maxn[l][len],maxn[r-(1<<len)+1][len])-
        min(minn[l][len],minn[r-(1<<len)+1][len]));
    }
    return 0;
}
```

当然，$\text{ST}$​ 表还能维护很多东西，只要满足那条性质的**静态**问题都能维护，但 $\text{ST}$​ 表较难修改。

于是就有了这道新鲜出炉的省选题：$JSOIWC2019Day4T1$

```
给定N个整数和M个询问，每次询问给定一个X，求有多少个区间[L,R]使得A[L]~A[R]的GCD为X。
```

算法1：

老师，我会暴力枚举每一个区间求 $GCD$！

复杂度：$O(MN^3\log A)$

期望得分：$0$

算法2：（本蒟蒻的做法）

老师，我会把所有区间GCD*G**C**D*预处理出来，扔进```map```里！

复杂度：$O(N^2\log N+N\log A+M\log N)$

期望得分：$50$

算法3：

显然 $GCD$​ 是满足上述性质的，因此可以用 $\text{ST}$表求出每个区间最小值。

~~然后每个询问 $O(N^2)$ 枚举~~

接着我们发现，以每个数为起点的区间 $GCD$ 最多$ O(\log N)$ 个（每次变化至少变小一半）

我们可以二分出第一次变化的点，记录出现次数，插入```map```即可

复杂度：$O(N\log^2N+N\log N\log A+M\log N)$

期望得分：$100$

### 进阶

thanks to @[xhhkwy](https://www.luogu.org/space/show?uid=96592)

假设现在有个毒瘤出题人故意卡你……

```
有N个数，M次询问，每次给定区间[L,R]，求区间内的最大值。

N<=2*10^7,M<=2*10^7,随机数据，时限5s
```

然后发现……预处理都 $T$ 飞了

怎么办？我们要想方设法降低 $\text{ST}$ 的构造时间

分块都学过吧（没学过？出门左转[P1903 [国家集训队\]数颜色 / 维护队列](https://www.luogu.org/problemnew/lists?name=&orderitem=pid&tag=289&content=0&type=)）

让我们瞻仰一下@[xhhkwy](https://www.luogu.org/space/show?uid=96592)的仙气

```
将序列分成长度是logN的块，预处理出每一块的前缀min与后缀min，
然后在把每一个块的最小值拉出来跑st，
预处理时间复杂度为N + (N/logN)*log(N/logN) = O(N)，
询问的话如果两个端点在一个块中那么暴力，时间复杂度O(logN)。
否则直接查st表+前后缀min
```

（引自@[xhhkwy](https://www.luogu.org/space/show?uid=96592)私信）

各位自行理解吧，注意只有数据随机的情况才能使大部分查询操作复杂度为$O(1)$，如果题目没有写明“随机数据”则不要轻易使用

### 后记

本蒟蒻的 $\text{ST}$ 表讲解到这里就结束了。希望大家已经掌握了这个算法。

~~点个赞呗~~