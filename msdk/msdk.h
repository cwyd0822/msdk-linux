//------------------------------------------------------------------------------//
//          �����������Ƽ����޹�˾                                              //
//    Ӳ��ģ��������ģ��Ӧ�ñ�̽ӿ�(API)˵��                                 //
//                                                                              //
//    ��API������                                                               //
//	  1. ˫ͷ012WU�������ض�                                                    //
//    2. Linux2.6.16����ϵͳ                                                    //
//                                                                              //
//	  ��ϸ���½�����������Ƽ����޹�˾�ٷ���վhttp://freeeasy-life.com          //
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
   unsigned char    CurrKbOp[10];       //��ǰKeyboard�Ĳ���ֵ
   unsigned char    CurrMouseOp[5];     //��ǰmouse�Ĳ���ֵ,������ƶ��������ƶ���Ҫclear xyֵ
   int     CurrMouseAbsPosX;   //��ǰmouse�ľ�������
   int     CurrMouseAbsPosY;   //��ǰmouse�ľ�������
   unsigned char    Mouse1stMovto;      //mouse�Ƿ�ִ�й��ƶ�����������Ĳ���;��¼��һ��
   int     Cx;                 //�����ƶ�ʱ�����ػ��ķֱ���x
   int     Cy;                 //�����ƶ�ʱ�����ػ��ķֱ���y
}UDEVHANDLE,*PUDEVHANDLE;

#define VID 0xC216
#define PID 0x0102
/***********�豸��������;���º����е�m_hdl��ָM_Open���صľ��*******/
//�򿪶˿ڻ�ȡ���;  
//Nbr�Ƕ˿ںţ�������˫ͷģ�黹�ǵ�ͷģ�飬���Ǵ�1��ʼ������Ϊ2/3/4...�����126�������ϲ���һ����ʼ����1������n�� �˿ڷֱ���1��2....n
HANDLE M_Open(int Nbr);
//�رն˿ڣ��ڳ����˳�ǰ�ٹرն˿�; ���� 0: �ɹ���!0: ʧ��
int M_Close(HANDLE m_hdl);

/***********���̲�������;���º����е�m_hdl��ָM_Open���صľ��*******/
/***********������������� 0: �ɹ���-1: ʧ��*******/
///////////���½ӿڲ��õ�HidKeyCode��USB���̵�KeyCode������windows��KeyCode���¿������򲻽�����ʹ��
//����(���º����̵���)����  //HidKeyCode: ������; Nbr: ���´���
int M_KeyPress(HANDLE m_hdl, int HidKeyCode, int Nbr);
//����ĳ������������        //HidKeyCode: ������
int M_KeyDown(HANDLE m_hdl, int HidKeyCode);
//����ĳ������              //HidKeyCode: ������
int M_KeyUp(HANDLE m_hdl, int HidKeyCode);
//��ȡ����״̬              //HidKeyCode: ������ //���� 0: ����״̬��1:����״̬��-1: ʧ��(�˿�δ��)
//ʹ�øýӿڣ��������ֹ��������̣�����ýӿڷ���ֵ�п��ܲ���ȷ
int M_KeyState(HANDLE m_hdl, int HidKeyCode);
///////////���Ͻӿڲ��õ�HidKeyCode��USB���̵�KeyCode������windows��KeyCode���¿������򲻽�����ʹ��
///////////���½ӿڲ��õ�KeyCode��windows��KeyCode���¿�����������ʹ��
//����(���º����̵���)����  //KeyCode: ������; Nbr: ���´���
int M_KeyPress2(HANDLE m_hdl, int KeyCode, int Nbr);
//����ĳ������������        //KeyCode: ������
int M_KeyDown2(HANDLE m_hdl, int KeyCode);
//����ĳ������              //KeyCode: ������
int M_KeyUp2(HANDLE m_hdl, int KeyCode);
//��ȡ����״̬              //KeyCode: ������ //���� 0: ����״̬��1:����״̬��-1: ʧ��(�˿�δ��)
//ʹ�øýӿڣ��������ֹ��������̣�����ýӿڷ���ֵ�п��ܲ���ȷ
int M_KeyState2(HANDLE m_hdl, int KeyCode);
///////////���Ͻӿڲ��õ�KeyCode��windows��KeyCode���¿�����������ʹ��
//�������а���
int M_ReleaseAllKey(HANDLE m_hdl);
//��ȡС����NumLock�Ƶ�״̬ //���� 0:��1:����-1: ʧ��
int M_NumLockLedState(HANDLE m_hdl);
//��ȡCapsLock�Ƶ�״̬ //���� 0:��1:����-1: ʧ��
int M_CapsLockLedState(HANDLE m_hdl);
//��ȡScrollLock�Ƶ�״̬ //���� 0:��1:����-1: ʧ��
int M_ScrollLockLedState(HANDLE m_hdl);
/***********����������;���º����е�m_hdl��ָM_Open���صľ��*******/
/***********������������� 0: �ɹ���-1: ʧ��*******/
//�������   //Nbr: ����ڵ�ǰλ�õ������� 
int M_LeftClick(HANDLE m_hdl,int Nbr);
//���˫��   //Nbr: ����ڵ�ǰλ��˫������
int M_LeftDoubleClick(HANDLE m_hdl,int Nbr);
//�������������
int M_LeftDown(HANDLE m_hdl);
//�������
int M_LeftUp(HANDLE m_hdl);
//�Ҽ�����   //Nbr: ����ڵ�ǰλ�õ�������
int M_RightClick(HANDLE m_hdl,int Nbr);
//�����Ҽ�������
int M_RightDown(HANDLE m_hdl);
//�����Ҽ�
int M_RightUp(HANDLE m_hdl);
//�м�����   //Nbr: ����ڵ�ǰλ�õ�������
int M_MiddleClick(HANDLE m_hdl,int Nbr);
//�����м�������
int M_MiddleDown(HANDLE m_hdl);
//�����м�
int M_MiddleUp(HANDLE m_hdl);
//�����������а���
int M_ReleaseAllMouse(HANDLE m_hdl);
//����������;  Nbr: ������,  Ϊ��,���Ϲ�����Ϊ��, ���¹���;
int M_MouseWheel(HANDLE m_hdl,int Nbr);
//������ƶ���ԭ��(0,0)  �ڳ����ƶ������쳣ʱ�������øú�������긴λ
int M_ResetMousePos(HANDLE m_hdl);
//�ӵ�ǰλ���ƶ����    x: x���򣨺��ᣩ�ľ��루��:����; ��ֵ:����; y: y�������ᣩ�ľ��루��:����; ��ֵ:���ϣ�
int M_MoveR(HANDLE m_hdl,int x, int y);
//�ƶ���굽ָ������    x: x���򣨺��ᣩ������; y: y�������ᣩ�����ꡣ����ԭ��(0, 0)����Ļ���Ͻ�
//ע�⣺������ֹ�������ƶ��ľ��볬����Ļ��С���ٴ�MoveTo���ܻ�����޷���ȷ�ƶ���ָ����������⣬������ָ����⣬�����ResetMousePos��λ
int M_MoveTo(HANDLE m_hdl,int x, int y);
//��ȡ��ǰ�����������  ����������x��y�С� 
//ע�⣺�ú���������ִ��һ��MoveTo��ResetMousePos�����������ȷִ�У�
//ע�⣺����������ֹ�������ƶ��ľ��볬����Ļ��С�������ȡ��������ֵ�п����ǲ���ȷ�ģ�������ָ����⣬�����ResetMousePos��λ
int M_GetCurrMousePos(HANDLE m_hdl,int *x, int *y);

//���½ӿڽ��������ػ��ͱ��ػ���ͬһ̨���Ե�ʹ�÷�ʽ(��ͷģ�飻˫ͷģ�������USBͷ�����ӵ�ͬһ̨����)
//���½ӿڽ�����ϵͳ��API����ȡ��ǰ���λ�ã�DLL������¼����ƶ���λ��
//�ƶ���굽ָ������    x: x���򣨺��ᣩ������; y: y�������ᣩ�����ꡣ
int M_MoveR2(HANDLE m_hdl,int x, int y);
//�ƶ���굽ָ������    x: x���򣨺��ᣩ������; y: y�������ᣩ�����ꡣ����ԭ��(0, 0)����Ļ���Ͻ�
int M_MoveTo2(HANDLE m_hdl,int x, int y);
//��ȡ��ǰ�����������  ����������x��y�С�
int M_GetCurrMousePos2(int *x, int *y);

//���½ӿڽ�ʹ�þ����ƶ����ܡ��ù���Ŀǰ������֧�ְ�׿
//��ʹ�þ����ƶ�����ǰ�����������뱻�ػ�����Ļ�ֱ���
//x: x���򣨺��ᣩ������; y: y�������ᣩ�����ꡣ����ԭ��(0, 0)����Ļ���Ͻ�
int M_ResolutionUsed(HANDLE m_hdl, int x, int y);
//������ƶ���ָ�����ꡣ�����ƶ����ܣ�����ƶ���ָ��λ��ʱ����ĳЩ�����������С�2�����
//ʹ�øýӿں󣬿��Ե���M_GetCurrMousePos��ȡ���λ��
int M_MoveTo3(HANDLE m_hdl, int x, int y);
/*******************ͨ�ò�������****************************/
//��ʱָ��ʱ��  time:��λms
int M_Delay(int time);
//��ָ������С���ֵ֮����ʱ���ʱ��  Min_time:��С��ʱʱ��; Max_time: �����ʱʱ�� ����λ��ms��
int M_DelayRandom(int Min_time, int Max_time);
//����С���ֵ֮��ȡ�����
int M_RandDomNbr(int Min_V, int Max_V);

#endif