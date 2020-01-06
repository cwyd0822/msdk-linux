#include<unistd.h>
#include<stdlib.h>
#include<time.h>
#include "msdk.h"

HANDLE m_hdl = NULL;
int main(int argc, char *argv[]) 
{
    m_hdl = M_Open(1);
    if (m_hdl != NULL) {
        printf("Open Success\n");
    }
    else {
        printf("Open Fail\n");
        return -1;
    }

    M_Delay(5000);//5s
    if (M_MoveR(m_hdl, 100, 100) == 0) {
        printf("MoveR Success\n");
    }
    else {
        printf("MoveR Fail\n");
    }
    usleep(500000); //500ms

    if (M_MoveTo(m_hdl, 500, 500) == 0) {
        printf("MoveTo Success\n");
    }
    else {
        printf("MoveTo Fail\n");
    }

    M_DelayRandom(2000, 3000);
    // if (M_KeyPress(m_hdl, Keyboard_a, 2048) == 0) {
    //    printf("KeyPress Success\n");
    // }
    srand((unsigned int)time(NULL));
    for (int i = 0; i < 2048; i ++) {
        int key = rand() % 35 + 4;
        printf("正在向目标系统输入： %c", key);
        M_KeyPress(m_hdl, key, 1);
    }
    // else {
    //    printf("KeyPress Fail\n");
    // }
    usleep(500000); //500ms

    if (M_ResolutionUsed(m_hdl, 1920, 1080) == 0) { //���ݱ��ػ���Ļ�ֱ��ʣ������޸�
        printf("M_ResolutionUsed Success\n");
    }
    else {
        printf("M_ResolutionUsed Fail\n");
    }
    
    if (M_MoveTo3(m_hdl, 800, 168) == 0) {
        printf("M_MoveTo3 Success\n");
    }
    else {
        printf("M_MoveTo3 Fail\n");
    }
    
    if (M_Close(m_hdl) == 0) {
        printf("Close Success\n");
    }
    else {
        printf("Close Fail\n");
    }
    return 0;
}
