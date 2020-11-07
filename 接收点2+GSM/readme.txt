
===========================================================================================
BroadcastCommunication实验
===========================================================================================
本实验的目的：
1、在ZX2530A 型CC2530 节点板上运行自己的程序。 
2、理解广播的实现方式。

===========================================================================================
实验步骤：
1. 准备3个CC2530射频板，参考实验指导书1.2章节，将无线节点板跳线设置为模式一，分别接上出厂电源； 
2. 先将main.c中的节点类型变量NODE_TYPE的值设置为0作为接收节点，然后选择Project->Rebuild All重新编译工程。
3. 将仿真器连接到其中一个CC2530节点板，上电CC2530节点板，然后点击菜单Project->Download and debug下载程序到节点板。此节点以下称为接收节点1。
4．修改main.c中的节点短地址RECV_ADDR的值为0x2510，保存，然后选择Project->Rebuild All重新编译工程。接下来通过仿真器把程序下载到另外一个CC2530节点板中，称为接收节点2；  
5. 将节点类型变量NODE_TYPE的值设置为1，保存。然后选择Project->Rebuild All重新编译工程编译工程，并下载到ZX2530A 型CC2530 节点板中，作为发送节点。 
6. 将发送节点通过串口线连接到 PC上，在PC机上打开串口调试助手，配置串口助手波特率为19200。 
7. 复位发送节点（让节点发送数据），可以看到串口调试助手上打印出发送情况。
8．将接收节点1和接收接点2上电，依次通过串口线连接到PC 上，，可以看到串口调试助手上打印出接收的数据。


以下是程序正确运行之后终端上显示的结果：
发送节点数据：
Send:Hello cc2530
Send:Hello cc2530
Send:Hello cc2530
Send:Hello cc2530
Send:Hello cc2530
Send:Hello cc2530
_

不同接收节点数据：
My Address 9504 , recv:Hello cc2530
My Address 9504 , recv:Hello cc2530
My Address 9504 , recv:Hello cc2530
My Address 9504 , recv:Hello cc2530
My Address 9504 , recv:Hello cc2530
My Address 9504 , recv:Hello cc2530
My Address 9488 , recv:Hello cc2530
My Address 9488 , recv:Hello cc2530
My Address 9488 , recv:Hello cc2530
My Address 9488 , recv:Hello cc2530
My Address 9488 , recv:Hello cc2530
_

