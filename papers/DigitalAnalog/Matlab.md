# Matlab 
>记录一些Matlab的用法。
---

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
1、 信誉评级X1
信用等级是根据企业实际情况评定的，可为企业资信提供高度参考，将其进行数据映射，A=100，B=80，C=60(银行对信誉评级为D的企业原则上不予放贷，无需做量化分析)

2、信用记录X2
根据企业是否出现过违约来判断，未出现违约获得全部分数100，出现则获得一半分数50

3、主营业务利润率 X3
反映企业的盈利能力，该指标越高，产品附加值高，市场竞争力越强，发展潜力越大，放贷风险也就越小，主营业务利润率=主营业务利润/主营业务收入额，得到的结果*100即为最终分数
~~~
---
1、经过数据处理+计算进销项总和+排序得到下图 

<img src="papers\DigitalAnalog\images\初步处理.png" alt="logo" title="logo" style="width: 100%; margin: auto; display: block;" />


2、计算三年利润率（每一年有点难，最好是年利润率求平均）
~~~
% 读取 Excel 文件中的数据
filename = 'data.xlsx';
sheetName = '企业信息'; % 替换为实际的工作表名称
dataTable = readtable(filename, 'Sheet', sheetName);

% 提取需要的列数据（根据列索引）
inputTotalAmount = dataTable{:, 5};  % 第5列：进项总金额
outputTotalAmount = dataTable{:, 8}; % 第8列：销项总金额

% 计算毛利润率
grossProfitMargin = (outputTotalAmount - inputTotalAmount) ./ outputTotalAmount * 100;

% 将毛利润率添加到表格中
dataTable.GrossProfitMargin = grossProfitMargin;

% 将更新后的表格保存到新的 Excel 文件
writetable(dataTable, 'updated_data_with_margin.xlsx');

% 显示结果
disp('毛利润率已计算并保存到 updated_data_with_margin.xlsx 文件中。');
~~~


3、给信用评级赋分
~~~
=IF(C2="A", 100, IF(C2="B", 80, IF(C2="C", 60, IF(C2="D", 40, 0))))
~~~


4、给守约度赋分
~~~
=IF(D96="否",100,IF(D96="是",50))
~~~