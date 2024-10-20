# 电控基础部分
>主要以学习Stm32，工具为大疆C板、6020云台电机、3508电机等


---

### 云台电机6020
>因为放置镜向所有要重新写要算法，控制两个电机


<img src="papers\BalanceCar\images\电机电流输入为编码值.png" alt="logo" title="logo" style="width: 100%; margin: auto; display: block;" />


---
# 存在的问题：
1、不能平衡（容易一边倒）
2、腿部容易塌陷。
3、站立会更不平衡

---
### 确定遥控的几个量
~~~c
rc_ctrl.rc.ch[0] --->  右侧摇杆水平方向控制
rc_ctrl.rc.ch[1] --->  右侧摇杆竖直方向控制
rc_ctrl.rc.ch[2] --->  左侧摇杆水平方向控制
rc_ctrl.rc.ch[3] --->  左侧摇杆竖直方向控制

上+下-  左+右-
~~~