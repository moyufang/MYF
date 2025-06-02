
### Html5

基本骨架
```html
<!DOCTYPE html>
<html>
	<head>
		<title>Title</title>
		
	</head>
	<body>
		<h1></h1>
	</body>
</html>
```


##### 元素分类

简单可分为两大类

| 块级元素                      | 内联元素(行内元素)               |
| ------------------------- | ------------------------ |
| 在页面内独占一行                  | 不独占一行，只占自身大小             |
| 可以设置 `width` `height` 等属性 | 不可设置 `width` `height` 属性 |
| 可包含其它块元素或内联元素             | 一般包含内联元素而不包含块级元素         |
常见块级元素
```
div form h1...h6 hr p table ul ol
```
常见内联元素
```
a b em i span strong
```
行内块级元素（不换行、可设置 `width` `height`的块级元素）
```
button img input
```

### 我知道名字的 HTML 元素

```html
<title> <h1-h6> <p> <a> <br> <div> <img>
```

### CSS

CSS 中有两个概念：选择器 和 样式

在 html 中 `<head>` 内引入
```html
<link rel="stylesheet" href="./sample_css_file.css">
```

自定义属性
变量名必须为以 `--` 开头且区分大小写，比如在 `:root` 下定义的全局变量名可以在任意子 DOM 中访问到
```css
:root {
  --global-color: black;
}
.box {
  --local-color: --global-color;
  color: var(--local-color); /* 这个元素使用 red */
}
```

##### 选择器

全局初始化：做一些初始化，如果有专门 `<style>` 的话则会用专门的 
```css
*{
	font-size:14px;
}
```
"或“选择器：用逗号分隔
```css
class1,class2{
	width:234dx;
	height:234dx;
}

```
”与“选择器：不进行任何分隔
```css
.class1.class2{

}
```
选择器的优先级是
```
行内样式 > id 选择器 > 类选择器 > 元素选择器
```
对元素选择器，关系型选择器：

| 名称  | 语法    |
| --- | ----- |
| 后代  | E F{} |
| 子代  | E>F{} |
| 相邻  | E+F{} |
| 兄弟  | E~F{} |
##### html box

所有 html 元素都可以抽象成一个 box，分 `margin` `border` `padding` `content` 四个区域
![][html_box.png]

浏览器的默认 CSS 中，`<body>` 的 `margin` 属性默认为 `8px`
### 常见属性
##### 字体属性

```css
font-size: <number>{px,em,rem} //px 是像素，em 父元素的字体大小，rem 是根元素业也即 html 的大小\
	chrome 支持的最小 px 是 12 px，再小没变化
font-width: {<number>,bold,bolder,ligher} //
color: {red,#ff0000,rgb(255,0,0),rgba(255,0,0,0.5)}
font-family: {Microsoft} //可用空格分隔出不同字体，浏览器会选择最左一个可用字体
```
##### 背景属性
```css
background-image:;
background-color:;
background-size:{<width> [<height>],<percentage>,cover,contain}; //设置背景图片的大小
background-repeat:{no-repeat,repeat-x,repeat-y,repeat} //默认是 repeat，在两个方向上均重复平铺
background-position:{left,right,center}
```
##### 表格属性
```css
border-collapse: collapse; //双边框变单边框
```
##### border属性
```css
border: <number> <style>{solid,outset,dotted,dashed} <color>; //用于指定元素边框的宽度、样式和颜色，不需要写全且与顺序无关
border-radius: <number>;

```

#### 常见伪类

| 名称        | 触发条件      |
| --------- | --------- |
| `:hover`  | 鼠标悬停      |
| `:active` | 鼠标按下但还未释放 |
| `:focus`  | 获得焦点      |
#### 变形属性
```css

//属性名、持续时间、过渡函数、延迟时间
//剩余过渡属性顺序顺序不变时，可省略部分属性
//可用设置多组过渡，逗号分隔
transition:
	<property>{all,none,<name>} <time> <timing-function>{ease|linear|cubic-bezier(n,n,n,n)} <time>;

//单独设置每组属性
transition-property: width, height, background;
transition-duration: 0.5s, 0.5s, 1s;
transition-timing-function: ease-in, ease-in, linear;
transition-delay: 0s, 0s, 0.2s;

transform: <transform-function>
```
