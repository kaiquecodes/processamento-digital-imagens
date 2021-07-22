#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


int main(int, char** argv){
  Mat image, image1,image2,image3,image4,imageS;

  String file,file1,file2,file3,file4;
  int width=0,height=0;
  
  image= imread(argv[1],IMREAD_GRAYSCALE);
  if(!image.data)
    cout << "nao abriu imagem" << endl;

  namedWindow("janela", WINDOW_AUTOSIZE);

  width = image.cols;
  height = image.rows;

  imageS.create(height,width,CV_8U);
   
 
  image1 = image(Rect(0, 0, width/2, height/2));
  image2 = image(Rect(width/2, 0, width/2, height/2));
  image3 = image(Rect(0, height/2, width/2, height/2));
  image4 = image(Rect(width/2, height/2, width/2, height/2));
 
  cout << "Digite dos nome do arquivo: " << endl;

  cin >> file;
  cin >> file1;
  cin >> file2;
  cin >> file3;
  cin >> file4;
  

  imwrite(file1 + ".png",image1);
  imwrite(file2 + ".png",image2);
  imwrite(file3 + ".png",image3);
  imwrite(file4 + ".png",image4);
 

image4.copyTo(imageS(Rect(0, 0, width/2, height/2))); 

image3.copyTo(imageS(Rect(width/2-1,0, width/2, height/2))); 

image2.copyTo(imageS(Rect(0,height/2-1, width/2, height/2))); 

image1.copyTo(imageS(Rect(width/2-1,height/2-1, width/2, height/2))); 


  imshow("janela", imageS);

  imwrite(file + ".png",imageS);
  
  waitKey();
  
  return 0;
}
