//------------------------------------------------------------------------------//
//          北京飞易来科技有限公司                                              //
//    硬件模拟键盘鼠标模块应用编程接口(API)说明                                 //
//                                                                              //
//    本API适用于                                                               //
//	  1. 双头012WU盒子主控端                                                    //
//    2. Linux2.6.16操作系统                                                    //
//                                                                              //
//	  详细请登陆北京飞易来科技有限公司官方网站http://freeeasy-life.com          //
//------------------------------------------------------------------------------//
/*
 * Copyright 2014, freeeasy-life.com
 * Released under the MIT License.
 *
 */
#ifndef  _MSDK_H
#define  _MSDK_H

#include "hidcomm.h"
#include "KeyCode.h"

typedef void *HANDLE;
typedef struct tag_udev
{
   libusb_device_handle *hDev;
   unsigned char    CurrKbOp[10];       //当前Keyboard的操作值
   unsigned char    CurrMouseOp[5];     //当前mouse的操作值,如果是移动，则是移动后要clear xy值
   int     CurrMouseAbsPosX;   //当前mouse的绝对坐标
   int     CurrMouseAbsPosY;   //当前mouse的绝对坐标
   unsigned char    Mouse1stMovto;      //mouse是否执行过移动到绝对坐标的操作;记录第一次
   int     Cx;                 //绝对移动时，被控机的分辨率x
   int     Cy;                 //绝对移动时，被控机的分辨率y
}UDEVHANDLE,*PUDEVHANDLE;

#define VID 0xC216
#define PID 0x0102
/***********设备操作函数;以下函数中的m_hdl是指M_Open返回的句柄*******/
//打开端口获取句柄;  
//Nbr是端口号，无论是双头模块还是单头模块，都是从1开始，依次为2/3/4...，最大126。电脑上插入一个就始终是1；插入n个 端口分别是1、2....n
HANDLE M_Open(int Nbr);
//关闭端口；在程序退出前再关闭端口; 返回 0: 成功；!0: 失败
int M_Close(HANDLE m_hdl);

/***********键盘操作函数;以下函数中的m_hdl是指M_Open返回的句柄*******/
/***********以下所有命令返回 0: 成功；-1: 失败*******/
///////////以下接口采用的HidKeyCode是USB键盘的KeyCode，不是windows的KeyCode，新开发程序不建议再使用
//单击(按下后立刻弹起)按键  //HidKeyCode: 键盘码; Nbr: 按下次数
int M_KeyPress(HANDLE m_hdl, int HidKeyCode, int Nbr);
//按下某个按键不弹起        //HidKeyCode: 键盘码
int M_KeyDown(HANDLE m_hdl, int HidKeyCode);
//弹起某个按键              //HidKeyCode: 键盘码
int M_KeyUp(HANDLE m_hdl, int HidKeyCode);
//读取按键状态              //HidKeyCode: 键盘码 //返回 0: 弹起状态；1:按下状态；-1: 失败(端口未打开)
//使用该接口，不允许手工操作键盘，否则该接口返回值有可能不正确
int M_KeyState(HANDLE m_hdl, int HidKeyCode);
///////////以上接口采用的HidKeyCode是USB键盘的KeyCode，不是windows的KeyCode，新开发程序不建议再使用
///////////以下接口采用的KeyCode是windows的KeyCode，新开发程序建议再使用
//单击(按下后立刻弹起)按键  //KeyCode: 键盘码; Nbr: 按下次数
int M_KeyPress2(HANDLE m_hdl, int KeyCode, int Nbr);
//按下某个按键不弹起        //KeyCode: 键盘码
int M_KeyDown2(HANDLE m_hdl, int KeyCode);
//弹起某个按键              //KeyCode: 键盘码
int M_KeyUp2(HANDLE m_hdl, int KeyCode);
//读取按键状态              //KeyCode: 键盘码 //返回 0: 弹起状态；1:按下状态；-1: 失败(端口未打开)
//使用该接口，不允许手工操作键盘，否则该接口返回值有可能不正确
int M_KeyState2(HANDLE m_hdl, int KeyCode);
///////////以上接口采用的KeyCode是windows的KeyCode，新开发程序建议再使用
//弹起所有按键
int M_ReleaseAllKey(HANDLE m_hdl);
//读取小键盘NumLock灯的状态 //返回 0:灭；1:亮；-1: 失败
int M_NumLockLedState(HANDLE m_hdl);
//读取CapsLock灯的状态 //返回 0:灭；1:亮；-1: 失败
int M_CapsLockLedState(HANDLE m_hdl);
//读取ScrollLock灯的状态 //返回 0:灭；1:亮；-1: 失败
int M_ScrollLockLedState(HANDLE m_hdl);
/***********鼠标操作函数;以下函数中的m_hdl是指M_Open返回的句柄*******/
/***********以下所有命令返回 0: 成功；-1: 失败*******/
//左键单击   //Nbr: 左键在当前位置单击次数 
int M_LeftClick(HANDLE m_hdl,int Nbr);
//左键双击   //Nbr: 左键在当前位置双击次数
int M_LeftDoubleClick(HANDLE m_hdl,int Nbr);
//按下左键不弹起
int M_LeftDown(HANDLE m_hdl);
//弹起左键
int M_LeftUp(HANDLE m_hdl);
//右键单击   //Nbr: 左键在当前位置单击次数
int M_RightClick(HANDLE m_hdl,int Nbr);
//按下右键不弹起
int M_RightDown(HANDLE m_hdl);
//弹起右键
int M_RightUp(HANDLE m_hdl);
//中键单击   //Nbr: 左键在当前位置单击次数
int M_MiddleClick(HANDLE m_hdl,int Nbr);
//按下中键不弹起
int M_MiddleDown(HANDLE m_hdl);
//弹起中键
int M_MiddleUp(HANDLE m_hdl);
//弹起鼠标的所有按键
int M_ReleaseAllMouse(HANDLE m_hdl);
//滚动鼠标滚轮;  Nbr: 滚动量,  为正,向上滚动；为负, 向下滚动;
int M_MouseWheel(HANDLE m_hdl,int Nbr);
//将鼠标移动到原点(0,0)  在出现移动出现异常时，可以用该函数将鼠标复位
int M_ResetMousePos(HANDLE m_hdl);
//从当前位置移动鼠标    x: x方向（横轴）的距离（正:向右; 负值:向左）; y: y方向（纵轴）的距离（正:向下; 负值:向上）
int M_MoveR(HANDLE m_hdl,int x, int y);
//移动鼠标到指定坐标    x: x方向（横轴）的坐标; y: y方向（纵轴）的坐标。坐标原点(0, 0)在屏幕左上角
//注意：如果出现过将鼠标移动的距离超过屏幕大小，再次MoveTo可能会出现无法正确移动到指定坐标的问题，如果出现该问题，需调用ResetMousePos复位
int M_MoveTo(HANDLE m_hdl,int x, int y);
//读取当前鼠标所在坐标  返回坐标在x、y中。 
//注意：该函数必须在执行一次MoveTo或ResetMousePos函数后才能正确执行！
//注意：如果曾经出现过将鼠标移动的距离超过屏幕大小，这里读取到的坐标值有可能是不正确的！如果出现该问题，需调用ResetMousePos复位
int M_GetCurrMousePos(HANDLE m_hdl,int *x, int *y);

//以下接口仅适用主控机和被控机是同一台电脑的使用方式(单头模块；双头模块的两个USB头都连接到同一台电脑)
//以下接口将调用系统的API来获取当前鼠标位置，DLL将不记录鼠标移动的位置
//移动鼠标到指定坐标    x: x方向（横轴）的坐标; y: y方向（纵轴）的坐标。
int M_MoveR2(HANDLE m_hdl,int x, int y);
//移动鼠标到指定坐标    x: x方向（横轴）的坐标; y: y方向（纵轴）的坐标。坐标原点(0, 0)在屏幕左上角
int M_MoveTo2(HANDLE m_hdl,int x, int y);
//读取当前鼠标所在坐标  返回坐标在x、y中。
int M_GetCurrMousePos2(int *x, int *y);

//以下接口将使用绝对移动功能。该功能目前还不能支持安卓
//在使用绝对移动功能前，必须先输入被控机的屏幕分辨率
//x: x方向（横轴）的坐标; y: y方向（纵轴）的坐标。坐标原点(0, 0)在屏幕左上角
int M_ResolutionUsed(HANDLE m_hdl, int x, int y);
//将鼠标移动到指定坐标。绝对移动功能，鼠标移动到指定位置时，在某些坐标上最大会有±2的误差
//使用该接口后，可以调用M_GetCurrMousePos读取鼠标位置
int M_MoveTo3(HANDLE m_hdl, int x, int y);
/*******************通用操作函数****************************/
//延时指定时间  time:单位ms
int M_Delay(int time);
//在指定的最小最大值之间延时随机时间  Min_time:最小延时时间; Max_time: 最大延时时间 （单位：ms）
int M_DelayRandom(int Min_time, int Max_time);
//在最小最大值之间取随机数
int M_RandDomNbr(int Min_V, int Max_V);

#endif