# train-simulation 2016年CS大作业 @ BUPT

模拟火车调度系统程序设计的实验要求

要求根据下面的要求实现模拟火车调度软件。

## 一、参数配置

1.	火车数量：最小为3
2.	火车类型：快车、慢车
3.	火车运行方向：顺时针、逆时针
4.	火车启动运行位置：有默认值，系统运行时可修改初始配置
5.	火车启动时间：0表示即可启动，其它整数值表示系统运行整数秒后启动
6.	火车运行速度：每秒移动的距离
7.	火车占用公共轨道停靠时间：0表示不占用，单位为秒，值不0为要占用时，要求在公共轨道中间停靠。

## 二、火车基本要求

1. 至少有三辆小火车 (如图1所示)。
2. 小火车各自分别沿参数指定方向行驶在自己的闭合轨道上。当运行到公共轨道时需监测是否会有冲突发生，如当火车A在公共轨道上运行的时候，另一辆火车B如果也想进入公共轨道，则必须等待，直到列车A离开了公共轨道，让出该段轨道。为了防止两辆火车在公共轨道上相撞，必须要有一个中央控制系统来调度火车的运行。
3. 为了能及时识别火车想进入公共轨道或者已经离开公共轨道，在公共轨道等待进入和驶出轨道双方向上，分别安装两个探测点，当探测点发现火车经过此点时，将会发送信号给中央控制系统，中央控制系统根据发送信号的是哪个探测点，识别出是哪辆火车想进入或者驶出公共轨道。

## 三．火车的运行控制

1. 火车从启动位置开始运行，除非因为等待公共轨道的释放而暂停，或者被人为停止，否则会一直运行。
2. 必须要保证所有火车有序运行，交替使用公共轨道。
3. 火车的运行速度等参数可以在配置文件中设置，也可在系统刚开始运行时提示是否修改默认配置，如果是则可以修改默认参数。
4. 在火车运行期间，火车除了因为等待公共轨道的释放而暂停，也可能由于某些特殊原因被人为停止，用户可以通过输入命令信号来控制火车的暂停和继续运行。
5. 如果火车在公共轨道停靠时间不为0时，要求在轨道中间位置停靠，并以秒为单位显示停靠倒计时的时间。

## 四．火车运行的控制策略
如果出现2个监测点同时向中央控制中心发信号，提示火车要进入公共运行轨道，这时需要根据以下三种策略来决定哪辆火车被允许进入。

- 交替策略：快、慢车交替使用公共轨道。第一次出现同时申请进入的情况时，让A车先行，第二次时让B车先行，第三次还是让A车先行，第四次B车先行。
- 快车优先策略：根据火车的运行速度，选择快车优先通过，慢车等待快车通过后再进入公共轨道，同级别情况下采用交替策略。
- 人工控制策略：当同时有多辆车请求进入公共轨道时，可以人工控制指定某辆车进入，其它车等待。

## 五．输入输出
要求开发两个版本：命令行版本和动画版本。具体说明如下：

- 命令行版本，要求实现键盘输入、命令行输出，并将输出结果记录到文件。
- 动画版本，要求实现图形界面输入、动画输出，并将输出结果记录到文件。

###（一）输入
一系列的火车控制信息和调度命令。输入数据包括以下两种：

1. 初始化信息，包括：各辆火车的车速、启动时间（相对于程序开始运行时间，以秒为单位）、启动位置以及探测点位置等，参见内容一说明。轨道宽度、高度、位置坐标可为常量。
2. 暂停或者继续某辆火车运行的控制信号。

---
输入方式可以采用两种方法：

a．从键盘输入。例如：可以在程序刚开始运行时，先提示用户从键盘输入初始化信息；运行中从键盘接受小火车的控制信号。以A车示例，可以设定初始化信息的输入顺序如下：

    [A Speed]	 //A运行速度
    [A StartTime]   //启动时间，0表示即刻启动
    [A StartPoint]	 //A启动位置（例如将A轨道分为0-12点刻度，启动位置在0点）
    [A InPoint]	 //A进入公共轨道探测点的位置,可以是绝对坐标，也可以是相对公共轨道的距离
    [A OutPoint]	 //A驶出公共轨道探测点位置,可以是绝对坐标，也可以是相对公共轨道的距离
    [A Type]        //火车类型，1快车，2慢车；也可根据火车速度规定快慢判别区间

开始运行后，以A车示例，可以设定控制信号的按键如下：

A车暂停：‘PA’

A车继续：‘CA’
    
b．图形界面输入。程序运行时，先提示是否修改默认配置，若修改则首先出现初始化信息录入界面，在文本框中输入全部的初始化信息。然后程序根据这些参数绘制主画面。在主画面中，鼠标第一次点击小火车时，小火车启动运行，再次点击小火车时，火车暂停。火车申请进入公共轨道时，探测点点亮。

###（二）输出：
a．命令行输出：

两种方案：（1）周期性在命令行窗口打印出各辆火车的状态信息，包括：运行到轨道的哪个位置、状态（运行-非公共轨道，运行-公共轨道、暂停-等待公共轨道、停靠-占用公共轨道）。（2）当小火车状态发生变化时，如刚启动时，或从暂停变为运行，或申请进入公共轨道，获准进入时，在命令行窗口打印出小火车当前新的位置、暂停还是运行、是否等候进入、进入/驶出等。

b．动画显示：在窗口中显示小火车在绘制的轨道中匀速运行的动画，显示每段公共轨道的四个探测点，显示调度控制面板。为了直观显示调度的结果，当火车被允许进入公共轨道时，有绿色信号灯点亮；若调度错误，导致两车在轨道上相撞，要模拟撞车效果。

c．火车运行情况的记录（结果）文件。

记录文件也是文本文件，每一行表示一个时刻的火车运行状态，包括以下内容：

当前时间：程序开始运行的系统时钟时间，单位秒。

火车运行方向：顺时针、逆时针。

火车当前位置：0.0-12.0。假如将A轨道分为0-12点刻度，当前位置在几点。因有多条轨道，还需有字符表示某辆火车当前处于那个轨道。

火车当前状态：运行-非公共轨道，运行-公共轨道、暂停-等待公共轨道、停靠-占用公共轨道。

每个数据项之间用一个空格隔开。
    
## 六．自由选择的高级功能
1.	同一轨道上车辆数目可大于1，火车运行速度可变，非匀速行驶，保障同一轨道多辆火车不能相撞。
2.	在火车运行轨道上设置若干停靠站点，不同的站点可能有不同的停靠时间。在动画输出时，要能显示出剩余停靠时间。
3.	火车可以不局限于特定轨道，可以在多条不同轨道上行驶，可以设计多个站台，给火车设定必须经过的站台路线，中央调度算法根据任务计算火车行驶路线，同时保障火车间不能相撞。
4.	火车轨道图形化定制。
5.	可根据自己思考，设计增加其它的功能和相应的参数。

注: 命令行方式下火车坐标不局限于类似钟表的12点刻度表示方式，也可思考采用其它更细化、精确的方法。