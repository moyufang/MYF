# 从框架开始

目的

# 从零开始
### 短期目标

blog website 
（1）展示功能（单个 blog 的展示）
（2）导航功能（便于用户定位到单个 blog）
### Frame
md 库
- `/md/`
裸 html 库
- `/raw_html/`
Server 图库
- `/img/`
- 图片命名格式
	- `md_img-<id>-<img_num>.png`
	- `icon-<icon_name>.icon`
	- `<other>-<other_name>.png`
Blog Manager
- `/data/blog_manager.json`
	- `blog_id_list`：装有有效 blog 的 `blog_id`

一个 Blog Struct 可以抽象为
- blog_id：3位`[0-9a-zA-Z]` 共 62 个字符组成的 id，blog总数不能超过 $62^{3}=238328$。
- 标题 title、简介 prelog、最后编辑时间 edited_time、标签列 tag_list
- md 源码（文本、代码、图片、latex）`<blog_id>.md`
- 裸 html `<blog_id>_raw.html`

后端：python + flask
- Resource
	- Server 图库
	- Server 裸 html 库
	- Server md 库
- Module
	- Blog Manager
- API
	- 响应 Server 图库 和 Server 裸 html 库 的资源获取请求
	- 本地 md 文件预处理（输入：本地某md文件的绝对路径；输出：完成以下功能）
		- md 源码图片链接预处理、并复制图片文件到 Server 图库
		- md 文件渲染成 no-style-html 文件，存入 Server 裸 html 库
前端：
- markdown 裸 html 渲染
	- 基本的 markdown 语法支持
	- katex 渲染内联 latex 公式

主页
1. 首先导入 `blog_header.json`，记录有所有信息。
2. 根据 `blog_header.json` 导入主页该有的信息

### 任务
1. 前后端框架分离
2. 开写
### 其它动画
1. 悬停在标题时，字体变色

$$
f(x)=\sum_{i=0}^{n}x^{{i\over2}}
$$

```cpp
int main(){
    
    return 0;
}
```



