

**WSL 2 的安装和配置请参考 [WSL 2 官方文档](https://docs.microsoft.com/zh-cn/windows/wsl/install)。** 


如果忘记了 Linux 分发版的密码：

1. 请打开 PowerShell，并使用以下命令进入默认 WSL 分发版的根目录：`wsl -u root`

   > 如果需要在非默认分发版中更新忘记的密码，请使用命令：`wsl -d Debian -u root`，并将 `Debian` 替换为目标分发版的名称。

需要安装和配置开发环境，包括 gcc, make, python, qemu 等。

```
echo "开始安装 gcc、make、Python 和 QEMU..."
echo "更新软件包列表..."
sudo apt update
sudo apt install -y gcc make
sudo apt install -y python3 python3-pip
sudo apt install qemu-system-x86 build-essential gdb
echo "检查安装结果："
echo "gcc 版本："
gcc --version
echo "make 版本："
make --version
echo "Python 版本："
python3 --version
echo "QEMU 版本："
qemu-system-x86_64 --version
echo "安装完成！"
```

