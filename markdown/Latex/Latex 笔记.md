LaTeX 的使用分为两个部分：
- **发行版**（TeX Live）：提供编译器、宏包等核心组件，相当于“引擎”。
- **编辑器**（VS Code）：提供编写代码、一键编译、实时预览的界面，相当于“操作台”。
### 安装 Latex(windows11)+vscode插件

访问清华大学镜像站 CTAN，下载 `install-tl-windows.exe`（约 5 MB），这是网络安装版，安装时自动下载所需组件，修改安装路径到非系统盘（如 `D:\texlive\2026`），路径不要包含中文或空格。

```
#验证安装
tex --version
xelatex --version
```

打开 VS Code，点击左侧「扩展」图标（或按 `Ctrl + Shift + X`），搜索「LaTeX Workshop」，点击安装。这是唯一核心插件