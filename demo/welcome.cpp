#include <mygraph.h>
#include "page0.h"
#include "page8.h"
#include"catalog.h"
#include "IMAGE.h"
#include "hz.h"

void h_main(void)
{
    cleardevice();
    hide_mouse();                           //�������
    barxy(0, 0, 640, 480, CYAN);            //��䱳��
    setcolor(CYAN);
    puthz(20, 150,"�˹����ܴ�¥�ǻ۽���ϵͳ",48,50,WHITE);
    puthz(280, 340,"��ӭ",32,40,WHITE);

    barxy(620,0,640,20, WHITE);             //���رհ�ť
    rectangle(620,0,640,20);
    setcolor(DARKGRAY);
    line(620,0,640,20);
    line(620,20,640,0);
    
    show_mouse();                            //��ʾ���
}

void g_main(void)
{     
    h_main();   //��ͼ
    getmouse(); //�����
    while (1)
    {
        if (M_sc > 0)
        {
            if (pd(280,335,355,375))
            {
                g_catalog();
            }
                       
            else if (pd(620,0,640,20))
            {
                exit(0);
            }
        }

        getmouse(); //�����
    }
}









/*int size;
    void *buffer;
    size=imagesize(0,0,640,480);
    buffer=malloc(size);
    getimage(0,0,640,480,buffer);
    //IMAGE img;
    */