#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include "msdk.h"
#include "AjjlKeyName.h"

/*******************�򿪹رպ���****************************/
//�򿪶˿ڻ�ȡ���;
//Nbr�Ǵ򿪵Ķ˿ںţ���1��ʼ������Ϊ2/3/4...�����126
//���������ֻ����һ��˫ͷģ�飬��򿪶˿ں�Ϊ1
HANDLE M_Open(int Nbr)
{
	libusb_device_handle *LibDev;
	HANDLE m_hdl;

    if ((Nbr < 1) || (Nbr > 126)) {
        return NULL;
    }
    LibDev = HID_Open(VID, PID);
    if (LibDev != NULL) {
        m_hdl = (malloc(sizeof(UDEVHANDLE)));
        memset((PUDEVHANDLE)m_hdl, 0, sizeof(UDEVHANDLE));
        ((PUDEVHANDLE)m_hdl)->hDev = LibDev;
    }
	return m_hdl;
}

int M_Close(HANDLE m_hdl)
{
    PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;
	if (HID_Close(l_hUdev->hDev) == 0) {
	    free(l_hUdev);
	    return 0;
	}
	return -1;
}

/***********���̲�������;���º����е�m_hdl��ָM_Open���صľ��*******/
//����(���º����̵���)����  //HidKeyCode: ������; Nbr: ���´���
int M_KeyPress(HANDLE m_hdl, int HidKeyCode, int Nbr)
{
    if(m_hdl == NULL) {
        return -1;
    }
    while (Nbr) {
        if (M_KeyDown(m_hdl, HidKeyCode) != 0) {
            return -1;
        }
        M_DelayRandom(50, 80);
        if (M_KeyUp(m_hdl, HidKeyCode) != 0) {
            return -1;
        }
        Nbr--;
        if (Nbr) {
            M_DelayRandom(150, 600);
        }
    }
    return 0;
}

//����ĳ������������        //HidKeyCode: ������
int M_KeyDown(HANDLE m_hdl, int HidKeyCode)
{
    if(m_hdl == NULL) {
        return -1;
    }
	PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;
	int KbPos;
	unsigned long dwp_LenResponse;
	unsigned char ucp_Response[32];
	//scan curr HidKeyCode
	if ((HidKeyCode < Keyboard_a) ||
	    (HidKeyCode > Keyboard_RightWindows) ||
	    ((HidKeyCode < Keyboard_LeftControl) && (HidKeyCode > Keyboard_Application)) ) {
        return -1;
	}

	//OutputDebugStringA("KeyDownF");
    l_hUdev->CurrKbOp[2] = 0;
	if ( (HidKeyCode >= Keyboard_LeftControl) && (HidKeyCode <= Keyboard_RightWindows)) {//���ǿ�����
		KbPos = HidKeyCode - Keyboard_LeftControl;
		l_hUdev->CurrKbOp[1] |= (1 << KbPos);
	    if (((l_hUdev->CurrKbOp[0] & 0x80) == 0) ||      //bit7 == 0
		    ((l_hUdev->CurrKbOp[0] & 0x7F) > 8)  ||      //err
		    ((l_hUdev->CurrKbOp[0] & 0x7F) < 3)) {       //err
			l_hUdev->CurrKbOp[0] = 0x83;
			l_hUdev->CurrKbOp[3] = 0;
		}
		else {//���ı�
		}
	}
	else {
	    if (((l_hUdev->CurrKbOp[0] & 0x80) == 0) ||      //bit7 == 0
		    ((l_hUdev->CurrKbOp[0] & 0x7F) > 8)  ||      //err
		    ((l_hUdev->CurrKbOp[0] & 0x7F) < 3) ) {      //err
			l_hUdev->CurrKbOp[0] = 0x83;
			l_hUdev->CurrKbOp[1] = 0;
			l_hUdev->CurrKbOp[3] = HidKeyCode;
		}
		else {
		    for (KbPos = 3; KbPos < 9; KbPos++) {//scan [3]~[8]
		        if (l_hUdev->CurrKbOp[KbPos] == 0) {//free position
		            l_hUdev->CurrKbOp[KbPos] = HidKeyCode;
		            l_hUdev->CurrKbOp[0] = 0x80 | KbPos;
		            break;
		        }
		        if (l_hUdev->CurrKbOp[KbPos] == HidKeyCode) {//�ð����Ѿ�����
		            break;
		        }
		    }
		    if (KbPos == 9) {//û���ҵ�free position
		        l_hUdev->CurrKbOp[8] = HidKeyCode; //ǿ��ռ�����һ��position
		        l_hUdev->CurrKbOp[0] = 0x88; //ǿ��ռ�����һ��position
		    }
		}
	}
	if (HID_Ctrl_Comm(l_hUdev->hDev,
		    ((l_hUdev->CurrKbOp[0]) & 0x0F) + 1,
			l_hUdev->CurrKbOp,
			&dwp_LenResponse,
			ucp_Response,
			0) != 0x0001) {
        return -1;
	}

    return 0;
}
//����ĳ������              //HidKeyCode: ������
int M_KeyUp(HANDLE m_hdl, int HidKeyCode)
{
    if(m_hdl == NULL) {
        return -1;
    }
	PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;
	int KbPos;
	unsigned long dwp_LenResponse;
	unsigned char ucp_Response[32];
	//scan curr HidKeyCode
	if ((HidKeyCode < Keyboard_a) ||
	    (HidKeyCode > Keyboard_RightWindows) ||
	    ((HidKeyCode < Keyboard_LeftControl) && (HidKeyCode > Keyboard_Application)) ) {
        return -1;
	}

	//OutputDebugStringA("KeyUpF");
    l_hUdev->CurrKbOp[2] = 0;
	if ( (HidKeyCode >= Keyboard_LeftControl) && (HidKeyCode <= Keyboard_RightWindows)) {//���ǿ�����
		KbPos = HidKeyCode - Keyboard_LeftControl;
		l_hUdev->CurrKbOp[1] &= (~(1 << KbPos));
	    if (((l_hUdev->CurrKbOp[0] & 0x80) == 0) ||      //bit7 == 0; err or M_ReleaseAllKey just now
		    ((l_hUdev->CurrKbOp[0] & 0x7F) > 8)  ||      //err
		    ((l_hUdev->CurrKbOp[0] & 0x7F) < 3)) {       //err
		    memset(l_hUdev->CurrKbOp, 0, 9);
		}
		else {//���ı�
		}
	}
	else {
	    if (((l_hUdev->CurrKbOp[0] & 0x80) == 0) ||      //bit7 == 0
		    ((l_hUdev->CurrKbOp[0] & 0x7F) > 8)  ||      //err
		    ((l_hUdev->CurrKbOp[0] & 0x7F) < 3)) {       //err
		    memset(l_hUdev->CurrKbOp, 0, 9);
		}
		else {
		    int CurrCmdLen = l_hUdev->CurrKbOp[0] & 0x7F;
		    for (KbPos = 3; KbPos <= CurrCmdLen; KbPos++) {//scan [3]~[LenOfCmd]
		        if (l_hUdev->CurrKbOp[KbPos] == HidKeyCode) {//�ҵ��ð���
		            l_hUdev->CurrKbOp[KbPos] = 0;
		            l_hUdev->CurrKbOp[0] = 0x80 | (KbPos - 1);
        		    //������İ����Ƶ�ǰ����
        		    int CurrMovPos;
        		    for (CurrMovPos = KbPos + 1; CurrMovPos <= CurrCmdLen; CurrMovPos++) {
            	        if (l_hUdev->CurrKbOp[CurrMovPos] == 0) {//free position
           	                break;
            	        }
            	        else {//�Ƶ�����ȥ
            	            l_hUdev->CurrKbOp[CurrMovPos - 1] = l_hUdev->CurrKbOp[CurrMovPos];
            	            l_hUdev->CurrKbOp[CurrMovPos] = 0;
            	            l_hUdev->CurrKbOp[0]++;
            	        }
        		    }
    	            if ((l_hUdev->CurrKbOp[0] & 0x7F) < 3) {//ֻʣ�¿�����
    	                l_hUdev->CurrKbOp[0] = 0x83;
    	                l_hUdev->CurrKbOp[3] = 0;
                		if (l_hUdev->CurrKbOp[1] == 0) {
                		    memset(l_hUdev->CurrKbOp, 0, 9);
                		}
    	            }
                    break;
		        }
		        if (l_hUdev->CurrKbOp[KbPos] == 0) {//free position
		            break;
		        }
		    }
		}
	}
	if (HID_Ctrl_Comm(l_hUdev->hDev,
		    ((l_hUdev->CurrKbOp[0]) & 0x0F) + 1,
			l_hUdev->CurrKbOp,
			&dwp_LenResponse,
			ucp_Response,
			0) != 0x0001) {
		//OutputDebugStringA("Er");
        return -1;
	}
	//OutputDebugStringA("KeyUpE");

    return 0;
}
//��ȡ����״̬              //HidKeyCode: ������ //���� 0: ����״̬��1:����״̬��-1: ʧ��(�˿�δ��)
int M_KeyState(HANDLE m_hdl, int HidKeyCode)
{
	int KbPos;
    if(m_hdl == NULL) {
        return -1;
    }
	PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;
    if ((HidKeyCode < Keyboard_a) ||
	    (HidKeyCode > Keyboard_RightWindows) ||
	    ((HidKeyCode < Keyboard_LeftControl) && (HidKeyCode > Keyboard_Application)) ) {
        return -1;
	}
	if ( (HidKeyCode >= Keyboard_LeftControl) && (HidKeyCode <= Keyboard_RightWindows)) {//���ǿ�����
		KbPos = HidKeyCode - Keyboard_LeftControl;
		if (l_hUdev->CurrKbOp[1] & (1 << KbPos)) {
			return 1;
		}
	}
	else {
		int CurrCmdLen = l_hUdev->CurrKbOp[0] & 0x7F;
		for (KbPos = 3; KbPos <= CurrCmdLen; KbPos++) {//scan [3]~[LenOfCmd]
		    if (l_hUdev->CurrKbOp[KbPos] == HidKeyCode) {//�ҵ��ð���
				return 1;
			}
		}
	}
    return 0;
}

//����(���º����̵���)����  //KeyCode: ������; Nbr: ���´���
int M_KeyPress2(HANDLE m_hdl, int KeyCode, int Nbr)
{
    if(m_hdl == NULL) {
        return -1;
    }
    if (KeyCode > 222) {
        return -1;
    }
    return(M_KeyPress(m_hdl, VKEY2HIDCODE[KeyCode], Nbr));
}

//����ĳ������������        //KeyCode: ������
int M_KeyDown2(HANDLE m_hdl, int KeyCode)
{
    if(m_hdl == NULL) {
        return -1;
    }
    if (KeyCode > 222) {
        return -1;
    }
    return(M_KeyDown(m_hdl, VKEY2HIDCODE[KeyCode]));
}
//����ĳ������              //KeyCode: ������
int M_KeyUp2(HANDLE m_hdl, int KeyCode)
{
    if(m_hdl == NULL) {
        return -1;
    }
    if (KeyCode > 222) {
        return -1;
    }
	return(M_KeyUp(m_hdl, VKEY2HIDCODE[KeyCode]));
}
//��ȡ����״̬              //KeyCode: ������ //���� 0: ����״̬��1:����״̬��-1: ʧ��(�˿�δ��)
int M_KeyState2(HANDLE m_hdl, int KeyCode)
{
    if(m_hdl == NULL) {
        return -1;
    }
    if (KeyCode > 222) {
        return -1;
    }
	return(M_KeyState(m_hdl, VKEY2HIDCODE[KeyCode]));

}
//�������а���
int M_ReleaseAllKey(HANDLE m_hdl)
{
    if(m_hdl == NULL) {
        return -1;
    }
	PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;
	unsigned long dwp_LenResponse;
	unsigned char ucp_Response[32];

    memset(l_hUdev->CurrKbOp, 0, 9);
	HID_Ctrl_Comm(l_hUdev->hDev,
		    1,
			l_hUdev->CurrKbOp,
			&dwp_LenResponse,
			ucp_Response,
			0);
    return 0;
}
//��ȡС����NumLock�Ƶ�״̬ //���� 0:��1:����-1: ʧ��
int M_NumLockLedState(HANDLE m_hdl)
{
    if(m_hdl == NULL) {
        return -1;
    }
    PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;
	unsigned long dwp_LenResponse;
	unsigned char ucp_Response[32], ucp_Command[32];

    ucp_Command[0] = 0xF7;
	if (HID_Ctrl_Comm(l_hUdev->hDev,
		    1,
			ucp_Command,
			&dwp_LenResponse,
			ucp_Response,
			3) != 0x0001) {

        return -1;
	}
	if (ucp_Response[0] & 0x01) {
	    return 1;
	}
	else {
        return 0;
    }
}
//��ȡCapsLock�Ƶ�״̬ //���� 0:��1:����-1: ʧ��
int M_CapsLockLedState(HANDLE m_hdl)
{
    if(m_hdl == NULL) {
        return -1;
    }
    PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;
	unsigned long dwp_LenResponse;
	unsigned char ucp_Response[32], ucp_Command[32];

    ucp_Command[0] = 0xF7;
	if (HID_Ctrl_Comm(l_hUdev->hDev,
		    1,
			ucp_Command,
			&dwp_LenResponse,
			ucp_Response,
			3) != 0x0001) {

        return -1;
	}
	if (ucp_Response[0] & 0x02) {
	    return 1;
	}
	else {
        return 0;
    }
}
//��ȡScrollLock�Ƶ�״̬ //���� 0:��1:����-1: ʧ��
int M_ScrollLockLedState(HANDLE m_hdl)
{
    if(m_hdl == NULL) {
        return -1;
    }
    PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;
	unsigned long dwp_LenResponse;
	unsigned char ucp_Response[32], ucp_Command[32];

    ucp_Command[0] = 0xF7;
	if (HID_Ctrl_Comm(l_hUdev->hDev,
		    1,
			ucp_Command,
			&dwp_LenResponse,
			ucp_Response,
			3) != 0x0001) {
        return -1;
	}

	if (ucp_Response[0] & 0x04) {
	    return 1;
	}
	else {
        return 0;
    }
}
/***********����������;���º����е�m_hdl��ָM_Open���صľ��*******/
//�������   //Nbr: ����ڵ�ǰλ�õ�������
int M_LeftClick(HANDLE m_hdl,int Nbr)
{
    if(m_hdl == NULL) {
        return -1;
    }
    while (Nbr) {
        if (M_LeftDown(m_hdl) != 0) {
            return -1;
        }
        M_DelayRandom(50, 80);
        if (M_LeftUp(m_hdl) != 0) {
            return -1;
        }
        Nbr--;
        if (Nbr) {
            M_DelayRandom(500, 900);
        }
    }
    return 0;
}
//���˫��   //Nbr: ����ڵ�ǰλ��˫������
int M_LeftDoubleClick(HANDLE m_hdl,int Nbr)
{
    if(m_hdl == NULL) {
        return -1;
    }
    while (Nbr) {
        if (M_LeftClick(m_hdl, 1) != 0) {
            return -1;
        }
        M_DelayRandom(60, 110);
        if (M_LeftClick(m_hdl, 1) != 0) {
            return -1;
        }
        Nbr--;
        if (Nbr) {
            M_DelayRandom(500, 800);
        }
    }
    return 0;
}
//�������������
int M_LeftDown(HANDLE m_hdl)
{
    if(m_hdl == NULL) {
        return -1;
    }
	PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;
	unsigned long dwp_LenResponse;
	unsigned char ucp_Response[32];

    l_hUdev->CurrMouseOp[0] = 0x04;
    l_hUdev->CurrMouseOp[2] = 0;
    l_hUdev->CurrMouseOp[3] = 0;
    l_hUdev->CurrMouseOp[4] = 0;
    l_hUdev->CurrMouseOp[1] |= 0x01;
	if (HID_Ctrl_Comm(l_hUdev->hDev,
		    5,
			l_hUdev->CurrMouseOp,
			&dwp_LenResponse,
			ucp_Response,
			0) != 0x0001) {
        return -1;
	}

    return 0;
}
//�������
int M_LeftUp(HANDLE m_hdl)
{
    if(m_hdl == NULL) {
        return -1;
    }
	PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;
	unsigned long dwp_LenResponse;
	unsigned char ucp_Response[32];

    l_hUdev->CurrMouseOp[0] = 0x04;
    l_hUdev->CurrMouseOp[2] = 0;
    l_hUdev->CurrMouseOp[3] = 0;
    l_hUdev->CurrMouseOp[4] = 0;
    l_hUdev->CurrMouseOp[1] &= (~0x01);
	if (HID_Ctrl_Comm(l_hUdev->hDev,
		    5,
			l_hUdev->CurrMouseOp,
			&dwp_LenResponse,
			ucp_Response,
			0) != 0x0001) {

        return -1;
	}

    return 0;
}
//�Ҽ�����   //Nbr: ����ڵ�ǰλ�õ�������
int M_RightClick(HANDLE m_hdl,int Nbr)
{
    if(m_hdl == NULL) {
        return -1;
    }
    while (Nbr) {
        if (M_RightDown(m_hdl) != 0) {
            return -1;
        }
        M_DelayRandom(50, 80);
        if (M_RightUp(m_hdl) != 0) {
            return -1;
        }
        Nbr--;
        if (Nbr) {
            M_DelayRandom(500, 900);
        }
    }
    return 0;
}
//�����Ҽ�������
int M_RightDown(HANDLE m_hdl)
{
    if(m_hdl == NULL) {
        return -1;
    }
	PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;
	unsigned long dwp_LenResponse;
	unsigned char ucp_Response[32];

    l_hUdev->CurrMouseOp[0] = 0x04;
    l_hUdev->CurrMouseOp[2] = 0;
    l_hUdev->CurrMouseOp[3] = 0;
    l_hUdev->CurrMouseOp[4] = 0;
    l_hUdev->CurrMouseOp[1] |= 0x02;
	if (HID_Ctrl_Comm(l_hUdev->hDev,
		    5,
			l_hUdev->CurrMouseOp,
			&dwp_LenResponse,
			ucp_Response,
			0) != 0x0001) {
        return -1;
	}

    return 0;
}
//�����Ҽ�
int M_RightUp(HANDLE m_hdl)
{
    if(m_hdl == NULL) {
        return -1;
    }
	PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;
	unsigned long dwp_LenResponse;
	unsigned char ucp_Response[32];

    l_hUdev->CurrMouseOp[0] = 0x04;
    l_hUdev->CurrMouseOp[2] = 0;
    l_hUdev->CurrMouseOp[3] = 0;
    l_hUdev->CurrMouseOp[4] = 0;
    l_hUdev->CurrMouseOp[1] &= (~0x02);
	if (HID_Ctrl_Comm(l_hUdev->hDev,
		    5,
			l_hUdev->CurrMouseOp,
			&dwp_LenResponse,
			ucp_Response,
			0) != 0x0001) {
        return -1;
	}

    return 0;
}
//�м�����   //Nbr: ����ڵ�ǰλ�õ�������
int M_MiddleClick(HANDLE m_hdl,int Nbr)
{
    if(m_hdl == NULL) {
        return -1;
    }
    while (Nbr) {
        if (M_MiddleDown(m_hdl) != 0) {
            return -1;
        }
        M_DelayRandom(50, 80);
        if (M_MiddleUp(m_hdl) != 0) {
            return -1;
        }
        Nbr--;
        if (Nbr) {
            M_DelayRandom(500, 900);
        }
    }
    return 0;
}
//�����м�������
int M_MiddleDown(HANDLE m_hdl)
{
    if(m_hdl == NULL) {
        return -1;
    }
	PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;
	unsigned long dwp_LenResponse;
	unsigned char ucp_Response[32];

    l_hUdev->CurrMouseOp[0] = 0x04;
    l_hUdev->CurrMouseOp[2] = 0;
    l_hUdev->CurrMouseOp[3] = 0;
    l_hUdev->CurrMouseOp[4] = 0;
    l_hUdev->CurrMouseOp[1] |= 0x04;
	if (HID_Ctrl_Comm(l_hUdev->hDev,
		    5,
			l_hUdev->CurrMouseOp,
			&dwp_LenResponse,
			ucp_Response,
			0) != 0x0001) {
        return -1;
	}

    return 0;
}
//�����м�
int M_MiddleUp(HANDLE m_hdl)
{
    if(m_hdl == NULL) {
        return -1;
    }
	PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;
	unsigned long dwp_LenResponse;
	unsigned char ucp_Response[32];


    l_hUdev->CurrMouseOp[0] = 0x04;
    l_hUdev->CurrMouseOp[2] = 0;
    l_hUdev->CurrMouseOp[3] = 0;
    l_hUdev->CurrMouseOp[4] = 0;
    l_hUdev->CurrMouseOp[1] &= (~0x04);
	if (HID_Ctrl_Comm(l_hUdev->hDev,
		    5,
			l_hUdev->CurrMouseOp,
			&dwp_LenResponse,
			ucp_Response,
			0) != 0x0001) {

        return -1;
	}

    return 0;
}
//����������;  Nbr: ������,  Ϊ��,���Ϲ�����Ϊ��, ���¹���;
int M_MouseWheel(HANDLE m_hdl,int Nbr)
{
    if(m_hdl == NULL) {
        return -1;
    }
	PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;
	unsigned long dwp_LenResponse;
	unsigned char ucp_Response[32];


    l_hUdev->CurrMouseOp[0] = 0x04;
    if (Nbr > 0) {
        l_hUdev->CurrMouseOp[4] = 1;
    }
    else {
        l_hUdev->CurrMouseOp[4] = 0xFF;
    }
    unsigned int NbrAbs = abs(Nbr);
    while (NbrAbs) {
    	if (HID_Ctrl_Comm(l_hUdev->hDev,
    		    5,
    			l_hUdev->CurrMouseOp,
    			&dwp_LenResponse,
    			ucp_Response,
    			0) != 0x0001) {
            l_hUdev->CurrMouseOp[2] = 0;
            l_hUdev->CurrMouseOp[3] = 0;
            l_hUdev->CurrMouseOp[4] = 0;

            return -1;
    	}
        NbrAbs--;
    	if (NbrAbs) {
    	    M_DelayRandom(20, 35);
    	}
    }
    l_hUdev->CurrMouseOp[2] = 0;
    l_hUdev->CurrMouseOp[3] = 0;
    l_hUdev->CurrMouseOp[4] = 0;

    return 0;
}
//DLL �ڲ����õ�MoveR Ϊ��ʵ�ֶ��̵߳���
int M_InternalMoveR(HANDLE m_hdl,int x, int y)
{
    if(m_hdl == NULL) {
        return -1;
    }

	PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;
	unsigned long dwp_LenResponse;
	unsigned char ucp_Response[32], luc_Rnd;
	unsigned int lui_Abs;

    if (l_hUdev->Mouse1stMovto == 1) {
        l_hUdev->CurrMouseAbsPosX += x;
        l_hUdev->CurrMouseAbsPosY += y;
		if (l_hUdev->CurrMouseAbsPosX < 0) {
			l_hUdev->CurrMouseAbsPosX = 0;
		}
		if (l_hUdev->CurrMouseAbsPosY < 0) {
			l_hUdev->CurrMouseAbsPosY = 0;
		}
    }
	while (x || y) {
		l_hUdev->CurrMouseOp[0] = 0x04;
		//����Xλ�ƣ�ÿ���ƶ�����rnd
		luc_Rnd = M_RandDomNbr(100, 127);
		lui_Abs = abs(x);
		if (x == 0) {
			l_hUdev->CurrMouseOp[2] = 0;
		}
		else if (x > 0) {
			if (luc_Rnd > lui_Abs) {
				l_hUdev->CurrMouseOp[2] = x;
			}
			else {
				l_hUdev->CurrMouseOp[2] = luc_Rnd;
			}
		}
		else {//x < 0
			if (luc_Rnd > lui_Abs) {
				l_hUdev->CurrMouseOp[2] = x;
			}
			else {
				l_hUdev->CurrMouseOp[2] = 0x100 - luc_Rnd;
			}
		}
		x -= (signed char)(l_hUdev->CurrMouseOp[2]);
		//����Yλ�ƣ�ÿ���ƶ�����rnd
		luc_Rnd = M_RandDomNbr(100, 127);
		lui_Abs = abs(y);
		if (y == 0) {
			l_hUdev->CurrMouseOp[3] = 0;
		}
		else if (y > 0) {
			if (luc_Rnd > lui_Abs) {
				l_hUdev->CurrMouseOp[3] = y;
			}
			else {
				l_hUdev->CurrMouseOp[3] = luc_Rnd;
			}
		}
		else {//x < 0
			if (luc_Rnd > lui_Abs) {
				l_hUdev->CurrMouseOp[3] = y;
			}
			else {
				l_hUdev->CurrMouseOp[3] = 0x100 - luc_Rnd;
			}
		}
		y -= (signed char)(l_hUdev->CurrMouseOp[3]);
		if (HID_Ctrl_Comm(l_hUdev->hDev,
				5,
				l_hUdev->CurrMouseOp,
				&dwp_LenResponse,
				ucp_Response,
				0) != 0x0001) {
	        l_hUdev->Mouse1stMovto = 0;
            l_hUdev->CurrMouseOp[2] = 0;
            l_hUdev->CurrMouseOp[3] = 0;
            l_hUdev->CurrMouseOp[4] = 0;
			return -1;
		}
		if (x || y) {
			M_DelayRandom(10, 20);
		}
	}
    l_hUdev->CurrMouseOp[2] = 0;
    l_hUdev->CurrMouseOp[3] = 0;
    l_hUdev->CurrMouseOp[4] = 0;
    return 0;
}

#define MIN_STEP 100 //ÿ�������ƶ������ظ��������100
//�ӵ�ǰλ���ƶ����    x: x���򣨺��ᣩ�ľ���; y: y�������ᣩ�ľ���
int M_MoveR(HANDLE m_hdl,int x, int y)
{
    if(m_hdl == NULL) {
        return -1;
    }
	PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;

	if (M_InternalMoveR(m_hdl, x, y) != 0) {
		return -1;
	}

    return 0;
}

//�ƶ���굽ָ������    x: x���򣨺��ᣩ������; y: y�������ᣩ������
int M_MoveTo(HANDLE m_hdl,int x, int y)
{
    if ((m_hdl == NULL) || (x < 0) || (y < 0)) {
        return -1;
    }
	PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;

    if (l_hUdev->Mouse1stMovto == 0) {
        if (M_InternalMoveR(m_hdl, -3000, -3000) != 0) {//�ƶ���ԭ��
            return -1;
        }
        if (M_InternalMoveR(m_hdl, x, y) != 0) {//�ƶ���ԭ��
            return -1;
        }
        l_hUdev->Mouse1stMovto = 1;
        l_hUdev->CurrMouseAbsPosX = x;
        l_hUdev->CurrMouseAbsPosY = y;
    }
    else {
        if (M_InternalMoveR(m_hdl, x - l_hUdev->CurrMouseAbsPosX, y - l_hUdev->CurrMouseAbsPosY) != 0) {
            l_hUdev->Mouse1stMovto = 0;
            return -1;
        }
        l_hUdev->CurrMouseAbsPosX = x;
        l_hUdev->CurrMouseAbsPosY = y;
    }
	if (l_hUdev->CurrMouseAbsPosX < 0) {
		l_hUdev->CurrMouseAbsPosX = 0;
	}
	if (l_hUdev->CurrMouseAbsPosY < 0) {
		l_hUdev->CurrMouseAbsPosY = 0;
	}

    return 0;
}

//��mouse�ƶ���ԭ��
int M_ResetMousePos(HANDLE m_hdl)
{
    if (m_hdl == NULL) {
        return -1;
    }
	PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;
    l_hUdev->Mouse1stMovto = 0;
    M_MoveTo(m_hdl, 0, 0);
    return 0;
}

int M_GetCurrMousePos(HANDLE m_hdl,int *x, int *y)
{
    if ((m_hdl == NULL) || (x < 0) || (y < 0)) {
        return -1;
    }
	PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;
	if (l_hUdev->Mouse1stMovto == 0) {
		return -1;
	}
    *x = l_hUdev->CurrMouseAbsPosX;
    *y = l_hUdev->CurrMouseAbsPosY;
    return 0;
}

//�ͷ��������İ���
int M_ReleaseAllMouse(HANDLE m_hdl)
{
    if(m_hdl == NULL) {
        return -1;
    }
	PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;
	unsigned long dwp_LenResponse;
	unsigned char ucp_Response[32];
    l_hUdev->CurrMouseOp[0] = 0x04;
    l_hUdev->CurrMouseOp[1] = 0;
    l_hUdev->CurrMouseOp[2] = 0;
    l_hUdev->CurrMouseOp[3] = 0;
    l_hUdev->CurrMouseOp[4] = 0;
	if (HID_Ctrl_Comm(l_hUdev->hDev,
		    5,
			l_hUdev->CurrMouseOp,
			&dwp_LenResponse,
			ucp_Response,
			0) != 0x0001) {
        return -1;
	}

    return 0;
}

//���ػ��ķֱ��ʣ�ֻ�����ú����ʹ�þ��������ƶ������û�����ã���ʹ�ñ����ֱ���
int M_ResolutionUsed(HANDLE m_hdl, int x, int y)
{
    if ((m_hdl == NULL) || (x < 0) || (y < 0)) {
        return -1;
    }
	PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;
	if ((x <= 0) || (y <= 0)) {
		return -2;
	}
	l_hUdev->Cx = x;
	l_hUdev->Cy = y;
	return 0;
}

#define HW_MAX_RES 0x7FFF
int M_InternalMoveTo3(HANDLE m_hdl, int x, int y)
{
	unsigned long dwp_LenResponse, temp1, temp2;
	unsigned char ucp_cmd[16], ucp_Response[32];
	PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;

	unsigned long fact = (HW_MAX_RES/l_hUdev->Cx)/4;

	ucp_cmd[0] = 6;
	ucp_cmd[1] = 0; //���������겻�������Ҽ�
	temp1 = (HW_MAX_RES * x)/l_hUdev->Cx + fact;
	if (temp1 > HW_MAX_RES) {
		temp1 = HW_MAX_RES;
	}
	ucp_cmd[2] = (unsigned char)(temp1);
	ucp_cmd[3] = (unsigned char)(temp1 >> 8);
	temp2 = (HW_MAX_RES * y)/l_hUdev->Cy + fact;
	if ((temp1 == 0) && (temp2 == 0)) {//������������0��x=y=0���򲻻����ƶ���������һ����1
		temp2 = fact;
	}
	if (temp2 > HW_MAX_RES) {
		temp2 = HW_MAX_RES;
	}
	ucp_cmd[4] = (unsigned char)(temp2);
	ucp_cmd[5] = (unsigned char)(temp2 >> 8);
	ucp_cmd[6] = 0;

	if (HID_Ctrl_Comm(l_hUdev->hDev,
		7,
		ucp_cmd,
		&dwp_LenResponse,
		ucp_Response,
		0) != 0x0001) {
		return -1;
	}
    l_hUdev->Mouse1stMovto = 1;
	l_hUdev->CurrMouseAbsPosX = x;
    l_hUdev->CurrMouseAbsPosY = y;
    return 0;
}

//���������ƶ�
int M_MoveTo3(HANDLE m_hdl, int x, int y)
{
    if ((m_hdl == NULL) || (x < 0) || (y < 0)) {
        return -1;
    }
	PUDEVHANDLE l_hUdev = (PUDEVHANDLE)m_hdl;
	int retcode;

	if ((x < 0) || (y < 0)) {
		return -2;
	}
	if ((l_hUdev->Cx == 0) || (l_hUdev->Cy == 0)) {
		return -3;
	}
	if ((x >= l_hUdev->Cx) || (y >= l_hUdev->Cy)) {
		return -4;
	}

	if (l_hUdev->Mouse1stMovto == 0) {//��һ���ƶ�, ֱ���ƶ���û������
	    retcode = M_InternalMoveTo3(m_hdl, x, y);
	    return retcode;
    }
    else {//���ϴμ�¼��λ�ÿ�ʼ�ƶ�
        int x_dis = x - l_hUdev->CurrMouseAbsPosX;
        int y_dis = y - l_hUdev->CurrMouseAbsPosY;
        if ((x_dis == 0) && (y_dis == 0)) { //�п����ֹ� �ƶ�����꣬���Ի�����Ҫ����ִ�о����ƶ�
    	    retcode = M_InternalMoveTo3(m_hdl, x, y);
    	    return retcode;
        }
        //1 ȷ��������˳ʱ�뻹����ʱ��
        int clockwise = 0;
        if (M_RandDomNbr(1, 100) % 2 == 0) {
            clockwise = 1;
        }
        //2 �����ƶ����룬ȷ����������(���ĵ�ƫ��������)
        int len = (double)(sqrt((double)(x_dis*x_dis + y_dis*y_dis))) + 0.5; //��������
        int theta = len/M_RandDomNbr(10,50); // �����̶�1/?, �м���ƫ��
        if (theta < 2) {
            theta = M_RandDomNbr(2, 5);
        }
        else if (theta > 100) {
            theta = M_RandDomNbr(100, theta);
        }
        else {
            theta += M_RandDomNbr(2, 20);
        }
        if (clockwise) {
            theta = theta * (-1);
        }
    	int x_currpos, y_currpos, x_prevpos, y_prevpos;
        x_currpos = y_currpos = x_prevpos = y_prevpos = 0; //��ǰ��Ϊԭ�㽨�����꣬��x y�����Ѿ����˶೤
        int x_oridest = x_dis; //���������x destination
        int y_oridest = y_dis; //���������y destination
        float t;
        int x_mov, y_mov;
        int xc, yc;
        unsigned char luc_Rnd;
        if (abs(x_dis) >= abs(y_dis)) {
            //3 ȷ�����Ƶ�λ��, ��������xһ��λ�ã��򻯼���
            xc = x_oridest/2; //���Ƶ�xλ��
            yc = y_oridest/2 + theta; //���Ƶ�yλ��
            while (abs(x_dis)) {
                if ((abs(x_dis) < MIN_STEP * 2) && (abs(x_dis) > MIN_STEP)) {
                    luc_Rnd = M_RandDomNbr(abs(x_dis)/2, abs(x_dis)/2 + MIN_STEP/2);
                }
                else {
                    luc_Rnd = M_RandDomNbr(MIN_STEP, MIN_STEP + 27);
                }
                if (luc_Rnd >= abs(x_dis)) { //ֱ��ֱ���ƶ�
                    x_mov = x_dis;
                    y_mov = y_dis;
                    l_hUdev->CurrMouseAbsPosX = x;
                    l_hUdev->CurrMouseAbsPosY = y;
                }
                else {
                    if (x_dis > 0) {
                        x_currpos += luc_Rnd; //׼����y��
                    }
                    else {
                        x_currpos -= luc_Rnd; //׼����y��
                    }
                    t = ((float)x_currpos)/x_oridest;
                    y_currpos = 2 * t * (1 - t) * yc + t * t * y_oridest + 0.5;
                    x_mov = x_currpos - x_prevpos;
                    if (abs(x_mov) > 126) {
                        x_mov = (x_mov > 0) ? 126 : -126;
                        x_currpos = x_prevpos + x_mov;
                    }
                    y_mov = y_currpos - y_prevpos;
                    if (abs(y_mov) > 126) {
                        y_mov = (y_mov > 0) ? 126 : -126;
                        y_currpos = y_prevpos + y_mov;
                    }
                    if (l_hUdev->Mouse1stMovto == 1) { //�ж��Ƿ�����Ļ��
                		if ((l_hUdev->CurrMouseAbsPosX + x_mov) < 0) {
                			x_mov = 0 - l_hUdev->CurrMouseAbsPosX;
                			l_hUdev->CurrMouseAbsPosX = 0;
                			x_currpos = x_prevpos + x_mov;
                		}
                		else {
                		    l_hUdev->CurrMouseAbsPosX += x_mov;
                		}
                		if ((l_hUdev->CurrMouseAbsPosY + y_mov) < 0) {
                			y_mov = 0 - l_hUdev->CurrMouseAbsPosY;
                			l_hUdev->CurrMouseAbsPosY = 0;
                			y_currpos = y_prevpos + y_mov;
                		}
                		else {
                		    l_hUdev->CurrMouseAbsPosY += y_mov;
                		}
                    }
    				x_prevpos = x_currpos;
    				y_prevpos = y_currpos;
                }
                x_dis -= (signed char)(x_mov);
                y_dis -= (signed char)(y_mov);
                if (M_InternalMoveTo3(m_hdl, l_hUdev->CurrMouseAbsPosX, l_hUdev->CurrMouseAbsPosY) != 0) {
                    return -1;
                }
         		if (abs(x_dis) || abs(y_dis)) {
        			M_DelayRandom(10, 20);
        		}
     		    if (abs(x_mov) == 0) {//�Ѿ�������Ե
        		    break;
        		}
            }
    	}
    	else {//x_dis<y_dis
            //3 ȷ�����Ƶ�λ��, ��������yһ��λ�ã��򻯼���
            yc = y_oridest/2; //���Ƶ�xλ��
            xc = x_oridest/2 + theta; //���Ƶ�yλ��
            while (abs(y_dis)) {
                if ((abs(y_dis) < MIN_STEP * 2) && (abs(y_dis) > MIN_STEP)) {
                    luc_Rnd = M_RandDomNbr(abs(y_dis)/2, abs(y_dis)/2 + MIN_STEP/2);
                }
                else {
                    luc_Rnd = M_RandDomNbr(MIN_STEP, MIN_STEP + 27);
                }
                if (luc_Rnd >= abs(y_dis)) { //ֱ��ֱ���ƶ�
                    x_mov = x_dis;
                    y_mov = y_dis;
                    l_hUdev->CurrMouseAbsPosX = x;
                    l_hUdev->CurrMouseAbsPosY = y;
                }
                else {
                    if (y_dis > 0) {
                        y_currpos += luc_Rnd; //׼����x��
                    }
                    else {
                        y_currpos -= luc_Rnd; //׼����x��
                    }
                    t = ((float)y_currpos)/y_oridest;
                    x_currpos = 2 * t * (1 - t) * xc + t * t * x_oridest + 0.5;
                    x_mov = x_currpos - x_prevpos;
                    if (abs(x_mov) > 126) {
                        x_mov = (x_mov > 0) ? 126 : -126;
                        x_currpos = x_prevpos + x_mov;
                    }
                    y_mov = y_currpos - y_prevpos;
                    if (abs(y_mov) > 126) {
                        y_mov = (y_mov > 0) ? 126 : -126;
                        y_currpos = y_prevpos + y_mov;
                    }
                    if (l_hUdev->Mouse1stMovto == 1) { //�ж��Ƿ�����Ļ��
                		if ((l_hUdev->CurrMouseAbsPosX + x_mov) < 0) {
                			x_mov = 0 - l_hUdev->CurrMouseAbsPosX;
                			l_hUdev->CurrMouseAbsPosX = 0;
                			x_currpos = x_prevpos + x_mov;
                		}
                		else {
                		    l_hUdev->CurrMouseAbsPosX += x_mov;
                		}
                		if ((l_hUdev->CurrMouseAbsPosY + y_mov) < 0) {
                			y_mov = 0 - l_hUdev->CurrMouseAbsPosY;
                			l_hUdev->CurrMouseAbsPosY = 0;
                			y_currpos = y_prevpos + y_mov;
                		}
                		else {
                		    l_hUdev->CurrMouseAbsPosY += y_mov;
                		}
                    }
    				x_prevpos = x_currpos;
    				y_prevpos = y_currpos;
                }
                x_dis -= (signed char)(x_mov);
                y_dis -= (signed char)(y_mov);
                if (M_InternalMoveTo3(m_hdl, l_hUdev->CurrMouseAbsPosX, l_hUdev->CurrMouseAbsPosY) != 0) {
                    return -1;
                }
         		if (abs(x_dis) || abs(y_dis)) {
        			M_DelayRandom(10, 20);
        		}
     		    if (abs(y_mov) == 0) {//�Ѿ�������Ե
        		    break;
        		}
            }
    	}
    	if (abs(x_dis) || abs(y_dis)) {//�������ʣ��û���ƶ���
            if (M_InternalMoveTo3(m_hdl, x, y) != 0) {
                return -1;
            }
        }
    }
    return 0;
}

/*******************ͨ�ò�������****************************/
int M_RandDomNbr(int Min_V, int Max_V)
{
	if (Min_V > Max_V) {
		Min_V = 0;
	}
  	unsigned int temp = Max_V - Min_V;
	if (temp >= RAND_MAX) {
		temp = RAND_MAX - 1;
	}
    return(rand()%temp + Min_V);
}

//��ʱָ��ʱ��  time:��λms
int M_Delay(int time)
{
    usleep(time * 1000);
	return 0;
}
//����С���ֵ֮����ʱ���ʱ��  Min_time:��С��ʱʱ��; Max_time: �����ʱʱ�� ����λ��ms��
int M_DelayRandom(int Min_time, int Max_time)
{
	usleep(M_RandDomNbr(Min_time, Max_time) * 1000);
	return 0;
}
