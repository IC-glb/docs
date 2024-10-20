<center><h2>🚀 能量机关识别与预测（2024）</h2></center>

<img src="https://github.com/user-attachments/assets/02e50b4b-ce7c-4ab4-b4f7-c61ccb5c7d16"  style="width: 70%; margin: auto; display: block;" />

## 🎉 目录
1. [文件介绍](#🚀文件介绍)
2. [能量机关识别](#能量机关识别)
    - [1. 构造函数初始化参数](#1-构造函数初始化参数)
    - [2. 识别流程函数](#2-识别流程函数)
    - [3. 轮廓的识别](#3-轮廓的识别)
    - [4. R的识别流程](#4-对R的识别流程)
    - [5. Leaf识别](#5-对Leaf识别)
        - [5.0 计算角度](#5-0-计算角度)
        - [5.1 抽象概念介绍](#5-1-抽象概念介绍)
        - [5.2 固定坐标](#5-2-固定坐标)
        - [5.3 目标](#5-3-目标)
3. [能量机关预测](#能量机关预测)
4. [如何调试](#如何调试)
5. [发展](#发展)
6. [测试视频](#测试视频)
## 🚀文件介绍

*参数配置文件*
```tree
├── SetParam_File
│   ├── Buff_data.yaml  //定义常量的值
│   ├── ...

```
*项目核心文件*

```tree
├── Buff_Detection                 // 识别
│   ├── buff_logs                  // 输出日志文件夹
│   ├── include
│   │   ├── BuffDetection.h        // 能量机关识别头文件
│   │   └── Logger.h               // 输出日志头文件包含函数（将数据以txt文件保存在指定文件夹---方面绘图可视化）
│   └── src
│       └── Buff_Detection.cpp     // 识别源文件
├── Buff_Prediction                // 预测
│   ├── include
│   │   └── BuffPrediction.h       // 预测头文件
│   └── src
│       └── Buff_Prediction.cpp    // 预测的源文件
```
<details>
<summary>功能包介绍</summary>
serial_device：串口通信功能包

robot_msgs：ROS自定义消息包

hik_camera：海康相机功能包

Model：模型集合包 (onnx模型)

public_include:：公共头文件包

robot_detection => SetParam_File：配置文件

robot_detection => AngleSolve：坐标系转换功能包

robot_detection => Armor_Detection：装甲板识别功能包

robot_detection => Armor_Track：装甲板跟踪功能包

robot_detection => Armor_Observe：整车观测功能包

robot_detection => Buff_Detection：能量机关识别功能包

robot_detection => Buff_Prediction：能量机关预测功能包

robot_detection => Neural_Armor_Detection：装甲板深度学习识别功能包

robot_detection => Neural_Buff_Detection：能量机关深度学习识别功能包

robot_detection =>  Outpost_Observe：反前哨站功能包

</details>

---
 
  

## 能量机关识别
### 1. 构造函数初始化参数

使用FileStorage类构造函数读取yaml文件数据，赋值于类定义的参数   
```
cv.FileStorage( source, flags， const string& encoding=string());

source –存储或读取数据的文件名（字符串），其扩展名(.xml 或 .yml/.yaml)决定文件格式。

flags – 操作模式，包括：
FileStorage::READ 打开文件进行读操作
FileStorage::WRITE 打开文件进行写操作
FileStorage::APPEND打开文件进行附加操作
FileStorage::MEMORY 从source读数据，或向内部缓存写入数据（由FileStorage::release返回）
```
<details>
<summary>ProjectCode</summary>

```c++
BuffDetection::BuffDetection()
{
    std::string package_path = ros::package::getPath("robot_detection");    // 通过ros获取路径
    cv::FileStorage fs(package_path + "/SetParam_File/Buff_data.yaml", cv::FileStorage::READ);  // 读取yaml文件

    //通道分离参数 红蓝各种阈值
    br_blue_threshold = (int) fs["br_blue_threshold"];   
    br_red_threshold = (int) fs["br_red_threshold"];

    rb_red_threshold = (int) fs["rb_red_threshold"];
    rb_blue_threshold = (int) fs["rb_blue_threshold"];

    //矩形 轮廓筛选部分参数
    r_min_area = (int) fs["r_min_area"];
    r_max_area = (int) fs["r_max_area"];

    leaf_min_area = (int) fs["leaf_min_area"];
    leaf_max_area = (int) fs["leaf_max_area"];

    //R标信息参数
    r_max_hb_ratio = (float) fs["r_max_hb_ratio"];  // 斜边和底边比率
    r_min_hb_ratio = (float) fs["r_min_hb_ratio"];

    r_max_hw_ratio = (float) fs["r_max_hw_ratio"];
    r_min_hw_ratio = (float) fs["r_min_hw_ratio"];

    R_ReInited_flag = (int) fs["R_ReInited_flag"];

    r_max_are_ratio = (double) fs["r_max_are_ratio"];
    r_min_are_ratio = (double) fs["r_min_are_ratio"];

    //leaf信息筛选参数
    leaf_max_hw_ratio = (float) fs["leaf_max_hw_ratio"];
    leaf_min_hw_ratio = (float) fs["leaf_min_hw_ratio"];

    leaf_r_max_distance = (int) fs["leaf_r_max_distance"];
    leaf_r_min_distance = (int) fs["leaf_r_min_distance"];


    //图像分类置信度评判
    leaf_classifier_confidence = (int) fs["leaf_classifier_confidence"];

    //R标图像匹配路径
    RModelPath = (std::string) fs["RModelPath"];
 
    // 分类模型路径赋值
    LeafModelPath = (std::string) fs["LeafModelPath"];
    HandleModelPath = (std::string) fs["HandleModelPath"];
 
    fs.release();
}
```
</details>

### 2. 识别流程函数

``` mermaid
graph TD;
    A[图像输入] --> B[图像红蓝分割];
    B --> C[初始化];
    C --> D[轮廓检测];
    D --> E[目标筛选];
    E --> F[返回最终目标]
```

可以点开ProjectCode,里面有代码注释
<details>
<summary>ProjectCode</summary>


```c++

std::vector<BuffTarget> BuffDetection::Detection(cv::Mat &frame_) {

    /**
    * @brief 对图像初步增强、采用图像分割、膨胀和均值滤波 等
    * @param  加上膨胀、滤波操作时间消耗长
    * @return  返回二值化图像
    */



    SetImage(frame_);
 
    /**
     * @brief 对用到的变量开关/容器初始化
     */

    Detection_Init();
    /**
     * @brief 1.寻找初始轮廓 2.将符合初筛条件的轮廓 分别进入R 与 leaf 的筛选中
     * @param candidateRect_Buff结构体是在robot_struct定义的_将识别信息传到预测类里面
     * @return  返回容器finalBuff
     */

    findCandidateRect(binary);

    
    /**
       * @brief 应对可能出现两个目标（新目标已经出现，旧目标还没有来得及切换图案的时候）
       * @param 判断对于的位置与R标的距离
       * @return  Nullptr
       */
    Get_final_buff();

    /**
     * @brief 可视化数据
     */
    show(frame_);


#ifdef SHOW_IMAGE
    cv::imshow("frame_" , frame_);
    cv::imshow("binary",binary);

#endif
    
    return final_buff_target;

}
```
</details>

### 3. 轮廓的识别

``` mermaid
graph TD;
    A[轮廓] -->|物理特征| B[初筛];
    B--> |长宽比/面积/是否初始化|C[R标轮廓];
    B--> |长宽比/面积| D[Leaf轮廓];

```

### 4. 对R的识别流程
```mermaid
graph TD;
    A[初筛轮廓];
    B[初始化标志];
    C[像素--->世界坐标系];
    D[是否第一次识别];
    E[距离图像中心点近];
    F[物理特征符合];
    G[是];
    H[否];
    A-->D;
    D-->G;
    D-->H;
    G-->E;
    G-->F;
    I[确定为R标];
    F --> I;
    E --> I;
    I-->C;
    C-->|第一次识别为True|A1[R的世界坐标系]

    H-->H1[R三维坐标转换像素坐标]
    H1-->H2[标志数++]
    H2-->H3[是否触发标志位]
    H3-->H4[是]
    H3-->H5[否]
    H5-->F1[目前世界转像素坐标即为R坐标]
    H4 -->|RUpate= True|H6[进入R三维坐标修正程序]
    H6 --> H7[距离上一时刻R坐标相近]
    H6 --> H8[满足R的物理特征]
    H7 --> H9[该轮廓为新的R]
    H9 -->B
    H9 -->I
    H8 -->H9
```

**注意**
- 第一次识别需要操作手手动将图像中心对准R标（才会进入识别）
- 只要第一次识别后且更新标志不为True 则只单纯使用坐标系转换得到像素坐标作为R标坐标
- **R的四个点坐标顺序对测距没影响且不是跟踪的事物，故不对坐标固定（但也可以固定）**


### 5. 对Leaf识别

```mermaid
graph TD;
    A[初筛轮廓]
    B[获取旋转角度]
    C[固定旋转矩形四点]
    D[取中点得到目标]
    E[仿射变换得ROI]
    F[模型推理]
    G[真实轮廓]
    H[初筛目标数量]
    I[存入最终目标容器]
    J[对符柄进行模型推理]
    K[符柄ROI]
    A --> B;
    B --> C;
    C --> D;
    D --> E;
    E --> F;
    F --> |True|G;
    G --> H;
    H --> |size=1|I ;
    H --> |size>1|J;
    J --> K;
    K --> F;
```

#### 5.0 计算角度

计算两个向量之间夹角的函数
给定三个点 `pt1(x1, y1)`, `pt2(x2, y2)`, `pt0(x0, y0)` 

步骤：
1. 向量点积公式
2. 计算两个向量的夹角余弦
3. 反余弦算夹角
4. 角度范围调整（根据点 pt0.y 和 pt1.y 的相对位置，对角度范围进行调整）
<details>
<summary>具体代码</summary>

```c++
float BuffDetection::getAngle(cv::Point2f pt1, cv::Point2f pt2, cv::Point2f pt0, std::string mode) {
    float _circleAngle360, _circleAngle180;
    float dx1 = pt1.x - pt0.x;
    float dy1 = pt1.y - pt0.y;
    float dx2 = pt2.x - pt0.x;
    float dy2 = pt2.y - pt0.y;
    float angle_line = (dx1 * dx2 + dy1 * dy2) / sqrtf((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10f);
    _circleAngle180 = acosf(angle_line) * 180.0f / 3.141592653f;

    if (pt0.y < pt1.y) {
        _circleAngle360 = 360.0f - _circleAngle180;
        _circleAngle180 = -_circleAngle180;
    } else {
        _circleAngle360 = _circleAngle180;
    }
    if (mode == "360") { return _circleAngle360; }
    else { return _circleAngle180; }
}
```
</details>


#### 5.1 抽象概念介绍
概念：(如叶子)
- 符叶 leaf 
- 符柄 handle
- 目标 target

<details>
<summary>符叶的样子</summary>
<img src="https://github.com/user-attachments/assets/dd4369b6-f521-4c9c-8524-591fcb003122"  style="width: 30% ; height:30%; margin: auto " />
<img src="https://github.com/user-attachments/assets/9b1bcbdf-a34c-4fe6-8cab-9495270a5493"  style="width: 30% ; height:30%; margin: auto " />
</details>
<details>
<summary>符柄的样子</summary>
<img src="https://github.com/user-attachments/assets/f0009877-876a-42a4-ab9b-08d17f3aeba0"  style="width: 30% ; height:30%; margin: auto " />
<img src="https://github.com/user-attachments/assets/1e3b01d1-bb53-4b00-b7ec-ce5c256ed06a"  style="width: 30% ; height:30%; margin: auto " />
</details>
<details>
<summary>目标 target</summary>
<img src="https://github.com/user-attachments/assets/98da0f53-beb0-4345-ad2f-57454d03c97d"  style="width: 30% ; height:10%; margin: auto  " />
</details>


#### 5.2 固定坐标 

Qustion: 🤔为什么需要固定坐标?

Answer: **因为符是旋转的，而是使用旋转矩形后，随着角度变化，符叶四个点会发生变化。方便后期仿射变换得到要求的ROI图像**


**坐标顺序（当前代码以图1为正参照）**

- leaf_target[0]        左下
- leaf_target[1]        左上
- leaf_target[2]        右上
- leaf_target[3]        右下

<div style="text-align: center;">
    <img src="https://github.com/user-attachments/assets/efcc6ad6-940c-4787-9a9a-bcbac084e5a6" 
         style="width: 30%; height: 30%; display: inline-block; margin: 10px;" />
    <img src="https://github.com/user-attachments/assets/2b67216a-99c3-4f4a-88f8-426cff0248f3" 
         style="width: 25%; height: 30%; display: inline-block; margin: 10px;" />
</div>


<details>
<summary>对应代码</summary>

```c++
if (leaf_.angle >= 0 && leaf_.angle < 91 || leaf_.angle >= 353 && leaf_.angle <= 360) {
            leaf_target[0] = Vertex[0];
            leaf_target[1] = Vertex[1];
            leaf_target[2] = Vertex[2];
            leaf_target[3] = Vertex[3];

        } else if (leaf_.angle >= 91 && leaf_.angle < 181) {
            leaf_target[0] = Vertex[3];
            leaf_target[1] = Vertex[0];
            leaf_target[2] = Vertex[1];
            leaf_target[3] = Vertex[2];

        } else if (leaf_.angle >= 181 && leaf_.angle < 266) {
            leaf_target[0] = Vertex[2];
            leaf_target[1] = Vertex[3];
            leaf_target[2] = Vertex[0];
            leaf_target[3] = Vertex[1];

        } else {
            leaf_target[0] = Vertex[1];
            leaf_target[1] = Vertex[2];
            leaf_target[2] = Vertex[3];
            leaf_target[3] = Vertex[0];
        }
```

**为什么不是0 90 180 270 这样固定。自己可以试试看。毕竟这都是测出来的，大概是跟分辨率不是正比例有关系吧**

</details>


#### 5.3 模型推理
>该分类器的实现基于 OpenCV 的 DNN（深度神经网络）模块来加载和推理 ONNX 模型，并返回一个布尔值


``` mermaid
graph TD;
    A[模型加载]
    B[图像转换为blob（四维数组）]
    C[设置网络的输入]
    D[前向传播forward]
    E[找到最高概率的类别]
    F[分类判断]
    G[释放内存,返回结果]
    A -->B;
    B --> C;
    C --> D;
    D --> D1[输出prob表示类别的概率分布]
    D --> E;
    E --> F;
    F --> G;

```

<details>
<summary>模型推理代码</summary>

```c++
bool BuffDetection::classifier(cv::Mat &src, size_t id, std::string &ModePath) {
    double confidence;
    size_t classId;
    // 加载ONNX模型

    cv::dnn::Net net = cv::dnn::readNetFromONNX(ModePath);

    // 将图像转换为blob
    cv::Mat blob = cv::dnn::blobFromImage(src, 1.0, cv::Size(30, 30), cv::Scalar(), true, false);

    // 设置网络的输入
    net.setInput(blob);

    // 进行前向传播以获取输出
    cv::Mat prob = net.forward();

    // 找到概率最高的类别
    cv::Point classIdPoint;
    minMaxLoc(prob.reshape(1, 1), nullptr, &confidence, nullptr, &classIdPoint);

    classId = classIdPoint.x;

    if (classId == id && confidence * 100 > leaf_classifier_confidence) {
        blob.release();
        prob.release();
        return true;
    } else {

        blob.release();
        prob.release();
        return false;
    }
}
```
</details>


#### 5.3 目标

**5.3.1目标的点顺序（固定|预测框的点位也要一样）**

<div style="text-align: center;">
    <img src="https://github.com/user-attachments/assets/3be143fd-07fa-4dd7-b3cd-bded828b2570" 
         style="width: 35%; height: 30%; display: inline-block; margin: 10px;" />
    <img src="https://github.com/user-attachments/assets/1f38ec10-8b07-424e-8ff2-dea91c49e5ab" 
         style="width: 31%; height: 30%; display: inline-block; margin: 10px;" />
</div>

**5.3.2目标干扰**

<span style="color: red;">1.打中十环后的图案和目标图案是非常相似的,使用要使用符柄进行二筛。</span>

<img src="https://github.com/user-attachments/assets/ce551c91-1c3a-4714-9411-b6fde08ac6bf" 
         style="width: 40%; height: 30%;"/>


<span style="color: red;">2.出现两个目标进行二筛。</span>

<img src="https://github.com/user-attachments/assets/9ebc3ef1-bb9e-4982-913b-6e81f4d70d6b" 
         style="width: 40%; height: 30%;"/>

---

## 能量机关预测
<video width="100%" height="80%" controls>
  <source src="https://github.com/user-attachments/assets/6f0700aa-6c86-446e-a70a-2b5157fbbb08" type="video/mp4">
</video>


### 1.小能量机关预测

固定预测角度就行。没啥难度...


### 2.大能量机关预测

采集目标转动的角度，使用傅里叶变换，拟合目标方程，得到幅值、相位等。再根据弹道飞行时间+发弹延时 求解预测未来时刻的旋转角度

此方案参考开源---感谢提供思路！
- [西安电子科技大学-IRobot战队能量机关拟合部分](https://github.com/SanZoom/RM2022-Infantry-Vision/blob/master/src/Rune/Fitting.cpp) 
- [桂林电子科技大学Evolution战队](https://github.com/freezing00/Baldr/blob/main/Src/angle/CoordinateSolver/BuffCoordinateSolver.cpp)

预测思路可以看佛科大的大能量机关推导

- [佛山科学技术学院醒狮战队大能量机关推导](https://github.com/Ash1104/RoboMaster2021-FOSU-AWAKENLION-OpenSource/blob/master/%E5%A4%A7%E8%83%BD%E9%87%8F%E6%9C%BA%E5%85%B3%E6%8E%A8%E5%AF%BC.pdf)


## 如何调试

```mermaid
graph TD;
    A[检查相机的内外参是否对应]
    B[8m击打装甲板是否精确]
    C[调整抬枪补偿参数]
    C1[运行能量机关Debug模式]
    D[首次程序先将图像中心靠近R标]
    E[进入识别预测模型]
    F[击打]
    G[调节滑动条修改模型预测量]
    
    A --> B;
    B --> |否|C;
    B --> |是|D;
    D --> C1;
    C1 --> E;
    E --> F;
    F --> G;

```

**🚨注意事项**

`Debug模式是针对性设计的，为了满足上场快速调式、场地道具测试快速调试、日常快速调试，主程序Robot_Detection才是核心`

1. `遥控控制大小符`需要自行，叫电控改代码。也可以将`vision_mode` 自瞄模式改成大符/小符模式;
2. 大符预测中的`DT`与`DELAY_TIME_`参数会影响预测的，可以滑动条改变一下看看效果;
3. 代码会自动识别顺逆时针，但小概率出现失误。跟`_buffAngleList.size() > 10`有关，`（程序刚进入采集时候突然切换符叶导致数据空间上不连续，影响判断）`;
4. 能量机关不会感应击打，需要遥控操控:`0 改变颜色 ; 1-6切换目标 ,且1-6图案不一样`;
5. 识别不到可能是因为曝光严重，可以看看`二值化图像是否过曝`,可以滑动条修改`通道分割阈值`
6. 不要将自启动程序改成`Buff_Debug`
7. 图传模式：`大符C` `小符X`
8. `神经网络模型训练看看就好,效果还行,实际迭代了几十个版本`
<details>
<summary>模型训练代码</summary>

```python
import numpy as np
import torch
import torch.nn as nn
import torch.optim as optim
import torchvision.transforms as transforms
from matplotlib import pyplot as plt
from torchvision.datasets import ImageFolder
from torch.utils.data import DataLoader
import torch.onnx
from tqdm import tqdm
from tool.Matplot_Tool import plot_data
from tool.Train_Tool import train_model, SaveModel, EarlyStopping
from tool.Train_Tool import evaluate_model

from torch.optim.lr_scheduler import CosineAnnealingLR
from torch.nn.utils import clip_grad_norm_

# 数据转换和加载
transform = transforms.Compose([
    transforms.Grayscale(num_output_channels=1) ,
    # transforms.RandomHorizontalFlip(),
    # transforms.RandomVerticalFlip(),
    # transforms.RandomRotation(10),
    transforms.ToTensor(),
 ])
test_transform = transforms.Compose([
    transforms.Grayscale(num_output_channels=1),
    transforms.ToTensor()


])
# 数据集的根目录包含两个子文件夹，每个子文件夹对应一个类别
train_dataset = ImageFolder(root='HandleDatasets/train', transform=transform)
test_dataset = ImageFolder(root='HandleDatasets/test', transform=test_transform)

train_loader = DataLoader(train_dataset, batch_size=32, shuffle=True)
test_loader = DataLoader(test_dataset, batch_size=32, shuffle=False)


class SimpleLeNet5(nn.Module):
    def __init__(self, num_classes):
        super(SimpleLeNet5, self).__init__()
        self.conv1 = nn.Conv2d(1, 6, kernel_size=2)

        self.pool1 = nn.MaxPool2d(kernel_size=2, stride=2)
        self.conv2 = nn.Conv2d(6, 12, kernel_size=2)

        self.pool2 = nn.MaxPool2d(kernel_size=2, stride=2)
        self.fc1 = nn.Linear(12 * 6 * 6, 256)   #432个参数
        self.dropout= nn.Dropout(0.5)
        self.fc2 = nn.Linear(256, 128)
        self.fc3 = nn.Linear(128, 64)
        self.fc4 = nn.Linear(64, num_classes)


    def forward(self, x):
        # print(x.shape)
        x = self.pool1(torch.relu(self.conv1(x)))
        x = self.pool2(torch.relu(self.conv2(x)))
        # print(x.shape)
        x = x.view(x.size(0), -1)
        x = torch.relu(self.fc1(x))
        x = self.dropout(x)
        x = torch.relu(self.fc2(x))
        x = self.dropout(x)
        x = torch.relu(self.fc3(x))
        x = self.dropout(x)
        x = self.fc4(x)
        return x

def main():
    # 创建模型并将其移动到GPU上
    num_classes = 2
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

    model = SimpleLeNet5(num_classes=num_classes).to(device)
    # 实例化一个TensorBoard写入器
    # writer = SummaryWriter()
    # 定义损失函数和优化器
    num_epochs = 100
    patience = 10  # 设置为你认为合适的值

    learning_rate = 0.005
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.Adam(model.parameters(), lr=learning_rate)
    scheduler = CosineAnnealingLR(optimizer, eta_min=0.0001 , T_max=num_epochs)

    # 定义余弦退火学习率调度器
    # 训练模型

    train_losses, test_losses, train_accs, test_accs = [], [], [], []
    learning_rates = []
    best_val_acc = 0.0
    best_epoch = 0
    early_stopping = EarlyStopping(patience=10, verbose=True)
    for epoch in range(num_epochs):
        progress_bar_train = tqdm(train_loader, desc=f'Train epoch {epoch + 1} / {num_epochs}', mininterval=0.3)
        train_loss, train_acc = train_model(model, progress_bar_train, criterion, optimizer, device, epoch,
                                            progress_bar_train)
        train_losses.append(train_loss)
        train_accs.append(train_acc)

        progress_bar_val = tqdm(test_loader, desc=f'Val epoch {epoch + 1} / {num_epochs}', mininterval=0.3)
        test_loss, test_acc = evaluate_model(model, progress_bar_val, criterion, device)
        test_losses.append(test_loss)
        test_accs.append(test_acc)

        # 更新学习率
        scheduler.step()
        # 记录学习率
        current_lr = optimizer.param_groups[0]['lr']
        learning_rates.append(current_lr)
        # 检查是否发生了过拟合
        early_stopping(test_loss,model,device)
        # 如果早停法触发，则提前结束训练
        if early_stopping.early_stop:
            # SaveModel(model, device)
            print("Early stopping")
            break

        # 打印训练和验证信息
        print(f'Train - Epoch [{epoch + 1}/{num_epochs}] - Loss: {train_loss:.6f}, Accuracy: {train_acc:.2f}%')
        print(f'Validation - Epoch [{epoch + 1}/{num_epochs}] - Loss: {test_loss:.6f}, Accuracy: {test_acc:.2f}%')

        # 绘制损失和准确率曲线
    plot_data(train_losses, test_losses, train_accs, test_accs)

    # 绘制学习率变化图像
    plt.figure()
    plt.plot(range(1, len(learning_rates) + 1), learning_rates, label='Learning Rate')
    plt.xlabel('Epoch')
    plt.ylabel('Learning Rate')
    plt.title('Learning Rate Schedule')
    plt.legend()
    plt.show()
    # 导出为ONNX格式（尽量使ONNX文件小于1MB）
    SaveModel(model , device)
    # 绘制Loss曲线图

if __name__ == "__main__":
    main()

```
</details>


## 发展

1. `数据预处理部分可以改成卡尔曼`
<details>
<summary>代码</summary>

```c++
void BuffPrediction::calculateRotateSpeed(BuffTarget &buffTargetPrediction)
{
    //定义静态过去和现在角度；
    static double nowAngle = 0.0f;
    static double lastAngle = 0.0f;
    static int count = 0;
    //定义过去和现在时间
    static double lastTime = (double) cv::getTickCount() / cv::getTickFrequency() * 1000; // ms
    double curTime = (double) cv::getTickCount() / cv::getTickFrequency() * 1000;
//    double curTime_ = (double) cv::getTickCount() /  cv::getTickFrequency()*1000.0  ;
//    std::cout<<curTime_<<std::endl;
    //如果叶片没有跳变，则把过去和现在角度以及过去和现在速度置零
//        std::cout<<leaf_.angle<<std::endl;
    if (!_sameLeaf) {
        lastAngle = nowAngle = _rotateSpeed.lastRotateSpeed = _rotateSpeed.nowRotateSpeed = 0.0f;
        return;
    }

    //如果过去角度已经被清零，则过去角度进行初始化为现在绝对角度
    if (lastAngle == 0.0f) {
        lastAngle = buffTargetPrediction.leaf_angle;
        return;
    }

    //每0.1s一次数据刷新
    if (curTime - lastTime < 100 ) {
        return;
    }
    //帧数递增
    count++;
    nowAngle = buffTargetPrediction.leaf_angle;
    //计算实时角速度
    _rotateSpeed.nowRotateSpeed = (float) fabs( angleToRadian((nowAngle - lastAngle)) * (1000.0f / (curTime - lastTime)));


    //过去角度和时间更新
    lastAngle = nowAngle;
    lastTime = curTime;
    //如果过去角速度已被清零，则对过去速度进行更新
    if (_rotateSpeed.lastRotateSpeed == 0.0f) {
        _rotateSpeed.lastRotateSpeed = _rotateSpeed.nowRotateSpeed;
        return;
    }
    //防止出现异常数据
    if (_rotateSpeed.nowRotateSpeed > 5 || _rotateSpeed.nowRotateSpeed < -5) {
        return;
    }

    //如果速度没有替换最小速度，则计数加1
    if (_speedRange.nowMinSpeed > _rotateSpeed.nowRotateSpeed) {
        _speedRange.nowMinSpeed = _rotateSpeed.nowRotateSpeed;
    } else {
        _speedRange.minSameNumber++;
    }
    //如果速度没有替换最大速度，则计数加1
    if (_speedRange.nowMaxSpeed < _rotateSpeed.nowRotateSpeed) {
        _speedRange.nowMaxSpeed = _rotateSpeed.nowRotateSpeed;
    } else {
        _speedRange.maxSameNumber++;
    }
    //如果连续20帧没有刷新最小速度，则该速度为波谷速度（该速度一旦更新，便不再更新）
    if (_speedRange.minSameNumber > 30 && !_speedRange.minSpeedFlag) {
        _speedRange.realMinSpeed = _speedRange.nowMinSpeed;
        _speedRange.minSpeedFlag = true;
    }
    //如果连续20帧没有刷新最大速度，则该速度为波峰速度（该速度一旦更新，便不再更新）
    if (_speedRange.maxSameNumber > 30 && !_speedRange.maxSpeedFlag) {
        _speedRange.realMaxSpeed = _speedRange.nowMaxSpeed;
        _speedRange.maxSpeedFlag = true;
    }

    _rotateSpeed.realRotateSpeed = _rotateSpeed.nowRotateSpeed;

    //赋予开火结构体现在的预测角度
    fire.GetNowSpeed = _rotateSpeed.realRotateSpeed;
}

```
</details>


2. `可以试试别的预测方案;`
3. `能力强可以从头写,注意代码可读性与规范化。`
4. 🚨`能量机关最好可以弹丸检测,检测环数,不然只能听个响`
5. 🚨`能量机关需要大量测试！！！能量机关需要大量测试！！！能量机关需要大量测试！！！`
6. 🚨`本代码后续已不打算优化,代码规范......`
7. `场上发现曝光其实非常理想，所以R标识别可以不用这么保险，其甚至可以直接传统识别到，轮廓特别干净`


## 测试视频

`下面是我的仓库,垃圾堆中有些许测试视频`

1. [hj_vision](https://gitee.com/icccc8888/hj_vision)
2. [video_test](https://gitee.com/icccc8888/video_test)
3. [buff_debug](https://gitee.com/icccc8888/buff_dubug)
