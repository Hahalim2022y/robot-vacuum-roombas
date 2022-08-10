#include <iostream>
#include <opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <vector>
#include <boost/concept_check.hpp>
using namespace std;
using namespace cv;
 
int main(int argc, char **argv) {
    Mat src = imread("/home/ubuntu123/map/kkkkk.png");
    imshow("1",src);
    if(src.empty())
    {
        cout << "can not open "  << endl;
        return -1;
    }
    Mat cimg;
    medianBlur(src, src, 5);
    cvtColor(src,cimg,COLOR_BGR2GRAY);
   GaussianBlur(cimg, cimg, Size(9, 9), 2, 2);
  //  medianBlur(cimg, cimg, 5);
    Canny(cimg,cimg,10,250,5);
    imshow("canny",cimg);
    
   vector<Vec3f> circles;
  HoughCircles(cimg, circles, HOUGH_GRADIENT, 1, 30,100,35, 1, 120);
  
   for( size_t i = 0; i < circles.size(); i++ )
{
//参数定义
Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
int radius = cvRound(circles[i][2]);
//绘制圆心
circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
//绘制圆轮廓
circle( src, center, radius, Scalar(155,50,255), 3, 8, 0 );
//打印圆心坐标
//printf("x = %d,y = %d\n",cvRound(circles[i][0]),cvRound(circles[i][1]));
printf("x = %d,y = %d\n",cvRound(circles[i][0])/710*5,cvRound(circles[i][1])/728*5);
}

   imshow("2",src);
   //cv::imwrite("/home/ubuntu123/map/555.jpg", cimg);
    cv::waitKey(0);
    return 0;
}
