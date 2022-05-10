#include <mygraph.h>
#include "page0.h"
#include "page8.h"
#include"catalog.h"
#include "IMAGE.h"
#include "hz.h"

void h_main(void)
{
    cleardevice();
    hide_mouse();                           //“˛≤ÿ Û±Í
    barxy(0, 0, 640, 480, CYAN);            //ÃÓ≥‰±≥æ∞
    setcolor(CYAN);
    puthz(20, 150,"»Àπ§÷«ƒ‹¥Û¬•÷«ª€Ω®‘ÏœµÕ≥",48,50,WHITE);
    puthz(280, 340,"ª∂”≠",32,40,WHITE);

    barxy(620,0,640,20, WHITE);             //ª≠πÿ±’∞¥≈•
    rectangle(620,0,640,20);
    setcolor(DARKGRAY);
    line(620,0,640,20);
    line(620,20,640,0);
    
    show_mouse();                            //œ‘ æ Û±Í
}

void g_main(void)
{     
    h_main();   //ªÊÕº
    getmouse(); // Û±ÍºÏ≤‚
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

        getmouse(); // Û±ÍºÏ≤‚
    }
}









/*int size;
    void *buffer;
    size=imagesize(0,0,640,480);
    buffer=malloc(size);
    getimage(0,0,640,480,buffer);
    //IMAGE img;
    */