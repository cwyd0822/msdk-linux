# msdk-linux
飞易来键鼠模拟器

## 请注意：
1. 目前的接口(msdk.c、hidcomm.c)仅支持一个双头盒子，如果需要支持多个，请自行参考程序修改。
2. 目前的linux接口代码不支持单头盒子，只支持双头盒子
3. linux的msdk接口代码没有完全移植windows下的msdk.dll的所有接口。
   某些特殊接口代码没有移植过来，如取设备序列号、设置用户数据等接口。


## 对Linux开发，我们不提供支持！

请注意，现在的例程是32位的！！！ 如果要在64位linux下开发，可能需要做一下改动，我们暂不提供支持，请自行研究！！

双头盒子主控端的Linux接口代码在msdk目录下，demo是例程, msdk.c、hidcomm.c是接口代码。

以下将以ubuntu为例，说明如何在linux开发：
1. 安装libusb
  - a 进入msdkLinux目录， ./configure
  - b make
  - c make install
  - d 到这，已经完成安装libusb
      - libusb.h 在/usr/local/include/libusb-1.0
      - libusb.so在/usr/local/lib

2. 编译demo
  - a 进入msdk目录
  - b ./build.sh  生成a.out

3. 测试：
  - a 将双头盒子主控端（有孔或印有“主”字的一端）插入电脑，将被控端插到被控机。
  
      被控机上打开一个记事本，并将光标点到记事本里
  - b 如果您用的是虚拟机，请将该主控端映射入虚拟机中
  - c 在/dev/bus/usb/ 中找到新增加的双头盒子，比如 /dev/bus/usb/001/003
  
      hexdump查看该003文件，里面可以查看到盒子对应的VID(C216或C217)、PID(0102)
	  
      修改权限：sudo chmod 666 /dev/bus/usb/001/003
	  
  - d 在msdk目录下，运行 ./a.out
  - e 等待5s，被控机上的鼠标将有移动，记事本里将会出现10个a。

## 您可以参考demo.c开发自己的程序

