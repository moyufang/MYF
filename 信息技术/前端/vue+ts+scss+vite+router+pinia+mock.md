### Vue 环境配置

##### 开发环境

1. npm + vscode

利用 npm 创建一个轻量级、热更新的 vue 开发环境，用 vscode 来编写代码。

创建项目
```cmd
npm create vue@latest my-ui -- --typescript --router --pinia
cd my-ui
npm install sass axios mockjs
npm install -D @types/mockjs
```
运行项目
```cmd
my_vue_project>npm run dev
```
项目结构
```
src/
├── components/           # 通用组件
│   ├── base/            # 基础UI组件
│   ├── layout/          # 布局组件
│   └── modules/         # 模块专用组件
├── views/               # 页面级组件（各模块）
├── stores/              # Pinia状态管理
├── services/            # API服务层
│   ├── api/             # 真实后端API
│   └── mock/            # Mock服务
├── types/               # TypeScript类型定义
├── utils/               # 工具函数
└── assets/              # 静态资源
```
##### 项目部署

详见 [生产环境部署指南](https://cn.vuejs.org/guide/best-practices/production-deployment.html)
```cmd
my_vue_project>npm run build
```

### 后端模拟工具(mock)
### 响应式

### Vue 组件

Vue 的组件可以按两种不同的风格书写：**选项式 API** 和**组合式 API**。

- `definePros` 可以从父组件中向子组件传递参数（深拷贝）。
- `defineExpos` 可以将子组件内部的对象、函数暴露给父组件。

### VuePress

VuePress 是一个以 Markdown 为中心的静态网站生成器。你可以使用 [Markdown](https://zh.wikipedia.org/wiki/Markdown) 来书写内容 (如文档、博客等) ，然后 VuePress 会帮助你生成一个静态网站来展示它们。

一个 VuePress 相当于 Vue + VueRouter，Vue 美化网站，VueRouter 生产页面路由（依据 md 文件集的相对路径），

##### VuePress 内置 Markdown 扩展

在 md 中使用相对路径访问其它 md 文件，渲染成 html 后路由会自动生成。

