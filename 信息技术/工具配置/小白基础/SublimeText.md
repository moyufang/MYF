[SublimeText配置介绍](https://www.cnblogs.com/Fighoh/p/18085925)
gcc
```json
{
    "shell_cmd": "gcc -o \"${file_base_name}\" \"${file}\" && \"${file_base_name}\"",
    "selector": "source",
    "working_dir": "${project_path:${folder}}",
    "encoding":"utf-8",
}
```
g++
```json
{
    "shell_cmd": "g++ -o \"${file_base_name}\" \"${file}\" && \"${file_base_name}\"",
    "selector": "source.c, source.c++",
    "working_dir": "${project_path:${folder}}",
    "encoding":"utf-8",
}
```
python12
```json
{
    "env": {"PYTHONIOENCODING": "utf-8"},
    "cmd": ["C:/Program Files/Python12/python.exe", "-u", "$file"],
    "file_regex": "^[ ]*File \"(...*?)\", line ([0-9]*)",
    "selector": "source.python",
    "shell": "true",
    "encoding":"utf-8"
}
```