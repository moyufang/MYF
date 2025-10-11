
# Logging 日志管理器

首先看一段代码的输出：
```python
def complex_operation(data: list) -> None:
  logger.debug("开始处理数据，记录数: %d", len(data))
  try:
    # 处理逻辑
    logger.info("数据处理进度: 50%")
    logger.info("操作完成")
  except Exception as e:
    logger.error("处理失败: %s", e, exc_info=True)
    raise
    
complex_operation(data)

"""
D:\AutoGame\AutoBangDream>set PYTHONPATH=d:\AutoGame\AutoBangDream && python -u "d:\AutoGame\AutoBangDream\temp.py"
[DEBUG @ temp.py:62] 开始处理数据，记录数: 99
[INFO  @ temp.py:65] 数据处理进度: 50%
[INFO  @ temp.py:66] 操作完成
"""
```

Python 的 `logging` 模块提供了一个灵活且强大的日志记录系统，它允许开发者记录应用程序运行时的各种信息，从调试信息到错误报告。

`logging` 主要优势有
- 多级别日志记录：支持不同严重程度的日志级别
- 灵活的输出目标：可输出到控制台、文件、网络等
- 可配置的格式：自定义日志消息的格式
- 模块化设计：可为不同模块设置不同的日志记录器
- 线程安全：在多线程环境中安全使用

下面是生成一个 logging 的代码

```python
import logging
import sys
from pathlib import Path
from typing import Optional

def setup_logging(
    name: str,
    level: int = logging.INFO,
    log_file: Optional[Path] = None,
    format_string: Optional[str] = None
) -> logging.Logger:
    """设置统一的日志配置
    
    Args:
        name: 日志器名称，通常是 __name__
        level: 日志级别，默认INFO
        log_file: 日志文件路径，如不提供则只输出到控制台
        format_string: 自定义日志格式
    
    Returns:
        配置好的日志器实例
    """
    if format_string is None:
        format_string = (
            '[%(levelname)-5s @ %(filename)s:%(lineno)d] %(message)s'
        )
    
    formatter = logging.Formatter(format_string)
    
    logger = logging.getLogger(name)
    logger.setLevel(level)
    
    # 避免重复添加处理器
    if not logger.handlers:
        # 控制台处理器
        console_handler = logging.StreamHandler(sys.stdout)
        console_handler.setFormatter(formatter)
        logger.addHandler(console_handler)
        
        # 文件处理器（如果提供了文件路径）
        if log_file:
            log_file.parent.mkdir(parents=True, exist_ok=True)
            file_handler = logging.FileHandler(log_file)
            file_handler.setFormatter(formatter)
            logger.addHandler(file_handler)
    
    # 防止日志传递到根日志器
    logger.propagate = False
    
    return logger

# 使用示例
logger = setup_logging(
    name=__name__,
    level=logging.DEBUG,
    log_file=Path("logs/app.log")
)

```

在 `format_string` 中，有`levelname` `filename` `lineno` `message` 等默认变量，也可以自定义变量
```python
state_logger = setup_logging(
  name='state_logger',
  level=logging.DEBUG,
  format_string='[STATE "%(state)s" @ %(filename)s:%(lineno)d ] %(message)s'
)

class StateFilter(logging.Filter):
  def filter(self, record):
    record.username = getattr(record, 'state', 'unknown')
    return True

state_logger.addFilter(StateFilter())
def LogS(state:str, *arg):
  state_logger.debug(*arg, extra={'state':state})
  
LogS("ready", "song loaded")
# 输出 [STATE "ready" @ main.py:73 ] song loaded
```