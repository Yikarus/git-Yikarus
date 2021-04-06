# CSS记录

配合HTML

CSS包括选择器和样式

在head中用style包裹

\<head>
    <style>
    </style>
</head>

规范，如下展开式风格，实际使用再打包。

h3 {<!-- 选择器后加空格-->

​	color: red;<!-- 冒号后加空格-->

}

## 选择器

分为基础选择器和符合选择器。

基础选择器：

标签选择器，类选择器，id选择器，通配符选择器。

### 标签选择器

标签名作为选择器。

### 类选择器

单独选择一个或某几个选择标签。

.类名 {<!-- 类名不能为标签名-->

​	属性: 属性值;

}

需要用class属性，比如<div class="what">。

.what {;}

#### 多类名

一个标签多个类名，class中以空格分开。

\<div class="red green">

### id选择器

#id名 {;}

\<div id="pink">

id只有一个。

### 通配符选择器

*选取所有的元素。

\* {;}

## CSS字体属性

### font-family

字体系列

h2 {

​	font-family: '宋体',Arial;<!-- SimSum 英文更兼容-->

}

多个字体用,分割，代表是没有前面的就用后面的。

### font-size

文字大小

body {

​	font-size: 20px;<!--必须加单位-->

}

标题标签需要单独设定。

### font-weight

字体粗细：normal bold bolder lighter 数字

和之前的strong一样。

.bold {

​	font-weight: 700;<!--提倡数字表示，700是加粗-->

}

### font-style

文字样式: normal italic

​	p {

​		font-style: normal;

}

### 字体复合属性

把以上属性综合写

​	body { 

​		<!--font:font-style font-weight font-size/line-height font-family;-->

​	font:normal normal 16px/16px SimSum;

}

严格遵守规范。必须保留font-size和font-family。

### 文本属性

文本属性定义文本的外观，文本的颜色、对齐文本、装饰文本、文本缩进、行间距等。

#### 文本颜色

​	div {

​		color: red;<!-- #ff9999 rgb(244,9,9)-->

}

#### 文本对齐

text-align，只能设置水平对齐。

​	div	{

​		text-align: left;<!-- right center;-->	

}

让div内的文字对齐。

#### 装饰文本

text-decoration可以加下划线，删除线，上划线。

取值：none underline overline line-through

#### 文本缩进

text-indent指定第一行的缩进。

p {

​	text-indent: 20px;

}

em相对单位，想对与当前元素文字大小。

p {

​	text-indent: 2em;

}

#### 行间距

line-height

p {

​	line-height: 26px;

}