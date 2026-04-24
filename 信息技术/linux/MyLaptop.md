### 环境

笔记本：i5-13500H(4性能核8能耗核，16线程)，16RAM，512GB+1TB两个固态硬盘
双系统：Windows11 + [Ubuntu Server 24.04.4](https://mirrors.tuna.tsinghua.edu.cn/ubuntu-releases/24.04.4/ubuntu-24.04.3-live-server-amd64.iso "ubuntu-24.04.3-live-server-amd64.iso")
磁盘分区概况：
```
nvme0n1 (476.92G)
  ├─ nvme0n1p1 (260M, vfat, EFI)
  ├─ nvme0n1p2 (199.85G, ntfs, Windows C:)
  ├─ nvme0n1p3 (1.95G, 恢复分区)
  └─ free space (274.87G)
nvme1n1 (931.51G)
  └─ free space (931.51G) → ext4, /
```
将使用整个 D 盘作为 "Ubuntu Live Server 24.04.4" 的 `/` 挂载磁盘
### Ubuntu Live Server 24.04.4安装笔记

##### 安装 u 盘的制作

下载好 "[ubuntu-24.04.4-live-server-amd64.iso](https://mirrors.tuna.tsinghua.edu.cn/ubuntu-releases/24.04.4/ubuntu-24.04.4-live-server-amd64.iso "ubuntu-24.04.4-live-server-amd64.iso")" 镜像后，使用 rufus 制作 uefi 启动盘

分区类型 "GPT"，目标系统类型 "UEFI" 文件系统 "FAT32（默认）" 簇大小 任意

点击 "开始" 后会提示选择 "ISO 镜像写入" 还是 "DD 镜像写入"，ubuntu 官方推荐是 dd，兼容性高。 

##### 安装过程

1. 重启后反复按 "Enter"，选择其它启动方式，然后从 U 盘启动，出现 GRUB 菜单 → 选择 **Try or Install Ubuntu Server**。
2. 安装过程关键步骤

| 步骤      | 操作                                                  |
| ------- | --------------------------------------------------- |
| 语言      | English（默认）                                         |
| 键盘      | 按需选择（如 English US）                                  |
| 网络连接    | 配置以太网或wifi                                          |
| 代理      | 留空                                                  |
| 镜像源     | 保持默认或选国内镜像（如 `mirrors.tuna.tsinghua.edu.cn/ubuntu`） |
| 存储配置    | 选择 **Customed**（手动分区）                               |
| openssh | 选择安装                                                |
3. 手动分区
- 将 D 盘的 "931.51G" 挂载到 "/" 上
- 文件系统是 ext4，即 linux 的标准文件系统，兼容性广

4. 用户设置
- Your name: shuyu
- Your servers name: shuyu
- User name: miu
- User password: 1

##### networkd-wait

启动时 `systemd-networkd-wait-online.service` 等待时间过长（默认约 2 分钟），通常是因为某个网络接口（如无线网卡或未插线的有线网卡）一直未能完成配置，导致服务超时。

1. 方法一：屏蔽该等待
```bash
sudo systemctl mask systemd-networkd-wait-online.service
```
2. 方法二：设置 `systemd-networkd-wait-online.service` 的超时时间
```
sudo systemctl edit systemd-networkd-wait-online.service
```
在打开的编辑器中，粘贴以下内容：
```bash
[Service]
ExecStart=
ExecStart=/lib/systemd/systemd-networkd-wait-online --any --timeout=10
```
- 第一个空的 `ExecStart=` 用于清除掉默认的启动命令    
- 第二条指定了新的启动命令。这个例子中，`--any` 参数表示只要有任一网络接口成功连线即可，`--timeout=10` 将超时时间设置为 1 秒。
如果是 “nano" 编辑器，则
- 按 `ctrl + O` 保存文件，文件名不要使用默认的(隐藏的文件，无法被识别到)，可以为 `override.conf`
- 按 `ctrl + X` 关闭编辑器
然后重新加载 systemd 的配置以使修改生效
```bash
sudo systemctl daemon-reload
```

### 基本开发环境配置

##### 通过无线网卡连接到校园网(需要"学号+密码")

安装 ubuntu 时，使用手机热点作为临时 wifi，仍然有效

```
# 更新软件包列表并安装必要工具
sudo apt update
sudo apt install wpasupplicant wireless-tools
```

`ip a` 查看所有网卡，其中 "wl" 开头的通常是无线网卡。
本机无线网卡为 "wlp0s20f3"

在 `/etc/netplan/` 下创建或者覆盖 "50-cloud-init.yaml"，写入

```yaml
network:
  version: 2
  renderer: networkd
  wifis:
    wlp0s20f3:
      dhcp4: true
      access-points:
        "SYSU-SECURE":
          auth:
            key-management: eap
            method: peap
            identity: "YourNetID"
            password: "YourPassword"
            phase2-auth: mschapv2
```

应用配置
```bash
# 测试配置语法
sudo netplan generate
# 如果语法无误，应用配置
sudo netplan apply
```
测试是否有效
```bash
# 尝试连接互联网
ping 4 -c 8.8.8.8
```
##### 配置基本开发环境

```bash
sudo apt update
sudo build-essentials -y
sudo apt install python3 python3-pip -y
sudo apt install python3-numpy python3-scipy -y
sudo apt install tmux -y
sudo apt install curl wget net-tools git -y
# 提供了一套标准化的接口和工具，使用户轻松、安全地管理 APT 软件源，避免手动编辑配置文件的繁琐和风险。
sudo apt install software-properties-common -y
```
##### 配置 SSH

Ubuntu Server 默认已安装 OpenSSH Server，但可能未启动或防火墙阻止。执行：

```bash
# 查看 SSH 服务状态
sudo systemctl status ssh
# 如果未运行，启动并设为开机自启
sudo systemctl enable --now ssh
# 确认服务监听，22 端口一般是 ssh 的监听端口
sudo ss -tlnp | grep :22
# 重启 ssh
sudo systemctl restart ssh
```

##### 方法一（废弃）

安装 `avahi-daemon`（因为 windows 始终不兼容 mDNS 的缘故，该方法废弃）

>`avahi-daemon` 是 Linux 系统上实现 **mDNS（多播DNS）** 和 **DNS-SD（DNS服务发现）** 的守护进程[](https://juejin.cn/post/7541317663558844468)。简单来说，它让局域网里的设备可以自动“自我介绍”和“互相发现”，省去了手动配置 IP 或 DNS 的麻烦。

```bash 
sudo apt install avahi-daemon
# 启动 Avahi 服务并设置开机启动
sudo systemctl enable --now avahi-daemon
# 查看启动状态
sudo systemctl status avahi-daemon
```

修改 ubuntu 主机名
```bash
# 查看主机信息
hostnamectl
# 修改主机名
sudo hostnamectl set-hostname shuyu
```

##### 方法二

在 Windows PC 主机上安装 openssh后，尝试进行 ssh
```
ssh <user_name>@<server_ip>
```
第一次连接会提示确认指纹，输入 `yes` 即可，然后输入 `miu` 用户的密码。

在 `C:\Users\<用户名>\.ssh\config` 添加如下内容
```
Host <server_name>
    HostName <server_ip>
    User <user_name>
```
之后在 PC 上可以直接 "ssh <user_name>@<server_name>" 进行远程访问了。

在 PC 上生成密钥（如果没有生成过的话）
```
ssh-keygen -t ed25519
```
然后找到并打开扩展名为".pub" 的公钥文件 `C:\Users\<用户名>\.ssh\*.pub`，把内容复制并添加新一行到 ubuntu 上 `~/.ssh/authorized_keys` 

##### vscode 使用 remote-ssh

- 在 vscode 扩展商店中搜索并安装 "remote-ssh" 扩展。
- `Ctrl+Shift+P` 打开命令面板，输入 `Remote-SSH: Add New SSH Host...`
- 输入连接命令 `ssh miu@shuyu`

mariadb, rabbitmq, keystone, apache, glance, nova, neutron, cinder, swift