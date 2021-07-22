#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char** argv){
  Mat image;
  Vec3b val;
  Point p1;
  Point p2;
  String file;
  
  image= imread(argv[1],IMREAD_GRAYSCALE);
  if(!image.data)
    cout << "nao abriu imagem" << std::endl;

  namedWindow("janela", WINDOW_AUTOSIZE);
  
  cout << "Digite as coordenadas de P1: " << endl;

  cin >> p1.x;
  cin >> p1.y; 

  cout << "Digite as coordenadas de P2: " << endl;
 
  cin >> p2.x;
  cin >> p2.y; 

  cout << "Digite o nome do arquivo: " << endl;

  cin >> file;
  
  for(int i=p1.y;i<p2.y;i++){
    for(int j=p1.x;j<p2.x;j++){
      image.at<uchar>(i,j)= 255 - image.at<uchar>(i,j);
    }
  }

  imshow("janela", image);

  imwrite(file + ".png",image);

  waitKey();
  
  return 0;
}