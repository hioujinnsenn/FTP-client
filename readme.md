### FTP客户端设计

许敏章，叶茂鑫



### 1.环境配置

基于Windows、QT、C++、Filezilla

Qt只负责界面图形化，未使用到Qt内置的网络设计。

使用Windows系统的winsock.h作为网络的API，从socket开始握手。

C++作为连接QT和Windows的socket库的语言，

Filezilla作为FTP的服务器端，放置在本地，以127.0.0.1作为地址。



### 2.基于FTP协议的命令设计

此处参考了IBM关于

https://www.ibm.com/developerworks/cn/linux/l-cn-socketftp/

使用socket设计FTP的通信细节

设想中是这样的如果基于ftp设计的很顺利，还可以考虑增加关于sftp的设计，**暂定**。



### 3.第一步，安装FTP服务器增加FTP用户

此处设计的FTP，使用filezilla，增加了一个ftp用户jh，密码对应为1233456



### 4.第二步，进行FTP登录认证

此处已经在login文件夹得login里解决了。

需要的参数：username，password



### 5.第三步，建立一个数据端口，获取服务器的文件夹信息才能够进行数据传输

此处暂定使用被动模式，直接基于IBM给的设计，也可以主动模式，只是**暂定**。

进入被动模式的命令是PASV。

另外在utils文件里，设计了关于命令的简化版本SendCommand，简化了对char*内存的操作。



### 6.第四步，