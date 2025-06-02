本文起笔与 ```2021.11.02```

以下若无特殊说明，对数值 $vl$，$vl[2^i]$ 表示其二进制表示下从低位向高位的第 i 位（从 $0$ 开始数）数字（$0$ 或 $1$）。

若默认将数值按二进制从高位向低位插入， 则 $\text{01-Trie}$ 的本质是一颗值域为 $[0, 2^w-1]$ 的动态开点权值线段树，其中 $w$ 为数值的二进制位数，每次插入相当于给对应位置上的权值加 $1$。

为了方便，这里称 $\text{01-Trie}$ 树上每个节点的高度为它与叶子节点的深度差（所有叶子节点深度都一样），叶子节点的高度均为 $0$。称 $\text{01-Trie}$ 的第 $i$ 层为 $\text{01-Trie}$ 上高度为 $i$ 的所有节点的集合，$\text{01-Trie}$ 有 $[0,w]$ 共 $w+1$ 层 。 

$\text{01-Trie}$ 这颗权值线段树干的事相当于是将所有插入的数值桶排序了一遍，之后我们可以在这颗权值线段树上干很多事，如查询最值和第 $k$ 大的值。

注意到动态开点权值线段树是一颗二叉树，树上任意节点是可以以 $O(1)$ 做到交换左右子树的，相当于可以以 $O(1)$ 做到改变若干数值之间的排名，即强制使该节点左子树下所有数值均大于该节点右子树下所有数值（原来是右子树下所有数值均大于左子树下所有数值）。

现在假设我们有一颗 $\text{01-Trie}$ 和一个数值 $vl$，我们想要查询 $\text{01-Trie}$ 上所有数值与 $vl$ 异或后的第 $k$ 大。

考虑如何实现，想法就是先求出 $vl$ 与 $\text{01-Trie}$ 上所有数值异或后的新数值们组成的新的 $\text{01-Trie}$，然后再在新 $\text{01-Trie}$ 上查询第 $k$ 大。

进一步分析可以发现，异或上 $vl$ 这一操作对原 $\text{01-Trie}$ 向新 $\text{01-Trie}$ 变换的操作实际上可以通过交换某些节点的左右子树来实现。

具体操作就是，对 $vl$ 的每一位 $vl[2^i]$，若 $vl[2^i]$ 为 $1$ 则让 $\text{01-Trie}$ 第 $i+1$ 层的所有节点均交换各自的左右子树一次，若 $vl[2^i]$ 为 $0$ 则 $\text{01-Trie}$ 第 $i+1$ 层节点均维持原状，不交换左右子树，这体现了异或的性质。

这个变换操作实质是重新确定了一个新的字典序，感性理解一下就是在用字典序比较两个数值时比较到第 $i$ 位时，重新确定了是为 $0$ 的更大还是为 $1$ 的更大。

如果我们真的对 $\text{01-Trie}$ 做这一变换操作，每次查询完后还要还原 $\text{01-Trie}$，那么单次查询的复杂度将会爆炸。

其实没必要进行复杂度如此巨大的显式变换操作，我们可以只进行复杂度较小的隐式变换操作，即我们不交换节点的左右子树，我们只是根据 $vl[2^i]$ 来判断 $\text{01-Trie}$ 第 $i$ 层中的某一节点中，是左子树里的数值大还是右子树里的数值大（左右子树中必存在一方，使得其所有数值均大于另一方所有数值），依此来判断在权值线段树上查询第 $k$ 大时递归到某节点时该向左子树继续递归还是向右子树继续递归。

通过隐式变换，在新 $\text{01-Trie}$ 树上查询第 $k$ 大的复杂度仍为 $O(w)$。

代码中 ```/*...*/``` 代表依具体情况而定

```cpp
#define maxh 31
#define maxnn /*...*/

int rt = 1, ncnt = 0; //rt 是 Trie 树的根节点，ncnt 是代表当前 Trie 树上有多少节点
struct Trie{ int lc, rc, sz; } trie[maxnn]; //lc 是左儿子，rc 是右儿子，sz 代表当前子树下有多少个数值
#define lc(x) trie[x].lc
#define rc(x) trie[x].rc
#define sz(x) trie[x].sz

inline void ud(int x){ sz(x) = sz(lc(x)) + sz(rc(x)); }

int stk[maxh+1]; //栈，insert 用
//插入数值 vl
void insert(int vl){
    //x 是当前节点编号，一开始为根节点 rt，h 是节点 x 的高度，top 为栈顶
    int x = rt, h = maxh, top = 1;
    while(h > 0){
        stk[top++] = x;
        if ((vl>>h-1)&1){
            if (!rc(x)) rc(x) = ++ncnt;
            x = rc(x);
        }
        else{
            if (!lc(x)) lc(x) = ++ncnt;
            x = lc(x);
        }
    }
    ++sz(x);
    while(top) ud(stk[--top]);
}

//查询异或 vl 后的第 rk 大
int qry(int rk, int vl, int h){ //rk 是排名，vl 是要异或的数值，h 是节点 x 的高度
    //x 是当前节点编号，一开始为根节点 rt，lch 与 rch 分别是进行隐式变换后的左右子树，ans 是答案
    int x = rt, lch, rch, ans = 0; 
    while(h > 0){
        //进行隐式变换
        if ((vl>>h-1)&1) lch = rc(x), rch = lc(x);
        else lch = lc(x), rch = rc(x);
        
        if (rk <= sz(lch)){
            ans &= ~(1<<h-1); //将第 h-1 位置 0
            x = lch;
        }
        else{
            rk -= sz(lch);
            ans |= (1<<h-1); //将第 h-1 位置 1
            x = rch;
        }
        --h;
    }
    return ans;
}
```



例题：

[P4735 最大异或和](https://www.luogu.com.cn/problem/P4735)

这题难点在于查询操作。

因为异或运算有可减性，连续区间异或和问题可以通过差分转化为前缀和相减问题（当然异或运算既相当于加运算也相当于减运算），即：

$$
a(p)\oplus...\oplus a(p+x)= s(p-1)\oplus s(p+x)
$$
其中 $s(i) = a(1)\oplus ... \oplus a(i)$。

于是查询操作就转化为求一个 $p \in [l-1, r-1]$，使得 $a(p)\oplus val$ 最大，其中 $val = s(n)\oplus x$。

相当于是将 $\{a(p) | p\in[l-l,r-1]\}$ 插入 $\text{01-Trie}$，然后再在这颗 $\text{01-Trie}$ 上查询最大值。

$\text{01-Trie}$ 树是一颗权值线段树，这样的查询操作很难不让人联想到主席树，所以可以建立可持久化 $\text{01-Trie}$（本质还是主席树）。

查询操作变为在主席树上的查询，隐式变换的操作不变。

```cpp
#define maxh 31
#define maxn /*...*/
#define maxnn /*...*/

int rt[maxn] = 1, ncnt = 0; //rt[i] 是第 i 版本的 Trie 树的根节点，ncnt 是建立可持久化 Trie 已使用的节点数
struct Trie{
    int ch[2], sz; //ch[0] 是左儿子，ch[1] 是右儿子，sz 是当前子树下有多少个数值
    Trie(){ ch[0] = ch[1] = sz = 0; }
    Trie(const Trie &t){ *this = t; }
    Trie &operator=(const Trie &t){ ch[0] = t.ch[0]; ch[1] = t.ch[1]; sz = t.sz; return *this}
} trie[maxnn];
#define ch(x,fg) trie[x].ch[fg]
#define sz(x) trie[x].sz
#define lc(x) ch(x, 0)
#define rc(x) ch(x, 1)

int new_node(int nn){
    int x = ++ncnt;
    trie[x] = trie[nn];
    return x;
}

inline void ud(int x){ sz(x) = sz(lc(x)) + sz(rc(x)); }

void insert(int x, int &y, int vl, int h){ //x 是历史版本，y 是新版本
    y = new_node(x);
    int fg = (vl>>h-1)&1;
    if (h > 1) insert(ch(x,fg), ch(y,fg), vl, h-1), ud(y);
    else ++sz(y);
}

//查询异或 vl 后的第 rk 大
int qry(int x, int y, int rk, int vl, int h){ //x 是负版本，y 是正版本
    int ans = 0;
    while(h > 0){
        int fg = (vl>>h-1)&1, //隐式变换，为 0 代表小的那方是左子树，为 1 代表小的那方是右子树
        	lsz = sz(ch(y,fg)) - sz(ch(x,fg));
        if (rk <= lsz){
            ans &= ~(1<<h-1); //将第 h-1 位置 0
            x = ch(x, fg), y = ch(y, fg);
        }
        else{
            rk -= lsz;
            ans |= (1<<h-1); //将第 h-1 位置 1
            x = ch(x, fg^1), y = ch(y, fg^1);
        }
        --h;
    }
    return ans;
}
```
