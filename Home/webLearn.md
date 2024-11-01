<center><h1>前端</h1></center>

> 以搭建个人网站为目的，学习前端三件套的知识。

## 目录

### 创建 Html 框架

`!` 可以快速创建 html 框架

```html
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Capser's Web</title>
  </head>
  <body></body>
</html>
```

### 链接 CSS 文件和 JS 文件

```html
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Capser's Web</title>
  <!-- 引入CSS文件 -->
  <link rel="stylesheet" href="Secondstyles.css" />
</head>

<body>
    <!-- 引入JS文件 -->
    <script src="SecondScript.js"></script>
</body>
</html>
```

### 导航栏
导航栏`<header>`部分。具体来说，它定义了一个无序列表（`<ul>`），其中包含多个列表项（`<li>`），每个列表项内有一个超链接（`<a>`）。这些超链接指向页面中的不同部分或页面。


```css
/*
    顶部导航栏样式
*/
/*
    顶部导航栏样式
*/
header {
    /* 黑色背景 */
    background-color: #27282c;
    /* 白色文字 */
    color: #fff;
    /* fixed定位 */
    position: fixed;
    /* 顶部0px */
    top: 0;
    /* 100%宽度 */
    width: 100%;
    height: 50px;
    /* z-index为1000，确保在最上层 */
    z-index: 1000;
    /* 使用 flexbox 布局 */
    display: flex;
    /* 垂直居中对齐 */
    align-items: center;
    /* 从左对齐 */
    justify-content: space-between;
    /* 内边距 */
    padding: 0 20px;
}

/*
    无序列表ul的样式
*/
header ul {
    list-style-type: none; /* 移除默认的列表样式 */
    margin: 0; /* 移除默认的外边距 */
    padding: 0; /* 移除默认的内边距 */
    display: flex; /* 使用 flexbox 布局 */
    align-items: center; /* 垂直居中对齐 */
    width: 100%; /* 占据整个宽度 */
}

/* 图标项 */
header ul .logo {
    display: flex; /* 使用 flexbox 布局 */
    align-items: center; /* 垂直居中对齐 */
    margin-right: 20px; /* 图标和文本之间的间距 */
}

/* 图片 */
header ul .logo img {
    width: 50px; /* 设置图片宽度 */
    height: 50px; /* 设置图片高度 */
    margin-right: 10px; /* 图片和文本之间的间距 */
}

/* 文本 */
header ul .logo p {
    margin: 30px; /* 移除默认的外边距 */
    font-size: 30px; /* 字体大小 */
}

/* 文本链接项 */
header ul .nav-links {
    display: flex; /* 使用 flexbox 布局 */
    justify-content: center; /* 文本居中对齐 */
    flex-grow: 1; /* 占据剩余空间 */
    margin-right: 20%;
}

/* 列表项的链接a的样式 */
header ul .nav-links a {
    /* 白色文字 */
    color: #fff;
    /* 去除下划线 */
    text-decoration: none;
    /* 10px上下内边距，20px左右内边距 */
    padding: 10px 60px;
    /* 块状元素 */
    display: block;
}

/* 列表项的链接a的hover样式 */
header ul .nav-links a:hover {
    /* 555灰色背景 */
    background-color: #555;
}

/* 图片 */
header ul .logo img {
    width: 50px; /* 设置图片宽度 */
    height: 50px; /* 设置图片高度 */
}
```