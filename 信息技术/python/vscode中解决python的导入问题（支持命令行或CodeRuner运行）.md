用快捷键 `Ctrl + Shift + P` 打开搜索，输入 “>Preferences: Open Workspace Settings (JSON)” 为项目创建 `.vscode/settings.json`

在 `.vscode/settings.json` 输入
```json
{
    "code-runner.executorMap": {
        "python": "set PYTHONPATH=$workspaceRoot && python -u $fullFileName"
    },
    "terminal.integrated.env.windows": {
        "PYTHONPATH": "${workspaceFolder}"
    },
    "terminal.integrated.env.osx": {
        "PYTHONPATH": "${workspaceFolder}"
    },
    "terminal.integrated.env.linux": {
        "PYTHONPATH": "${workspaceFolder}"
    }
}
```

之后在所有python文件中，可以以 `${workspaceFolder}` 为基准路径的相对路径在多文件中进行互相导入

例如以下例子：

项目结构
```
project_name/
  test/
    yes.py
  no.py
```
其中 te/yes.py
```python
import no
```
`no.py`
```python
print("no")
```

在命令行中运行`python te/yes.py`，或者用 Code Runner 运行 yes.py，根据输出可知，no被正确导入了。