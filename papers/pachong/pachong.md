# 爬虫
>网络爬虫能够自动请求网页，并将所需要的数据抓取下来。通过对抓取的数据进行处理，从而提取出有价值的信息。
---
### 爬虫分类
* 通用  ： 遵循robots协议（规定可抓取的东西）
* 聚焦：只抓取与需求相关的
* 增量式 ：增量式更新
---

为了限制爬虫带来的危险，大多数网站都有良好的反爬措施，并通过 robots.txt 协议做了进一步说明，下面是淘宝网 robots.txt 的内容：
~~~
User-agent: Baiduspider 
Disallow: /baidu Disallow: /s? 
Disallow: /ulink? 
Disallow: /link? 
Disallow: /home/news/data/ 
Disallow: /bh
.....
User-agent: * 
Disallow: /
~~~


---

### 编写爬虫的流程
---
* **先由 urllib 模块的 request 方法打开 URL 得到网页 HTML 对象。**
* **使用浏览器打开网页源代码分析网页结构以及元素节点。**
* **通过 Beautiful Soup 或则正则表达式提取数据。**
* **存储数据到本地磁盘或数据库。**


!>爬虫程序需要尽量伪装成人访问网站的样子，而非机器访问，否则就会被网站的反爬策略限制，甚至直接封杀 IP，相关知识会在后续内容介绍。

---
### 前端介绍
---
网页一般由三部分组成，分别是 HTML（超文本标记语言）、CSS（层叠样式表）和 JavaScript（简称“JS”动态脚本语言），它们三者在网页中分别承担着不同的任务。
* **HTML 负责定义网页的内容**
* **CSS 负责描述网页的布局**
* **JavaScript 负责网页的行为**

>如果用人体来比喻网站结构的话，那么 HTML 是人体的骨架，它定义了人的嘴巴、眼睛、耳朵长在什么位置；CSS 描述了人体的外观细节，比如嘴巴长什么样子，眼睛是双眼皮还是单眼，皮肤是黑色的还是白色的等；而 JavaScript 则表示人拥有的技能，例如唱歌、打球、游泳等。


---
### 静态网页和动态网页
---
**静态网页**

<span style="color: red;">静态网页</span>是标准的HTML文件，通过 GET 请求方法可以直接获取。静态并非静止不动，它也包含一些动画效果。当网站信息量较大的时，网页的生成速度会降低，由于静态网页的内容相对固定，且不需要连接后台数据库，因此<span  style="color: red;">响应速度非常快</span>。但静态网页更新比较麻烦，每次更新都需要重新加载整个网页。

---

**动态网页**

 AJAX（是指一种创建交互式、快速动态网页应用的网页开发技术）、ASP(是一种创建动态交互式网页并建立强大的 web 应用程序)、JSP(是 Java 语言创建动态网页的技术标准) 等技术，它不需要重新加载整个页面内容，就可以实现网页的局部更新。

 动态页面使用“动态页面技术”与服务器进行少量的数据交换，从而实现了网页的异步加载。

 <span style= "color:green;">抓取动态网页的过程较为复杂，需要通过动态抓包来获取客户端与服务器交互的 JSON 数据。抓包时，可以使用谷歌浏览器开发者模式（快捷键：F12）Network选项，然后点击 XHR，找到获取 JSON 数据的 URL。</span>


 ---
 ### 审查网页元素
 ---

#### 检查百度首页 

 下面以检查百度首页为例：首先使用 Chrome 浏览器打开百度，然后在百度首页的空白处点击鼠标右键（或者按快捷键：F12），在出现的会话框中点击“检查”，并进行如图所示操作：

![alt text](papers\pachong\image.png)

点击审查元素按钮，然后将鼠标移动至您想检查的位置，比如百度的输入框，然后单击，此时就会将该位置的代码段显示出来（如图 1 所示）。最后在该代码段处点击右键，在出现的会话框中选择 Copy 选项卡，并在二级会话框内选择“Copy element”，如下所示：


![alt text](papers\pachong\image-1.png)

百度输入框的代码如下所示：
~~~html
<input type="text" class="s_ipt" name="wd" id="kw" maxlength="100" autocomplete="off">
~~~

---
#### 编辑网页代码 

通过检查元素也可以更改网页代码，下面通过C语言中文网登录界面进行简单演示：
![alt text](papers\pachong\image-2.png)

检查密码框的 HTML 代码，代码如下所示：
~~~html
<input name="password" type="password" id="password" autocomplete="off">
~~~
只要在显示出的代码段上稍微做一下更改，密码就会变为可见状态。如下图所示：
![alt text](papers\pachong\image-3.png)

双击 type="password" 将输入框类型更改为 text，此类操作适用于所有网站的登录界面。但是需要注意，您做的更改仅限本次有效，当关闭网页后，会自动恢复为原来的状态。

---
#### 检查网页结构
对于爬虫而言，检查网页结构是最为关键的一步，需要对网页进行分析，并找出信息元素的相似性。下面以猫眼电影网为例，检查每部影片的 HTML 元素结构。如下所示：

![alt text](papers\pachong\image-4.png)

第一部影片的代码段如下所示：
~~~html
    <div class="board-item-main">
      <div class="board-item-content">
              <div class="movie-item-info">
        <p class="name"><a href="/films/1299372" title="你好，李焕英" data-act="boarditem-click" data-val="{movieId:1299372}">你好，李焕英</a></p>
        <p class="star">
                主演：贾玲,张小斐,沈腾
        </p>
<p class="releasetime">上映时间：2021-02-12</p>    </div>
    <div class="movie-item-number score-num">
<p class="score"><i class="integer">9.</i><i class="fraction">5</i></p>       
    </div>

      </div>
    </div>
~~~

接下来检查第二部影片的代码，如下所示：
~~~html
    <div class="board-item-main">
      <div class="board-item-content">
              <div class="movie-item-info">
        <p class="name"><a href="/films/553231" title="心灵奇旅" data-act="boarditem-click" data-val="{movieId:553231}">心灵奇旅</a></p>
        <p class="star">
                主演：杰米·福克斯,蒂娜·菲,菲利西亚·拉斯海德
        </p>
<p class="releasetime">上映时间：2020-12-25</p>    </div>
    <div class="movie-item-number score-num">
<p class="score"><i class="integer">9.</i><i class="fraction">3</i></p>       
    </div>

      </div>
    </div>
~~~
经过对比发现，除了每部影片的信息不同之外，它们的 HTML 结构是相同的，比如每部影片都使用<dd></dd>标签包裹起来。这里我们只检查了两部影片，在实际编写时，你可以多检查几部，从而确定它们的 HTML 结构是相同的。

---
### 准备工作
---
* **python语言**：Python 爬虫作为 Python 编程的进阶知识，要求学习者具备较好的 Python 编程基础。
* **Wed前端**：了解 Web 前端的基本知识，比如 HTML、CSS、JavaScript，这能够帮助你分析网页结构，提炼出有效信息。
* **HTTP协议** ：掌握 OSI 七层网络模型，了解 TCP/IP 协议、HTTP 协议，这些知识将帮助您了解网络请求（GET 请求、POST 请求）和网络传输的基本原理。


---
### 第一个Python爬虫程序
---

##### 获取响应对象 
向百度（http://www.baidu.com/）发起请求，获取百度首页的 HTML 信息，代码如下：
纯文本复制

~~~Python
#导包,发起请求使用urllib库的request请求模块
import urllib.request
# urlopen()向URL发请求,返回响应对象,注意url必须完整
response=urllib.request.urlopen('http://www.baidu.com/')
print(response)
~~~
上述代码会返回百度首页的响应对象， 其中 urlopen() 表示打开一个网页地址。注意：请求的 url 必须带有 http 或者 https 传输协议。

输出结果，如下所示：
~~~
<http.client.HTTPResponse object at 0x032F0F90>
~~~

####  输出HTML信息

~~~python
#提取响应内容
html = response.read().decode('utf-8')
#打印响应内容
print(html)
~~~

通过调用 response 响应对象的 read() 方法提取 HTML 信息，该方法返回的结果是字节串类型(bytes)，因此需要使用 decode() 转换为字符串。

### 常用方法

**1) urlopen()**
表示向网站发起请求并获取响应对象，如下所示：
~~~python
urllib.request.urlopen(url,timeout)
~~~

urlopen() 有两个参数，说明如下：
* url：表示要爬取数据的 url 地址。
* timeout：设置等待超时时间，指定时间内未得到响应则抛出超时异常。

---

**2) Request()**
该方法用于创建请求对象、包装请求头，比如重构 <span style= "color:green;">User-Agent（即用户代理，指用户使用的浏览器）使程序更像人类的请求，而非机器。</span>重构 User-Agent 是爬虫和反爬虫斗争的第一步。在下一节会做详细介绍。

~~~python
urllib.request.Request(url,headers)
~~~

参数说明如下：
* url：请求的URL地址。
* headers：重构请求头。

---
**3) html响应对象方法**
~~~python
bytes = response.read() # read()返回结果为 bytes 数据类型
string = response.read().decode() # decode()将字节串转换为 string 类型
url = response.geturl() # 返回响应对象的URL地址
code = response.getcode() # 返回请求时的HTTP响应码
~~~

---
**4) 编码解码操作**
~~~python
#字符串转换为字节码
string.encode("utf-8") 
#字节码转换为字符串
bytes.decode("utf-8") 

~~~


---
### User-Agent（用户代理）是什么
---
**User-Agent 即用户代理，简称“UA”**它是一个特殊字符串头。网站服务器通过识别 “UA”来确定用户所使用的操作系统版本、CPU 类型、浏览器版本等信息。而网站服务器则通过判断 UA 来给客户端发送不同的页面。

常见的 User-Agent 请求头，如下所示：
![alt text](papers\pachong\image-5.png)


---
### 爬虫程序UA信息
---
下面，通过向 HTTP 测试网站（http://httpbin.org/）发送 GET 请求来查看请求头信息，从而获取爬虫程序的 UA。代码如下所示：
~~~python
#导入模块
import urllib.request
#向网站发送get请求
response=urllib.request.urlopen('http://httpbin.org/get')
html = response.read().decode()
print(html)
~~~
程序运行后，输出的请求头信息如下所示：
~~~text
{
  "args": {},
  #请求头信息
  "headers": {
    "Accept-Encoding": "identity",
    "Host": "httpbin.org",
    "User-Agent": "Python-urllib/3.7", #UserAgent信息包含在请求头中！
    "X-Amzn-Trace-Id": "Root=1-6034954b-1cb061183308ae920668ec4c"
  },
  "origin": "121.17.25.194",
  "url": "http://httpbin.org/get"
}
~~~
从输出结果可以看出，User-Agent 竟然是 Python-urllib/3.7，这显然是爬虫程序访问网站。因此就需要重构 User-Agent，将其伪装成“浏览器”访问网站。

!>注意：httpbin.org 这个网站能测试 HTTP 请求和响应的各种信息，比如 cookie、IP、headers 和登录验证等，且支持 GET、POST 等多种方法，对 Web 开发和测试很有帮助。


---
### 构建User-Agnet代理池
---
>在编写爬虫程序时，一般都会构建一个 User-Agent （用户代理）池，就是把多个浏览器的 UA 信息放进列表中，然后再从中随机选择。构建用户代理池，能够避免总是使用一个 UA 来访问网站，因为短时间内总使用一个 UA 高频率访问的网站，可能会引起网站的警觉，从而封杀掉 IP。

#### 自定义UA代理池
构建代理池的方法也非常简单，在您的 Pycharm 工作目录中定义一个 **ua_info.py**文件，并将以下 UA 信息以列表的形式粘贴到该文件中，如下所示：

~~~python
ua_list = [
    'Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Maxthon 2.0',
    'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_0) AppleWebKit/535.11 (KHTML, like Gecko) Chrome/17.0.963.56 Safari/535.11',
    'User-Agent:Opera/9.80 (Windows NT 6.1; U; en) Presto/2.8.131 Version/11.11',
    'Mozilla/5.0 (Windows NT 6.1; rv:2.0.1) Gecko/20100101 Firefox/4.0.1',
    'Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.0)',
    'Mozilla/5.0 (Windows; U; Windows NT 6.1; en-us) AppleWebKit/534.50 (KHTML, like Gecko) Version/5.1 Safari/534.50',
    'Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0',
    ' Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1',
    'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1',
    ' Mozilla/5.0 (Macintosh; Intel Mac OS X 10.6; rv:2.0.1) Gecko/20100101 Firefox/4.0.1',
]
~~~
经过上述操作，用户代理池就构建成功。


---
#### 模块随机获取UA

您也可以使用专门第三方的模块来随机获取浏览器 UA 信息，不过该模块需要单独安装，安装方式如下：

~~~shell
pip install fake-useragent
~~~

下载安装成功后，演示如下代码：
~~~python
from fake_useragent import UserAgent
#实例化一个对象
ua=UserAgent()
#随机获取一个ie浏览器ua
print(ua.ie)
print(ua.ie)
#随机获取一个火狐浏览器ua
print(ua.firefox)
print(ua.firefox)
~~~

!>试过不行


---
### URL编码/解码详解
---
当 URL 路径或者查询参数中，带有中文或者特殊字符的时候，就需要对 URL 进行编码（采用十六进制编码格式）。URL 编码的原则是使用安全字符去表示那些不安全的字符。
>安全字符，指的是没有特殊用途或者特殊意义的字符。

#### URL基本组成
URL 是由一些简单的组件构成，比如协议、域名、端口号、路径和查询字符串等，示例如下：

~~~url
http://www.biancheng.net/index?param=10
~~~
路径和查询字符串之间使用问号?隔开。上述示例的域名为 www.biancheng.net，路径为 index，查询字符串为 param=1。


**URL 中规定了一些具有特殊意义的字符，常被用来分隔两个不同的 URL 组件，这些字符被称为保留字符。**例如：
* 冒号：用于分隔协议和主机组件，斜杠用于分隔主机和路径
* ?：用于分隔路径和查询参数等。
* =用于表示查询参数中的键值对。
* &符号用于分隔查询多个键值对。

---
#### 哪些字符需要编码
URL 之所以需要编码，是因为 URL 中的某些字符会引起歧义，比如 URL 查询参数中包含了”&”或者”%”就会造成服务器解析错误；再比如，URL 的编码格式采用的是 ASCII 码而非 Unicode 格式，这表明 URL 中不允许包含任何非 ASCII 字符（比如中文），否则就会造成 URL 解析错误。

URL 编码协议规定（RFC3986 协议）：URL 中只允许使用 ASCII 字符集可以显示的字符，比如英文字母、数字、和- _ . ~ ! *这 6 个特殊字符。当在 URL 中使用不属于 ASCII 字符集的字符时，就要使用特殊的符号对该字符进行编码，比如空格需要用%20来表示。

除了无法显示的字符需要编码外，还需要对 URL 中的部分保留字符和不安全字符进行编码。下面列举了部分不安全字符：
~~~
[ ] < > " "  { } | \ ^ * · ‘ ’ 等
~~~

![alt text](papers\pachong\image-6.png)


---
### Python实现编码与解码
---
Python 的标准库**urllib.parse模块**中提供了用来编码和解码的方法，分别是 urlencode() 与 unquote() 方法。

| 方法 | 说明  
| ------ | ------ | 
| urlencode() | 该方法实现了对 url 地址的编码操作|  
| unquote()  | 该方法将编码后的 url 地址进行还原，被称为解码 |  

####  编码urlencode()
下面以百度搜索为例进行讲解。首先打开百度首页，在搜索框中输入“爬虫”，然后点击“百度一下”。当搜索结果显示后，此时地址栏的 URL 信息，如下所示：

~~~text
https://www.baidu.com/s?wd=爬虫&rsv_spt=1&rsv_iqid=0xa3ca348c0001a2ab&issp=1&f=8&rsv_bp=1&rsv_idx=2&ie=utf-8&tn=baiduhome_pg&rsv_enter=1&rsv_dl=ib&rsv_sug3=8&rsv_sug1=7&rsv_sug7=101
~~~

可以看出 URL 中有很多的查询字符串，而第一个查询字符串就是“wd=爬虫”，其中 wd 表示查询字符串的键，而“爬虫”则代表您输入的值。

在网页地址栏中删除多余的查询字符串，最后显示的 URL 如下所示：

~~~text
https://www.baidu.com/s?wd=爬虫
~~~

使用搜索修改后的 URL 进行搜索，依然会得到相同页面。因此可知“wd”参数是百度搜索的关键查询参数。下面编写爬虫程序对 “wd=爬虫”进行编码，如下所示：


~~~python
#导入parse模块
from urllib import parse
#构建查询字符串字典
query_string = {
'wd' : '爬虫'
}
#调用parse模块的urlencode()进行编码
result = parse.urlencode(query_string)
#使用format函数格式化字符串，拼接url地址
url = 'http://www.baidu.com/s?{}'.format(result)
print(url)
~~~

输出结果，如下所示：
~~~
wd=%E7%88%AC%E8%99%AB
http://www.baidu.com/s?wd=%E7%88%AC%E8%99%AB
~~~
编码后的 URL 地址依然可以通过地网页址栏实现搜索功能。

除了使用 urlencode() 方法之外，也可以使用 quote(string) 方法实现编码，代码如下：

~~~python
from urllib import parse
#注意url的书写格式，和 urlencode存在不同
url = 'http://www.baidu.com/s?wd={}'
word = input('请输入要搜索的内容:')
#quote()只能对字符串进行编码
query_string = parse.quote(word)
print(url.format(query_string))
~~~

输出结果如下：

~~~
输入：请输入要搜索的内容:编程帮www.biancheng.net
输出：http://www.baidu.com/s?wd=%E7%BC%96%E7%A8%8B%E5%B8%AEwww.biancheng.net
~~~
注意：quote() 只能对字符串编码，而 urlencode() 可以直接对查询字符串字典进行编码。因此在定义 URL 时，需要注意两者之间的差异。方法如下：
~~~python
# urllib.parse
urllib.parse.urlencode({'key':'value'}) #字典
urllib.parse.quote(string) #字符串
~~~

---
#### 解码unquote(string)
解码是对编码后的 URL 进行还原的一种操作，示例代码如下：

~~~python
from urllib import parse
string = '%E7%88%AC%E8%99%AB'
result = parse.unquote(string)
print(result)
~~~

输出结果：
~~~text
爬虫
~~~

---
#### URL地址拼接方式

最后，给大家介绍三种拼接 URL 地址的方法。除了使用 format() 函数外，还可以使用字符串相加，以及字符串占位符，总结如下：
~~~python
# 1、字符串相加
  baseurl = 'http://www.baidu.com/s?'
  params='wd=%E7%88%AC%E8%99%AB'
  url = baseurl + params
# 2、字符串格式化（占位符）
  params='wd=%E7%88%AC%E8%99%AB'
  url = 'http://www.baidu.com/s?%s'% params
# 3、format()方法
  url = 'http://www.baidu.com/s?{}'
  params='wd=%E7%88%AC%E8%99%AB'
  url = url.format(params)
~~~


---
### Python爬虫抓取网页
---
首先我们对要编写的爬虫程序进行简单地分析，该程序可分为以下三个部分：
* 拼接 url 地址
* 发送请求
* 将照片保存至本地

---
#### 导入所需模块
本节内容使用 urllib 库来编写爬虫，下面导入程序所用模块：
~~~python
from urllib import request
from urllib import parse
~~~

---
#### 拼接URL地址
定义 URL 变量，拼接 url 地址。代码如下所示：
~~~python
url = 'http://www.baidu.com/s?wd={}'
#想要搜索的内容
word = input('请输入搜索内容:')
params = parse.quote(word)
full_url = url.format(params)
~~~

---
#### 向URL发送请求
发送请求主要分为以下几个步骤：
* 创建请求对象-Request
* 获取响应对象-urlopen
* 获取响应内容-read
代码如下所示：

~~~python
#重构请求头
headers = {'User-Agent':'Mozilla/5.0 (Windows NT 6.1; WOW64; rv:6.0) Gecko/20100101 Firefox/6.0'}
#创建请求对应
req = request.Request(url=full_url,headers=headers)
#获取响应对象
res = request.urlopen(req)
#获取响应内容
html = res.read().decode("utf-8")
~~~

---
#### 保存为本地文件
把爬取的照片保存至本地，此处需要使用 Python 编程的文件 IO 操作，代码如下：
~~~python
filename = word + '.html'
with open(filename,'w', encoding='utf-8') as f:
    f.write(html)
~~~

---
### 正则表达式基本语法
---
正则表达式(regular expression)是一种字符串匹配模式或者规则，它可以用来检索、替换那些符合特定规则的文本。

![alt text](papers\pachong\image-7.png)


---
### Python re模块用法详解

[re模块用法详解](https://c.biancheng.net/python_spider/re-module.html)


---
### Python csv模块（读写文件）

CSV 文件又称为逗号分隔值文件，是一种通用的、相对简单的文件格式，用以存储表格数据，包括数字或者字符。CSV 是电子表格和数据库中最常见的输入、输出文件格式，可参考《CSV介绍》。

通过爬虫将数据抓取的下来，然后把数据保存在文件，或者数据库中，这个过程称为数据的持久化存储。本节介绍 Python 内置模块 CSV 的读写操作。


#### CSV文件写入

~~~ python
writer(csvfile, dialect='excel', **fmtparams)
~~~

参数说明：
* csvfile：必须是支持迭代(Iterator)的对象，可以是文件(file)对象或者列表(list)对象。
* dialect：编码风格，默认为 excel 的风格，也就是使用逗号,分隔。
* fmtparam：格式化参数，用来覆盖之前 dialect 对象指定的编码风格。



---
### Python爬虫抓取猫眼电影排行榜
---
>本节使用 Python 爬虫抓取猫眼电影网 TOP100 排行榜（https://maoyan.com/board/4）影片信息，包括电影名称、上映时间、主演信息。

#### 确定页面类型

点击右键查看页面源码，确定要抓取的数据是否存在于页面内。通过浏览得知要抓取的信息全部存在于源码内，因此该页面输属于静态页面。如下所示：

~~~html
<div class="movie-item-info">
        <p class="name"><a href="/films/1200486" title="我不是药神" data-act="boarditem-click" data-val="{movieId:1200486}">我不是药神</a></p>
        <p class="star">
                主演：徐峥,周一围,王传君
        </p>
<p class="releasetime">上映时间：2018-07-05</p>    </div>
~~~

---
#### 确定url规律
想要确定 url 规律，需要您多浏览几个页面，然后才可以总结出 url 规律，如下所示：

~~~
第一页：https://maoyan.com/board/4?offset=0
第二页：https://maoyan.com/board/4?offset=10
第三页：https://maoyan.com/board/4?offset=20
...
第n页：https://maoyan.com/board/4?offset=(n-1)*10
~~~

---
#### 确定正则表达式
通过分析网页元素结构来确定正则表达式，如下所示：
~~~ html
<div class="movie-item-info">
        <p class="name"><a href="/films/1200486" title="我不是药神" data-act="boarditem-click" data-val="{movieId:1200486}">我不是药神</a></p>
        <p class="star">
                主演：徐峥,周一围,王传君
        </p>
<p class="releasetime">上映时间：2018-07-05</p></div>
~~~

使用 Chrome 开发者调试工具来精准定位要抓取信息的元素结构。之所以这样做，是因为这能避免正则表达式的冗余，提高编写正则表达式的速度。正则表达式如下所示：
~~~html
<div class="movie-item-info">.*?title="(.*?)".*?class="star">(.*?)</p>.*?releasetime">(.*?)</p>
~~~
!>编写正则表达式时将需要提取的信息使用(.*?)代替，而不需要的内容（包括元素标签）使用.*?代替。


---
#### 编写程序
~~~ python
from urllib import request
import re
import time
import random
import csv
from ua_info import ua_list

# 定义一个爬虫类
class MaoyanSpider(object): 
    # 初始化
    # 定义初始页面url
    def __init__(self):
        self.url = 'https://maoyan.com/board/4?offset={}'
    
    # 请求函数
    def get_html(self,url):
        headers = {'User-Agent':random.choice(ua_list)}
        req = request.Request(url=url,headers=headers)
        res = request.urlopen(req)
        html = res.read().decode()
        # 直接调用解析函数
        self.parse_html(html)
    
    # 解析函数
    def parse_html(self,html):
        # 正则表达式
        re_bds = '<div class="movie-item-info">.*?title="(.*?)".*?<p class="star">(.*?)</p>.*?class="releasetime">(.*?)</p>'
        # 生成正则表达式对象
        pattern = re.compile(re_bds,re.S)
        # r_list: [('我不是药神','徐峥,周一围,王传君','2018-07-05'),...] 列表元组
        r_list = pattern.findall(html)
        self.save_html(r_list)

    # 保存数据函数，使用python内置csv模块
    def save_html(self,r_list):
        #生成文件对象  
        with open('maoyan.csv','a',newline='',encoding="utf-8") as f:
            #生成csv操作对象
            writer = csv.writer(f)
            #整理数据
            for r in r_list:
                name = r[0].strip()
                star = r[1].strip()[3:]
                # 上映时间：2018-07-05
                # 切片截取时间
                time = r[2].strip()[5:15]
                L = [name,star,time]
                # 写入csv文件
                writer.writerow(L)
                print(name,time,star)

    # 主函数
    def run(self):
        #抓取第一页数据
        for offset in range(0,11,10):
            url = self.url.format(offset)
            self.get_html(url)
            #生成1-2之间的浮点数
            time.sleep(random.uniform(1,2))

# 以脚本方式启动
if __name__ == '__main__':
    #捕捉异常错误
    try:
        spider = MaoyanSpider()
        spider.run()
    except Exception as e:
        print("错误:",e)
~~~