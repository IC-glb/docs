# 数模比赛的学习笔记
>记录打数模的学习

## [赛前准备](papers\DigitalAnalog\赛前准备.md)
### **提交注意事项**
![alt text](papers\DigitalAnalog\images\Time.png)



---
### **软件**

- 1、Matlab

- 2、Mathpix:截图即可获取公式编辑？？还付费？
 
- 3、AxGlyph：学术绘图（付费）66

- 我感觉写公式可以使用[latexlive](https://www.latexlive.com/)



---
### **赛题选择**

-  A题偏物理工程，答案很标准 **本科生不适合**

-  B题刚改革、题型不定。

-  **C题偏向经营/运筹/统一/数据分析类（最好）**


!> 最好开赛**6小时定题**：先查文献


---
### **搜索AND文献AND数据**

*搜索技巧*
![alt text](papers\DigitalAnalog\images\Search.png)


*文献*
- 先看知网的硕博士论文
- 高级检索 
- 其他： [谷歌学术镜像](http://scholar.hedasudi.com/)、[Open Access Library](https://www.oalib.com/)  

!>没必要看国外文献


*查数据*
- 优先在知网、谷歌学术搜索
- [**国家统计局**](https://data.stats.gov.cn/)
- kaggle


---
## [数据预处理](papers\DigitalAnalog\数据预处理.md)
!>官方提供的数据可能有问题、缺失值，不能直接用

---
### **缺失值**
- 有些单元格为null
- **缺失太多直接删除该项指标**
- 简单处理：均值、众数插补

> 定量：身高、年龄(均值填补)

> 定性：性别、文化程度(最多的值填补)

!>上述只适合对个体精度要求不大的数据

*解决方法*
- Newton插值法：构造近似函数，补上缺失值。但区间边缘会出现不稳定震荡。**不适合对导数有要求的题目**
适用：热力学温度、地形测量、定位等只追求函数值精确而不关心变化的数据

- 样条插值法：用分段光滑的函数曲线去插值，适用：**零件加工、水库水流量、图像基线漂移，机器人轨迹等精度要求高、没有突变的数据**


---
### **异常值**
- 有很明显的差异
- 正太分布原则
![alt text](papers\DigitalAnalog\images\异常值.png)
- 画箱型图
![alt text](papers\DigitalAnalog\images\箱型图.png)



---
## [建模](papers\DigitalAnalog\建模.md)
---
### *什么是模型*
![alt text](papers\DigitalAnalog\images\什么是模型.png)

---
### *建模过程*
- 一篇完整的数模论文（**摘要 {最重要}、问题重述、模型假设、符号说明、模型建立（求解[最长]、模型的优缺点与改进方法、参考文献和附录**）

>**摘要**包含问题、通了什么方法、求得什么结果、每一部分的大致步骤。

- 问题重述：简述

- **模型的建立**：一组公式和对公式的每一个变量的解释，就是一个模型

>1、先查阅资料
2、简单的模型  ->   修改模型。
3、把题目的变量带入模型中去。


- **模型求解**：查阅资料-->常用算法
例如：
![alt text](papers\DigitalAnalog\images\代码解决的过程.png)

- 模型优缺点与改进方法：不是必须。前提是模型建立好、能改进就改进。**分析正文模型常用在解决在什么哪种问题，又和本题有什么区别**

- 参考文献：一定规范！知网复制

- 附录：不要把网上搜到的代码直接复制粘贴，即使复制也要换变量名。



## [线性规划](papers\DigitalAnalog\线性规划.md)



## [2020年国赛C题](papers\DigitalAnalog\20C.md)
### 数据预处理
---
**1、读取Excel**
>xlsread 函数可以用来读取 Excel 文件，但如果你有较新的 MATLAB 版本，建议使用 readtable 或 readmatrix 函数，这些函数提供了更好的灵活性和功能。
~~~Matlab
% 读取 Excel 文件中的“进项发票信息”工作表
filename = 'data.xlsx';
sheetName = '进项发票信息';

% 使用 readmatrix 函数读取数据
data = readmatrix(filename, 'Sheet', sheetName, 'Range', 'A2:F210948');
~~~
>**filename:含义**：指定要读取的 Excel 文件的文件名。它可以是完整的路径或相对路径。示例：'data.xlsx'，即 Excel 文件名为 data.xlsx。'Sheet', **sheetName:含义：**指定要读取的工作表名称。sheetName 是一个字符串，表示工作表的名称。示例：'进项发票信息'，即工作表的名称是 '进项发票信息'。   **'Range', 'A2:F210948':含义：**指定要读取的数据范围。'A2:F210948' 是一个字符串，表示从 Excel 文件的 A2 单元格到 F210948 单元格的数据范围。解释：'A2'：数据区域的起始单元格，表示从第 2 行第 A 列开始读取数据。'F210948'：数据区域的结束单元格，表示读取到第 210948 行第 F 列的数据。readmatrix 函数的作用readmatrix 函数用于将指定的 Excel 文件中的数据读取到一个 MATLAB 矩阵中。这个矩阵中的每个元素对应于 Excel 文件中指定范围内的一个单元格的值。
---

**2、绘制箱型图**
~~~matlab
figure;
boxplot(data(:, 3:5), 'Labels', {'列3', '列4', '列5'}); 
title('箱型图');
xlabel('样本属性名称');
ylabel('样本数值大小');
~~~

读取数据：
readmatrix 函数读取 Excel 中的数据，并指定工作表和数据范围。
绘制箱型图：
boxplot 函数绘制第 3 列到第 5 列的数据的箱型图，并用 'Labels' 参数为每列指定标签。
Labels 参数用来为箱型图中的每一列指定标签，你可以根据实际列的意义替换 '列3', '列4', 和 '列5'。


>在 Excel 中，**列 E 到 G 实际上对应于第 5 列到第 7 列。**下面是列的对应关系：
>列 E 对应于第 5 列。
>列 F 对应于第 6 列。
>列 G 对应于第 7 列。
>因此，如果你在 MATLAB 中读取数据，并且希望选择列 E 到 G 的数据，那么你需要选择第 5 列到第 7 列的数据。

  
**示例**

<img src="papers\DigitalAnalog\images\箱型图代码示例.png" alt="logo" title="logo" style="width: 60%; margin: auto; display: block;" />

~~~Matlab
% 读取 Excel 文件中的“进项发票信息”工作表
filename = 'data.xlsx';
sheetName = '进项发票信息';

% 使用 readmatrix 函数读取数据
dataTable = readmatrix(filename, 'Sheet', sheetName, 'Range', 'A2:H210948');

% 查看 dataTable 的尺寸
disp(size(dataTable));

% 绘制箱型图，只选择实际存在的列（例如第 5、6、7 列）
figure;
boxplot(dataTable(:, 5:7), 'Labels', {'金额', '税额', '价税合计'}); 
title('箱型图');
xlabel('样本属性名称');
ylabel('样本数值大小');
~~~

---
**总体得处理结果：**

~~~
% 读取 Excel 文件中的“进项发票信息”工作表
filename = 'data.xlsx';
sheetName = '进项发票信息';

% 使用 readmatrix 函数读取数据
dataTable = readmatrix(filename, 'Sheet', sheetName, 'Range', 'A2:H210948');

% 选择要处理的列（例如第 5 到第 7 列）
dataToCheck = dataTable(:, 5:7);

% 初始化逻辑索引数组
isNotOutlier = true(size(dataToCheck, 1), 1);

% 遍历每一列，计算异常值并标记异常点
for col = 1:size(dataToCheck, 2)
    columnData = dataToCheck(:, col);
    
    % 计算统计量
    Q1 = prctile(columnData, 25);
    Q3 = prctile(columnData, 75);
    IQR = Q3 - Q1;
    
    % 确定异常值的范围
    lowerBound = Q1 - 1.5 * IQR;
    upperBound = Q3 + 1.5 * IQR;
    
    % 查找并标记异常点
    isOutlier = columnData < lowerBound | columnData > upperBound;
    isNotOutlier = isNotOutlier & ~isOutlier;
end

% 删除异常点所在的行
cleanedData = dataTable(isNotOutlier, :);

% 显示结果
disp('原始数据尺寸:');
disp(size(dataTable));
disp('去除异常值后的数据尺寸:');
disp(size(cleanedData));

% 将去除异常值后的数据写回 Excel（可选）
writematrix(cleanedData, 'cleaned_data.xlsx');

~~~
结果图：

<img src="papers\DigitalAnalog\images\处理后的箱型图.png" alt="logo" title="logo" style="width: 60%; margin: auto; display: block;" />


---

**3、剔除作废发票**

~~~

% 剔除发票状态为“作废发票”的行
cleanedData = cleanedData(~strcmp(cleanedData.('发票状态'), '作废发票'), :);

~~~

### 确定权重
~~~
1、 

~~~
---
**经过数据处理+计算进销项总和+排序得到下图**

<img src="papers\DigitalAnalog\images\初步处理.png" alt="logo" title="logo" style="width: 100%; margin: auto; display: block;" />



---
## [Matlab](papers\DigitalAnalog\Matlab.md)


---
## [华数杯](papers\DigitalAnalog\华数杯.md)