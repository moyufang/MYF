
[Linux 命令大全](https://www.runoob.com/linux/linux-command-manual.html)

# 基本命令

### 文件查看

基本信息
```
ls; ll; tree
pwd; which; whereis;
find
```

**find**
```
find <path> -<matching_method> <matching_condition>
```
popular used matching_method
```
-name //
-type <f; d; l; c; b>
-maxdepth
-mindepth
```

文件内容
```
cat; <();
```

**cat**

查看文件内容

查看系统内核版本
```
cat /proc/version
```

**diff**
```
diff -qbB <file1> <file2>
```
这行比较文件时不考虑空白行、行末是否有空格、行中空格长度是否相等。
注意行首空格的长度会被比较。
- -q或--brief 　仅显示有无差异，不显示详细的信息。
- -b或--ignore-space-change 　不检查空格字符的不同。
- -B或--ignore-blank-lines 　不检查空白行。
- -y或--side-by-side 　以并列的方式显示文件的异同之处。

**cmp**
```
cmp <file1> <file2>
```
显示第一个不同的字符及其行数。然而没有忽略行末空格与空白行的选项，不好用。

**<()**
```
vim <(busybox --list)
```

### 文件管理

```
touch; rm; mv; cp; ln;
wget; tar;
```
### 权限管理
```
chmod; chown; 
```
### 进程

```
ps; pstree;
kill; pkill
```

### 其它

```
alias			//对命令起别名
readlink -f 	//找到软连接的最终文件
```

