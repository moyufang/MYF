

### SSH

SSH的默认端口是22，也就是说，你的登录请求会送进远程主机的22端口。使用p参数，可以修改这个端口。
```
ssh -p 2222 user@host
```
上面这条命令表示，ssh直接连接远程主机的2222端口。

当远程主机的公钥被接受以后，它就会被保存在文件$HOME/.ssh/known_hosts之中。下次再连接这台主机，系统就会认出它的公钥已经保存在本地了，从而跳过警告部分，直接提示输入密码。

```
ssh-keygen # 生成一个 ssh 公钥
```
在 `$HOME/.ssh/` 目录下，会新生成两个文件：`id_rsa.pub`和`id_rsa`。前者是你的公钥，后者是你的私钥。

重启ssh服务。

```
// ubuntu系统  
service ssh restart

// CentOS 系统
systemctl restart sshd

// debian系统  
/etc/init.d/ssh restart
```

有的机器进行加固，是不允许用户root权限登录ssh 服务的，解决方案是修改配置/etc/ssh/sshd_config ，添加如下内容，然后重启ssh服务 sudo /etc/init.d/ssh restart
```
PermitRootLogin yes
UsePAM yes
  ```

这种方法要求用户必须提供自己的公钥。如果没有现成的，可以直接用ssh-keygen生成一个：
```
ssh-keygen
```
将公钥传送到远程主机host上面，再登录，就不需要输入密码了。
```
ssh-copy-id user@host
```

远程主机将用户的公钥，保存在登录后的用户主目录的$HOME/.ssh/authorized_keys文件中。公钥就是一段字符串，只要在 `authorized_keys` 文件的末尾追加一行公钥就行了。

[SSH原理与运用（一）：远程登录](https://www.ruanyifeng.com/blog/2011/12/ssh_remote_login.html)
[VSCode配置 SSH连接远程服务器+免密连接教程](https://zhuanlan.zhihu.com/p/667236864)
[ssh登录失败排查过程](https://blog.csdn.net/q_xiami123/article/details/114365263)
[CentOS 7.3 root用户远程登录失败原因](https://blog.csdn.net/W_TF2017/article/details/108320250)
在 /etc/ssh/sshd_config文件中添加 Allow Users

scp 传输文件指令
```
scp -r [file_or_dir_path] [user]@[destination]:[path_in_destination]
```
### 系统设置

**.profile** **etc**

如果想要让某些指令每次登录某用户时都生效（如 `alias` `ln` `export`），可以在该用户的 `.bashrc` 文件末尾中加入该指令
### 磁盘

### 网络


#### Shell 教程


[Shell 教程](https://www.runoob.com/linux/linux-shell.html)

[【Linux】shell编程之循环语句](https://blog.csdn.net/h15162064289/article/details/130399009)

一些 `for` 和 `case` 的事例
```sh
for i in {1..5}; do
	echo $i
	if [ $i -eq 2 ]; then
		echo "I found 2"
	elif [ $i -ne 2 ]; then
		echo "I didn't find 2"
	fi
done
chars=(a b c d)
for c in ${chars[@]}; do
	if [ $c = a ]; then
		echo "I found a"
	fi
done

case x in
	abc*def.c)
	;;
	ttt[a-z][0-9].cpp)
	;;
esac
```

# Linux 启动过程

一般的 linux 内核启动流程如下

- bios 芯片将 bootloader 从载入内存
- bootloader 通过 UEFI(一种接口协议) 磁盘，找到 kernel image，将 kernel 载入内存
	- 几乎所有磁盘都提供了便于 bios 访问磁盘的程序（一般是 Linear Block Access），性能不比内核启动的磁盘驱动。
- kernel 初始化设备及其驱动程序
	- CPU、内存、磁盘
- kernel 挂载 root 文件系统
- kernel 启动 pid 为 1 的被称作 init 的进程，用户空间从此启动
	- init 进程有三个主要版本：SysVinit(Red Hat Linux等), Upstart(旧Ubuntu), systemd。很多 Linux distro 正转向 systemd
### bootloader

bootloader 完成以下任务
- 从多个内核中选取一个使用
- 从多个内核参数集中选取一个使用
- 允许用户手动更改 内核的选取 以及 内核参数的设置
- 启动内核

### initramfs

initramfs 的诞生源于一个问题：如果一个磁盘的驱动程序存储在其自身上，不通过 uefi 访问磁盘的 kernel 如何加载它呢？

linux 的方案是将这些驱动程序打包成归档文件，然后由 bootloader 在加载 kernel 前载入内存，这样的归档文件被称作 initramfs，kernel 启动后会在内存上读取 initramfs 并挂载到 `/` 上，然后运行 initramfs 中的 init 程序，initramfs 的 init 程序一般为 bash 脚本，在退出前会完成一些必要工作（如加载磁盘驱动程序）

所以 initramfs 的本质是一个物理存储媒介是 ram 内存的文件系统。linux 实现中，initramfs 是通过 gzip 压缩的 cpio 归档文件。

initramfs 不是必要的，比如 kernel 本身包含了相应的磁盘驱动程序的情况下。
