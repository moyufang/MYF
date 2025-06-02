##### Add "@" & "-" before commands.

默认情况下，`make`会打印出它执行的每一条命令。如果我们不想打印某一条命令，可以在命令前加上`@`，表示不打印命令（但是仍然会执行）：

```makefile
no_output:
	@echo 'not display'
	echo 'will display'
```

`make`在执行命令时，会检查每一条命令的返回值，如果返回错误（非0值），就会中断执行，对于执行可能出错，但不影响逻辑的命令，可以用`-`忽略。

##### Seek c-file utomatically

```makefile
# $(wildcard *.c) 列出当前目录下的所有 .c 文件: hello.c main.c
# 用函数 patsubst 进行模式替换得到: hello.o main.o
OBJS = $(patsubst %.c,%.o,$(wildcard *.c))
TARGET = world.out

$(TARGET): $(OBJS)
	cc -o $(TARGET) $(OBJS)

clean:
	rm -f *.o $(TARGET)
```

##### `$@` and `$^` and `$<`

```makefile
world.out: hello.o main.o
	@echo '$$@ = $@' # 变量 $@ 表示target
	@echo '$$< = $<' # 变量 $< 表示第一个依赖项
	@echo '$$^ = $^' # 变量 $^ 表示所有依赖项
	cc -o $@ $^
```

##### Some useful command

```
basename #去除扩展名，如 `a.c b.c` -> 'a b'
notdir #去除路径，如 `./src/a.c` -> `a.c`
addprefix
addsuffix
```