#include<iostream>
#include<opencv2/opencv.hpp>
#include<math.h>
 
using namespace std;
using namespace cv;
int main(int argc, char* argv[])
{
    double x,y;
    Mat one=imread("/home/ubuntu123/map/kkkkk.png");//地图
    Mat oned=imread("/home/ubuntu123/map/mmm.png");//模板检测
    cvtColor(one,one,COLOR_BGR2GRAY);
     cvtColor(oned,oned,COLOR_BGR2GRAY);
imshow("one",one);
imshow("oned",oned);
    Mat three;
    double min ,max;
    Point min2 ,max2;
    matchTemplate(one,oned,three,TM_SQDIFF_NORMED);//模板匹配
    imshow("three",three);
    minMaxLoc(three,&min,&max,&min2,&max2);
    x=5.0*min2.x/one.cols;
    y=5.0*(one.rows-min2.y)/one.rows;
    cout<<min<<"  "<<x<<","<<y<<endl;
    circle(one,min2,70,0);
    imshow("onev",one);
    waitKey();
    return 0;
}
