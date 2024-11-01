<center><h1>Stm32 学习</h1></center>

---
## GPIO

### 寄存器数量

- 两个 32 位配置寄存器（`GPIOx_CRL/GPIOx_CRH`）
- 两个 32 位数据寄存器（`GPIOx_IDR/GPIOx_ODR`）
- 一个 32 位置位/复位寄存器（`GPIOx_BSRR`）
- 一个 16 位复位寄存器（`GPIOx_BRR`）
- 一个 32 位锁定寄存器（`GPIOx_LCKR`）

### 配置模式

1. `输入浮空`接受实际输入电压
2. `输入上拉`通过一个电阻使其上拉到 Vcc
3. `输入下拉`把电压拉低至 GND
4. `推挽输出`可输出高低电平，由三个三极管控制
5. `模拟输入`0/1 二进制数字信号
6. `开漏输出`需要一个上拉电阻，外部供电

> `开漏和推挽都是通过P-MOS 和 N-MOS来控制的，区别在于输出电压`

### GPIO 功能

1. 普通 I/O 功能
2. 单独的位设置或清除功能
3. 外部中断
4. 复用功能
5. ...

### 点灯

```c
#include"stm32f10x.h"
void delay ms(unsigned short int Number);
int main(void){
    //包含STM32F1系列微控制器的头文件//延时子函数
    //声明用于GPI0初始化的结构体
    GPI0 InitTypeDef GPI0 InitStructure;
    RCC APB2PeriphClockCmd(RCC APB2PeriphGPIOE,ENABLE);//使能PE时钟
    //对PE0引脚进行设置
    GPI0 InitStructure.GPI0 Pin =GPI0 Pin 0;
    GPI0 InitStructure.GPI0 Mode = GPI0 Mode Out PP;//选择推挽输出模式
    GPI0 InitStructure.GPI0 Speed=GPI0 Speed 50MHz;//频率最高为50MHz
    GPI0 Init(GPIOE,&GPI0 InitStructure);
    //对引脚进行配置
    while(1 ){
        GPI0 SetBits(GPIOE，GPI0 Pin 0);
        delay ms(500);
        GPIO ResetBits(GPIOE，GPI0 Pin 0);
        delay ms(500);
    }
    //输出高电平
    //输出低电平
    void delay ms(unsigned short int Number){
        unsigned int i;
        while(Number --)
            {
                i=12000;
                while(i--)
            };
    }
}
```

---
## 中断和事件

### 优先级

**抢占优先级和响应优先级（或副优先级）**

1) 抢占优先级

高抢占优先级可以在低抢占优先级中断处理过程中被响应，称`中断嵌套`

2) 响应优先级
`在抢占式优先级相同的情况下`，高副优先级的中断优先被响应;
`在抢占式优先级相同的情况下`,如果有低副优先级中断正在执行,`高副优先级的中断要等待已被响应的低副优先级中断执行结束后才能得到响应`,即所谓的非抢断式响应(不能嵌套)。

### 中断管理

`NVIC`是向量中断控制器。

#### 中断的使能和除能
传统的中断的使能和除能是通过设置中的中断控制寄存器的一个相应位为1/0来完成的,而 `Cortex-M3` 中，通过设置 NVIC 的`中断使能（SETENAx）、失能(CLRENAx)寄存器`来完成。

#### 中断的挂起和解挂
如果中断被抢断了，该中断被挂起。中断挂起状态可以通过设置中断挂起寄存器(SETPENDx)和挂起清除寄存器(CLRPENDx)来读取。

### 优先级


