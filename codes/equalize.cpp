#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image;
  int width, height;
  VideoCapture cap;
  vector<Mat> planG; 
  vector<Mat> planE; 
  Mat histR, histE; 
  int nbins = 64; 
  float range[] = {0, 255};
  const float *histrange = { range };
  bool uniform = true; 
  bool acummulate = false;  
  int key;
  Mat gray; 
  Mat equa; 
  
  cap.open(0);

  if(!cap.isOpened()){
    std::cout << "cameras indisponiveis";
    return -1;
  }

  cap.set(cv::CAP_PROP_FRAME_WIDTH, 640); 
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
  width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
  height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  int histw = nbins, histh = nbins/2; 
  Mat histImgR(histh, histw, CV_8UC1, Scalar(0,0,0));
  Mat histImgE(histh, histw, CV_8UC1, Scalar(0,0,0));

  
  while(1){
    cap >> image;
    cvtColor(image, gray, COLOR_BGR2GRAY ); //converte grayscale
    split(gray, planG); 
  
    calcHist(&planG[0], 1, 0, Mat(), histR, 1,
                 &nbins, &histrange,
                 uniform, acummulate); 
                   
    normalize(histR, histR, 0, histImgR.rows, cv::NORM_MINMAX, -1, cv::Mat());
    
    histImgR.setTo(Scalar(0));
    
    for(int i=0; i<nbins; i++){
      line(histImgR,
               Point(i, histh),
               Point(i, histh-cvRound(histR.at<float>(i))),
               Scalar(255), 1, 8, 0);
      
    }
    
    histImgR.copyTo(gray(Rect(0, 0       ,nbins, histh)));
      
    imshow("Grayscale", gray);
     

    equalizeHist(gray, equa ); //equaliza
    split (equa, planE); 
    calcHist(&planE[0], 1, 0, Mat(), histE, 1,
                 &nbins, &histrange,
                 uniform, acummulate); 
                   
    normalize(histE, histE, 0, histImgE.rows, cv::NORM_MINMAX, -1, cv::Mat());
    
    histImgE.setTo(Scalar(0));
    
    for(int i=0; i<nbins; i++){
      line(histImgE,
               Point(i, histh),
               Point(i, histh-cvRound(histE.at<float>(i))),
               Scalar(255), 1, 8, 0);
      
    }
    
    histImgE.copyTo(equa(Rect(0, 0       ,nbins, histh)));
       
    imshow("Equalize",equa);
  
    key = cv::waitKey(30);
    if(key == 27) break;

 }

  return 0;
}