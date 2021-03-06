#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){ 
  Mat image, image2;
  int width, height;
  VideoCapture cap;
  vector<Mat> planes, planes2; 
  Mat histR, histR2;
  int nbins = 64; 
  float range[] = {0, 255}; 
  const float *histrange = { range }; 
  bool uniform = true;  
  bool acummulate = false;  
  int key;

	cap.open(0); 

  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }

  cap.set(CAP_PROP_FRAME_WIDTH, 640); 
  cap.set(CAP_PROP_FRAME_HEIGHT, 480);
  width = cap.get(CAP_PROP_FRAME_WIDTH);
  height = cap.get(CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  int histw = nbins, histh = nbins/2; 
  Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0)); 
  Mat histImgR2(histh, histw, CV_8UC3, Scalar(0,0,0));
  

  while(1){
    cap >> image;
    split (image, planes2); 
    calcHist(&planes2[0], 1, 0, Mat(), histR, 1,
                 &nbins, &histrange,
                 uniform, acummulate); 
              

    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    

    histImgR.setTo(Scalar(0)); 
       

    for(int i=0; i<nbins; i++){
      line(histImgR,
               Point(i, histh),
               Point(i, histh-cvRound(histR.at<float>(i))),
               Scalar(0, 0, 255), 1, 8, 0);
            
      
    }
    histImgR.copyTo(image(Rect(0, 0       ,nbins, histh)));

    cap >> image2;
    split (image2, planes); 

    calcHist(&planes[0], 1, 0, Mat(), histR2, 1,
                 &nbins, &histrange,
                 uniform, acummulate); 

    normalize(histR2, histR2, 0, histImgR2.rows, NORM_MINMAX, -1, Mat());

    histImgR2.setTo(Scalar(0)); 
    
 for(int i=0; i<nbins; i++){
      
      line(histImgR2,
               Point(i, histh),
               Point(i, histh-cvRound(histR2.at<float>(i))),
               Scalar(0, 0, 255), 1, 8, 0);        
      
    }
   histImgR2.copyTo(image(Rect(0, 2*histh   ,nbins, histh)));


    double comp = compareHist(histR,histR2,0);

    cout << comp << endl;

    if(comp<0.996500){
      putText(image, "Motion Detected", Point(image.cols/2 - 50, 40), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0, 0, 255), 2);
    }

    imshow("image", image);
    key = waitKey(30);
    if(key == 27) break;
  }
  return 0;
}