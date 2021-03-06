#include <numeric>
#include <ctime>
#include <cstdlib>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define STEP 5
#define JITTER 3
#define RAIO 1

int top_slider = 1;
int slider_max = 100;

char TrackbarName[50];

Mat image, border,cannypot;
  int width, height;
  Vec3b color;
  int x, y;
  vector<int> yrange;
  vector<int> xrange;

void canny(int, void*){

  Canny(image, border, top_slider, 3*top_slider);
   
  cannypot = Mat(height, width, CV_8UC3, Scalar(255,255,255));
  for(int i =0; i<width; i++){
    for(int j =0; j<height; j++){
      if(border.at<uchar>(j,i) == 255)
      {
          x = i+rand()%(2*JITTER)-JITTER+1;
          y = j+rand()%(2*JITTER)-JITTER+1;
          color = image.at<Vec3b>(y,x);
          circle(cannypot,
                 cv::Point(x,y),
                 RAIO,
                 CV_RGB(color[2],color[1],color[0]),
                 -1,
                 LINE_AA);
      }
    }
  }
  imshow("canny",cannypot);

}

int main(int argc, char**argv){

  image= imread(argv[1],IMREAD_COLOR);
  
  srand(time(0));

  width=image.size().width;
  height=image.size().height;

  xrange.resize(height/STEP);
  yrange.resize(width/STEP);

  iota(xrange.begin(), xrange.end(), 0);
  iota(yrange.begin(), yrange.end(), 0);

  for(uint i=0; i<xrange.size(); i++){
    xrange[i]= xrange[i]*STEP+STEP/2;
  }

  for(uint i=0; i<yrange.size(); i++){
    yrange[i]= yrange[i]*STEP+STEP/2;
  }

  
  sprintf(TrackbarName,"Threshold_inferior-%d",slider_max);
  namedWindow("canny",1);
  createTrackbar( TrackbarName, "canny",
                &top_slider,
                slider_max,
                canny );
           
  canny(top_slider, 0 );

  waitKey();
  imwrite("cannypot.png",cannypot);
  return 0;
}