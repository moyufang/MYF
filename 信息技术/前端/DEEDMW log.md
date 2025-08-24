代码原则
- 拒绝微调 css，专注核心功能实现
- 全局 css 文件，任何样式调用全局 css 文件：避免 css 屎山
- 函数能用就行，不做任何关于语法糖的优化：语法糖复杂化函数、降低可维护性
- 阶段一不做任何优化用户体验的动画，能用就行
- 动态维护配色方案，新增配色在全局 css 中注册
顶层项目结构 vue + vue-router + typescript + scss
```
src/
├── components/          # 可复用组件
│   ├── ui/             # 基础UI组件
│   └── common/         # 通用组件
├── composable/         # 组合式函数
├── views/              # 路由页面组件
├── router/             # 路由配置
├── types/              # TypeScript类型定义
├── assets/             # 静态资源
│   └── styles/         # 样式文件
│       ├── _variables.scss   # SCSS变量
│       ├── _mixins.scss      # SCSS混合
│       ├── _base.scss        # 基础样式
│       ├── _utilities.scss   # 工具类
│       └── main.scss         # 主样式文件
├── utils/              # 工具函数
└── plugins/            # 插件
```

# 需求文档 & To do list

##### Home Page
- [ ] 卡片式
- [x] blog 式
- [x] Latex 渲染
- [ ] loading 界面
- [ ] 切换 page 时淡出淡入动画
- [ ] 字体美观
##### Logic

文本公式编辑器
- [x] 词法分析 + 表达式树构建
- [x] 错误检测 + 错误定位
- [ ] 错误 UI
	- [ ] 错误位置高亮
	- [ ] 错误提示

0阶逻辑
- [ ] 真值表
- [ ] 主析式、主合式（ latex 字符串）
- [ ] 归结推理（latex 字符串，分有无目标两种模式）
- [ ] 图形化公式编辑器（0阶、1阶）
1阶逻辑
- [ ] 全称左提
- [ ] 归结推理
##### Graph
- [ ] 框架重写

# Tips

### SVG Icon (colorful icon)

设置 div 的背景为纯色，然后用 svg 图片做蒙版，这样图标的颜色就是背景颜色了

```scss
.notice-icon{
    height: $icon-size;
    width: $icon-size;
    margin: $icon-margin;
    
    background-color: $color;

	//创建蒙版
    -webkit-mask-image: url($icon-src);
    -webkit-mask-size: contain;
    -webkit-mask-repeat: no-repeat;
    mask-image: url($icon-src);
    mask-size: contain;
    mask-repeat: no-repeat;
  }
```

