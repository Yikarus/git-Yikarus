# HTML记录

## <!DOCTYPE html>

表示当前文档版本是html5，必须写在文档最前，是文档类型声明标签。

## <html lang="zh-CN">

当前文档显示语言。

## <meta charset="UTF-8" />

中文：GB2312、BIG5、GBK。UTF-8为万国码。不写可能乱码。

## <h1>-<h6>

标题标签，大小依次变小。和div一样独占一行。

## <p></p>

将文字变成段落，浏览器会自动换行。

## <br />

换行标签，强制换行，<br />是单标签，且在同一个段落，不会改变段间距。

## 文本格式化标签

<strong>粗体</strong>：<strong></strong>或者<b></b>

<em>斜体</em>：<em></em>或者<i></i>

<del>删除线</del>：<del></del>或者<s></s>

<ins>下划线</ins>：<ins></ins>或者<u></u>

## <div>和<span>

没有语义，是一个盒子，可以把东西放到盒子里，可以用来布局。

\<div>是分区，一个方块，一行一个div，一人独占一行

<span>是跨距，代表横向，小块，一行上可以和很多

## 图像标签

\<img src="图像URL" />

是单标签，src是必须属性，指定图形路径文件名。

图像属性：

| src    | 图片路径 | 必须属性                 |
| ------ | -------- | ------------------------ |
| alt    | 文本     | 替换文本，不能显示图片时 |
| title  | 文本     | 鼠标放在图像上的文字     |
| width  | 像素     | 图像宽度                 |
| height | 像素     | 图像高度                 |
| border | 像素     | 图像边框，一般通过css    |

## <a>

从当前页面可以跳转，

<a href="跳转目标，必须有"  target="目标窗口弹出"></a>

target:_self _blank新建

连接分为外部链接和内部链接。

- 外部链接正常url

- 内部链接，网站内部网页，直接写相对路径加名字。

- 空连接 href="#"

- 下载链接 若是文件，直接下载。

- 锚点链接 跳转到当前页面的指定地址，设置属性为#名字，<a href="#two">需要给目标位置标签设置id<h3 id="two">

网页元素链接 图片文本都可以加超链接。

## 注释标签

<!--注释语言 -->

空格  &nbsp ;

&lt;		&lt ;

&gt;		&gt ;

&amp;		&amp ;

&yen;		&yen ;

&copy;		&copy ;版权

&reg;		&reg ;注册商标

&deg;		&deg ;

&plusmn;		&plusmn ;

&times;		&times ;

&divide;		&divide ;

&sup2;		&sup2 ;

&sup3;		&sup3 ;

## 表格

/<table>表格
    /<tr>
    /<th>表头单元格，加粗，居中显示
    </th>
        </tr>
    /<tr>单元行
    /<td>单元格
    </td>
    </tr>
    
</table>

表格属性，不常用，通过css设置。写到table里

| align       | center,left,right | 对其                              |
| ----------- | ----------------- | --------------------------------- |
| border      | 1或""             | 是否有边框                        |
| cellpadding | 像素值            | 单元格边缘和内容之间的空白，默认1 |
| cellspacing | 像素值            | 单元格之间空白，默认2             |
| width       | 像素或百分比      | 宽度                              |
| height      | 像素              | 高度                              |

表格结构标签 <thead>为头部区域，必须有<tr>，<tbody>为主体区域。

### 合并单元格

方式：

跨行 rowspan="合并行数" 行之间合并，上下合并

跨列 colspan="合并列数"

写到起始单元格上。

## 列表标签

用来布局。

分为无序列表，有序列表，自定义列表。

无序列表 <ul>，列表项<li>，<ul>内只放<li>，<li>内可以放其他。后面可以用css去掉圆点。

<ul>
    <li>
   	你喜欢的女人
    </li>
    <li>
        河南人
    </li>
</ul>

有序列表<ol> <li>

<ol>
  <li>第一名</li>
    <li>第二名</li>
</ol>

自定义列表

常用于对术语进行解释，表格<dl>，头项<dt>，孩子<dd>，一个<dt>好几个<dd>

<dl>
    <dt>小米</dt>
    <dd>手机</dd>
    <dd>电脑</dd>
   	<dd>汽车</dd>
</dl>

## 表单

表单为了收集信息。

表单由表单域，表单空间，和提示信息构成。

\<form>可以将表单域内的结果提交给服务器。

\<form action="http://www.baidu.com//" method="GET" name="name1">

</form>

| action | url地址  | 指定接受并处理表单数据的服务器程序的url地址 |
| ------ | -------- | ------------------------------------------- |
| method | get/post | 指定提交表单方式                            |
| name   | 名称     | 指定表单名称，以区分                        |

表单控件。

**input输入** <input type="属性值" />

属性值 button cleckbox file hidden image password radio reset submit text

name指定控件名字，指定同一个名字可以制作单选，复选框。

value定义input的值

checked 选择框的状态，单选框以最后一个为准

maxlength 指定输入字符最大长度

label可以绑定表单元素，增加选定范围<label for="name">

**select下拉**

\<select><option selected="selected">选项1</option>

<option>选项2</option>

</select>

**textarea文本域**

大量多行表单元素

\<textarea cols="一行几个" rows="可以几行">中间是默认文本</textarea>

实际使用css控制文本域大小

**实例**

<form action="http://www.baidu.com//" method="GET" name="name1">
  用户名：<input type="text" name="name" value="伊卡洛斯"><br/>  
  密码：<input type="password" name="password"><br/>
    <!--单选 用name和value指定同一个radio-->
    性别:<label for="sex">男</label><input type="radio" name="sex" checked="checked"> 女<input type="radio" name="sex" > 中<input type="radio" name="sex"><br/>
    <input type="submit" value="注册"><br/>
    <input type="reset" value="重写"><br/>
    <!--搭配按钮和js-->
     <input type="button" value="发送"><br/>
     <input type="file">
    <select><option>选项1</option><option selected="selected">选项2</option></select><textarea cols="3">这是默认</textarea></form>



