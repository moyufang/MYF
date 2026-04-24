

### 1. 变量

定义变量时，等号两边**不能有空格**。引用变量时使用 `$` 符号。

```bash
# 定义变量
name="Alice"
age=25

# 使用变量
echo $name
echo "年龄: ${age}"   # 花括号可选，用于明确边界
```

**常用变量操作：**

| 语法                | 说明           |
| ----------------- | ------------ |
| `${var}`          | 获取变量值        |
| `${#var}`         | 获取字符串长度      |
| `${var:-default}` | 变量为空时使用默认值   |
| `${var:=default}` | 变量为空时赋默认值    |
| `${var:?error}`   | 变量为空时报错      |
| `${var:+value}`   | 变量非空时返回value |
| `${var#pattern}`  | 删除最短前缀       |
| `${var##pattern}` | 删除最长前缀       |
| `${var%pattern}`  | 删除最短后缀       |
| `${var%%pattern}` | 删除最长后缀       |
| `${var/old/new}`  | 替换第一个匹配      |
| `${var//old/new}` | 替换所有匹配       |

```bash
path="/home/user/file.txt"
echo ${path#*/}      # home/user/file.txt
echo ${path##*/}     # file.txt
echo ${path%.txt}    # /home/user/file
```

**特殊变量：**

| 变量 | 说明 |
|------|------|
| `$0` | 脚本名称 |
| `$1` ~ `$9` | 位置参数（第1~9个参数） |
| `${10}` | 第10个及以上参数需用花括号 |
| `$#` | 参数个数 |
| `$@` | 所有参数（视为独立字符串） |
| `$*` | 所有参数（视为一个整体字符串） |
| `$$` | 当前Shell进程ID |
| `$?` | 上一条命令的退出状态（0成功，非0失败） |
| `$!` | 最后一个后台进程的PID |

---

### 2. 分支

#### if 语句

```bash
if [ 条件 ]; then
    # 条件为真时执行
elif [ 其他条件 ]; then
    # 其他条件为真时执行
else
    # 都不满足时执行
fi
```

**常用测试条件：**

| 表达式 | 含义 |
|--------|------|
| `[ -f file ]` | 文件存在且是普通文件 |
| `[ -d dir ]` | 目录存在 |
| `[ -e file ]` | 文件存在 |
| `[ -r file ]` | 文件可读 |
| `[ -w file ]` | 文件可写 |
| `[ -x file ]` | 文件可执行 |
| `[ str1 = str2 ]` | 字符串相等 |
| `[ str1 != str2 ]` | 字符串不等 |
| `[ -z str ]` | 字符串为空 |
| `[ -n str ]` | 字符串非空 |
| `[ num1 -eq num2 ]` | 数字相等 |
| `[ num1 -ne num2 ]` | 数字不等 |
| `[ num1 -gt num2 ]` | 大于 |
| `[ num1 -lt num2 ]` | 小于 |
| `[ num1 -ge num2 ]` | 大于等于 |
| `[ num1 -le num2 ]` | 小于等于 |

```bash
if [ -f "/etc/passwd" ]; then
    echo "passwd文件存在"
elif [ -d "/etc" ]; then
    echo "etc目录存在"
else
    echo "都不存在"
fi
```

**逻辑运算：**
- 与：`[ 条件1 ] && [ 条件2 ]` 或 `[ 条件1 -a 条件2 ]`
- 或：`[ 条件1 ] || [ 条件2 ]` 或 `[ 条件1 -o 条件2 ]`
- 非：`[ ! 条件 ]`

#### case 语句

```bash
case $变量 in
    模式1)
        命令
        ;;
    模式2|模式3)
        命令
        ;;
    *)
        默认命令
        ;;
esac
```

```bash
case $1 in
    start)
        echo "启动服务"
        ;;
    stop|restart)
        echo "停止或重启"
        ;;
    *)
        echo "用法: $0 {start|stop|restart}"
        ;;
esac
```

---

### 3. 循环

#### for 循环

```bash
# 遍历列表
for i in 1 2 3 4 5; do
    echo "数字: $i"
done

# 使用花括号范围
for i in {1..10}; do
    echo $i
done

# 步进 (bash 4.0+)
for i in {1..10..2}; do
    echo $i   # 1,3,5,7,9
done

# C 风格
for ((i=0; i<5; i++)); do
    echo $i
done

# 遍历文件
for file in *.txt; do
    echo "处理 $file"
done
```

#### while 循环

```bash
count=1
while [ $count -le 5 ]; do
    echo "count: $count"
    ((count++))
done

# 读取文件逐行
while read line; do
    echo "行内容: $line"
done < file.txt
```

#### until 循环

```bash
count=1
until [ $count -gt 5 ]; do
    echo $count
    ((count++))
done
```

**循环控制：**
- `break`：跳出循环
- `continue`：跳过本次循环剩余部分，进入下一次迭代

---

### 4. 函数与传参

#### 定义和调用

```bash
# 定义函数
function myfunc() {
    echo "函数被调用"
}

# 或省略 function 关键字
myfunc2() {
    echo "参数1: $1"
    echo "参数2: $2"
    echo "参数个数: $#"
    echo "所有参数: $@"
    return 0   # 返回值（0-255）
}

# 调用
myfunc
myfunc2 hello world
```

**函数内特殊变量：**

| 变量 | 说明 |
|------|------|
| `$1, $2...` | 传递给函数的参数 |
| `$#` | 传递给函数的参数个数 |
| `$@` | 所有参数（独立） |
| `$*` | 所有参数（整体） |
| `$?` | 获取函数返回值 |

```bash
# 求和函数
sum() {
    local a=$1   # local 声明局部变量
    local b=$2
    echo $((a + b))
    # echo 输出作为函数返回值（通过命令替换获取）
}

result=$(sum 5 3)
echo "5+3=$result"   # 输出 8

# 使用 return 返回状态码
check_file() {
    if [ -f "$1" ]; then
        return 0
    else
        return 1
    fi
}

check_file /etc/passwd
if [ $? -eq 0 ]; then
    echo "文件存在"
fi
```

---

### 5. 常见语法糖/快捷操作

#### 命令替换

```bash
# 旧语法（反引号）
now=`date`

# 推荐语法 $()
now=$(date)
files=$(ls *.txt)
```

#### 算术运算

```bash
# 旧语法
sum=`expr 3 + 5`

# 推荐 $((...))
sum=$((3 + 5))
((sum++))          # 自增
((sum += 10))      # 加等于
```

#### 条件判断的快捷写法

```bash
# 逻辑与 &&，逻辑或 ||
[ -f file.txt ] && echo "文件存在"   # 条件成立才执行后面
[ -f file.txt ] || echo "文件不存在" # 条件不成立才执行后面

# 三元运算符效果
result=$(( a > b ? a : b ))
```

#### 双中括号 [[ ]]

比单中括号 `[ ]` 更强大、更安全（bash 内置）：

```bash
# 支持正则匹配
[[ $str =~ ^[0-9]+$ ]] && echo "全是数字"

# 支持逻辑运算符 && || 不需要 -a -o
[[ $a -gt 5 && $b -lt 10 ]]

# 字符串比较（不需要加引号）
[[ $name == Alice ]]

# 通配符匹配
[[ $file == *.txt ]]
```

#### 数组

```bash
# 定义数组
arr=(apple banana cherry)

# 访问元素
echo ${arr[0]}    # apple
echo ${arr[@]}    # 所有元素
echo ${#arr[@]}   # 数组长度

# 遍历
for fruit in "${arr[@]}"; do
    echo $fruit
done
```

#### 字符串操作糖

```bash
# 读取输入
read -p "请输入姓名: " name

# 默认值
echo ${name:-"匿名"}

# 输出重定向
echo "日志" >> log.txt    # 追加
echo "错误" >&2           # 输出到标准错误
```

#### 调试模式

```bash
# 在脚本开头启用
set -x   # 打印执行的每一条命令
set -e   # 遇到错误立即退出
set -u   # 使用未定义变量时报错

# 组合使用
set -eux

# 临时开启
set -x
your_command
set +x
```

#### Here Document

```bash
cat << EOF
这是多行文本
变量会展开: $HOME
EOF

cat << 'EOF'   # 引号阻止变量展开
$HOME 会原样输出
EOF
```

#### 进程替换

```bash
# 比较两个命令的输出
diff <(ls dir1) <(ls dir2)
```

---

### 总结示例脚本

```bash
#!/bin/bash
# 示例：综合演示

set -u   # 未定义变量报错

# 函数：打印帮助
usage() {
    cat << HELP
用法: $0 [options] <file>
选项:
  -c, --count   统计行数
  -h, --help    显示帮助
HELP
}

# 参数解析
count_mode=false
file=""

while [[ $# -gt 0 ]]; do
    case $1 in
        -c|--count)
            count_mode=true
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        -*)
            echo "未知选项: $1"
            exit 1
            ;;
        *)
            file=$1
            shift
            ;;
    esac
done

if [[ -z "$file" ]]; then
    echo "错误: 请指定文件"
    usage
    exit 1
fi

if [[ ! -f "$file" ]]; then
    echo "错误: 文件 $file 不存在"
    exit 1
fi

if $count_mode; then
    lines=$(wc -l < "$file")
    echo "文件行数: $lines"
else
    cat "$file"
fi
```

掌握以上内容，您就可以编写大多数日常所需的 Bash 脚本了。