# 应变式电子秤固件
## 项目目的
目前市场上的电子秤的都有专用芯片或MCU，  
专用芯片内部的处理流程不明，还甚至查不到型号。  
MCU里含有软件，并且这些软件是非自由软件，无法获取到相应的源代码。  

目前很多电子秤的功能都不满足我的需求，而且无法进行二次开发。  
某些电子称有与计算机通信的接口，但是都是难用的串口，而我希望用USB接口。  
我打算自己做一个电子秤，来实现我想要的功能。  


## 所需硬件
- CH32V103C8T6主控
- 称重传感器与AD转换芯片(HX711或CS1237)
- 4x4矩阵键盘
- 128x64 SSD1306 OLED显示屏 I2C接口


## 目前功能
- 置零：按下按键示数清零
- 波形显示：显示AD数值的变化波形（不受置零和零点跟踪影响）
- 零点跟踪：自动识别空载，显示为0.00g，在加载后使用稍前的数据置零
- USB实时数据：将实时数据通过USB发送到电脑
- 动态刷新率(根据变化速率确定刷新间隔，变化时响应快，稳定时较长的滤波点数)


## 待添加功能
- 砝码校准，温度补偿，空气浮力修正
- 稳值判断，数据记录，设置菜单
- 多种单位，计件，超载警报
- 跨平台适配


## 关于版权
### 本软件
Copyright (c) 2022  Xu Ruijun
本软件以GNU通用公共许可证第三版或更新授权(GPL-3.0-or-later)

### 其他
my_libs: 我的另一个项目，使用LGPLv3或更新授权，见[github仓库](https://github.com/XuHg-zjcn/my_libs)或[gitee仓库](https://gitee.com/xu-ruijun/mcu_mylibs)
CH32V103固件: 南京沁恒微电子股份有限公司，Apache 2.0许可证，见[官网上的下载页面](https://www.wch.cn/downloads/CH32V103EVT_ZIP.html)。仓库里没有相关代码，只保留了符号链接，需要自行下载。
12x24字体：`num24.png`以及转换后的`font_12x24.h`使用Ubuntu自带的"Monospace"字体制作，协议不明