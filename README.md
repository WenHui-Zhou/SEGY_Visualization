# SEGY_Visualization
> Using C++ and Python to read a SEG-Y seismic file and and draw a seismogram

**写在前面：** 

这个项目是关于SEG-Y文件的读取，以及可视化工作。所用到的语言是C++，以及Python。C++用于解析文件，Python用于可视化部分。编辑器用的是VS2012,Python的版本为Python36。

**代码未完善之处有：**

 - 显示过于粗糙，有时间的话，后续将写一个界面
 - 代码许多地方需要补充处理异常代码
 - 代码结构混乱

**以下是核心代码具体说明：**

code       | introduction
--------   | ---
LoadPic.py | 生成地震图
main.cpp   | 代码执行顺序
segy.h     | 解析segy文件

关于C++调用Python的环境配置可以看[这里](https://github.com/WenHuiXie/the-road-to-python/blob/master/VS2012%20C++%E8%B0%83%E7%94%A8Python36%E9%85%8D%E7%BD%AE%E5%8F%8A%E5%AE%9E%E7%8E%B0.md) 。
