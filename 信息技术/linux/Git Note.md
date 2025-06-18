对某个指令使用 `--help`，会在浏览器打开一个 html 手册（本地html文件），如
```
git push --help
```

常用指令备忘
```
git init
git config --global user.name "Your name"
git config --global user.email "email@example.com"

git add <filename>
git rm <filename>
git commit -m "Messages"
git status
git diff <filename>
git restore <filename>

git log --pretty=oneline
git reflog //git指令序列的简要日志
git reset <hash> //版本回退
git diff <hash> <hash> //分析两个版本的差异

git checkout -- <filename> //用版本库里的文件替换工作区中的文件
git branch <name> //创建分支；去掉<name>查看所有分支
git branch -d <name> //删除分支
git checkout -b <name> //创建新分支并切换
git switch -c <name> //创建并切换到新分支，删掉 -c 则是创建

git stash //将工作区与缓存区的文件打包入栈，缓存起来（已消失）
git stash list //显示缓存的包

git stash apply [stash@{<number>}] //将栈指定的文件包恢复，不指定则恢复栈顶
git stash drop [stash@{<number>}] //在栈中删除指定包，默认栈顶
git stash pop [stash@{<number>}] //相当于 apply+drop，从栈中弹出指定文件包，不指定则弹出栈顶的包

git restore -s
```

### 远程仓库

```
git remote add <respo_name> <url|git@url> //<respo_name> 是本地git对远程仓库的别名，在 push 和 pull 中会用到

git remote push -u <respo_name> <branch_name> //将分支<branch_name> push 到远程仓库，-u 在第一次 push 时使用，如果需要强制覆盖远程仓库的同名分支，可用 -f 选项

git remote rm <respo_name> //取消连接，远程仓库并未被删除

git push -d <respo_name> <branch_name> //在远程仓库中删除 <branch_name> 分支

```

### git 配置代理

设置代理加速 git 对 github 的访问。
```
git config --global -l //显示所有设置

//给git设置代理
git config --global http.proxy 127.0.0.1:10809  
git config --global https.proxy 127.0.0.1:10809
```
